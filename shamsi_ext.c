#include "postgres.h"
#include "parser/scansup.h"
#include "pgtime.h"
#include "nodes/makefuncs.h"
#include "utils/timestamp.h"
#include "utils/datetime.h"
#include "utils/date.h"

const int greg_days_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const int shamsi_days_month[12] = {31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29};

PG_MODULE_MAGIC;
PG_FUNCTION_INFO_V1(shamsi_format_timestamp);
PG_FUNCTION_INFO_V1(shamsi_format_date);
PG_FUNCTION_INFO_V1(shamsi_part_timestamp);
PG_FUNCTION_INFO_V1(shamsi_part_date);

struct pg_tm tm_to_shamsi(struct pg_tm *tm);
int time_t_to_shamsi_part(text *units, pg_time_t *t);

struct pg_tm tm_to_shamsi(struct pg_tm *tm)
{
    struct pg_tm shamsi;
    int shamsi_day_no, s_np;
    int greg_day_no, i;

    int gy = tm->tm_year + 300;

    greg_day_no = 365 * gy + (gy + 3) / 4 - (gy + 99) / 100 + (gy + 399) / 400;

    for (i = 0; i < tm->tm_mon; i++)
    {
        greg_day_no += greg_days_month[i];
    }

    if (tm->tm_mon > 1 && ((gy % 4 == 0 && gy % 100 != 0) || (gy % 400 == 0)))
    {
        greg_day_no += 1;
    }
    greg_day_no += (tm->tm_mday - 1);
    shamsi_day_no = greg_day_no - 79;

    s_np = shamsi_day_no / 12053;
    shamsi_day_no %= 12053;
    shamsi.tm_year = 979 + (33 * s_np) + 4 * (shamsi_day_no / 1461);

    shamsi_day_no %= 1461;

    if (shamsi_day_no >= 366)
    {
        shamsi.tm_year += (shamsi_day_no - 1) / 365;
        shamsi_day_no = (shamsi_day_no - 1) % 365;
    }
    for (i = 0; i < 11; i++)
    {
        if (shamsi_day_no < shamsi_days_month[i])
        {
            i -= 1;
            break;
        }
        shamsi_day_no -= shamsi_days_month[i];
    }

    shamsi.tm_mon = i + (i == 11 ? 1 : 2);
    shamsi.tm_mday = shamsi_day_no + 1;
    shamsi.tm_hour = tm->tm_hour;
    shamsi.tm_min = tm->tm_min;
    shamsi.tm_sec = tm->tm_sec;

    if (shamsi.tm_mon <= 6)
    {
        shamsi.tm_yday = (shamsi.tm_mon - 1) * 31 + shamsi.tm_mday;
    }
    else
    {
        shamsi.tm_yday = 186 + (shamsi.tm_mon - 7) * 30 + shamsi.tm_mday;
    }
    shamsi.tm_wday = (tm->tm_wday + 1) % 7;

    return shamsi;
};

Datum shamsi_format_timestamp(PG_FUNCTION_ARGS)
{
    TimestampTz timestamp = PG_GETARG_TIMESTAMPTZ(0);
    bool with_time = PG_GETARG_BOOL(1);

    pg_time_t t = timestamptz_to_time_t(timestamp);
    pg_tz *tehran = pg_tzset("Asia/Tehran");
    struct pg_tm *tm = pg_localtime(&t, tehran);
    struct pg_tm sdate = tm_to_shamsi(tm);

    if (with_time)
    {
        text *output = (text *)palloc(VARHDRSZ + 19);
        char *result = psprintf("%d/%02d/%02d %02d:%02d:%02d",
                                sdate.tm_year, sdate.tm_mon, sdate.tm_mday,
                                sdate.tm_hour, sdate.tm_min, sdate.tm_sec);
        SET_VARSIZE(output, VARHDRSZ + 19);
        memcpy(output->vl_dat, result, 19);
        PG_RETURN_CSTRING(output);
        PG_RETURN_TEXT_P(output);
    }
    else
    {
        text *output = (text *)palloc(VARHDRSZ + 10);
        char *result = psprintf("%d/%02d/%02d",
                                sdate.tm_year, sdate.tm_mon, sdate.tm_mday);
        SET_VARSIZE(output, VARHDRSZ + 10);
        memcpy(output->vl_dat, result, 10);
        PG_RETURN_TEXT_P(output);
    }
}

Datum shamsi_format_date(PG_FUNCTION_ARGS)
{
    DateADT date = PG_GETARG_DATEADT(0);
    Timestamp timestamp = date2timestamp_no_overflow(date);

    pg_time_t t = timestamptz_to_time_t(timestamp);
    struct pg_tm tm = *pg_gmtime(&t);
    struct pg_tm sdate = tm_to_shamsi(&tm);
    text *output = (text *)palloc(VARHDRSZ + 10);
    char *result = psprintf("%d/%02d/%02d",
                            sdate.tm_year, sdate.tm_mon, sdate.tm_mday);
    SET_VARSIZE(output, VARHDRSZ + 10);
    memcpy(output->vl_dat, result, 10);
    PG_RETURN_TEXT_P(output);
}

int time_t_to_shamsi_part(text *units, pg_time_t *t)
{
    char *lowunits;
    int type, val;
    float8 result;

    pg_tz *tehran = pg_tzset("Asia/Tehran");
    struct pg_tm *tm = pg_localtime(t, tehran);
    struct pg_tm sdatetime = tm_to_shamsi(tm);

    lowunits = downcase_truncate_identifier(VARDATA_ANY(units),
                                            VARSIZE_ANY_EXHDR(units),
                                            false);

    type = DecodeUnits(0, lowunits, &val);

    if (type == UNKNOWN_FIELD)
        type = DecodeSpecial(0, lowunits, &val);

    switch (val)
    {
    case DTK_SECOND:
        result = (&sdatetime)->tm_sec;
        break;

    case DTK_MINUTE:
        result = (&sdatetime)->tm_min;
        break;

    case DTK_HOUR:
        result = (&sdatetime)->tm_hour;
        break;

    case DTK_DAY:
        result = (&sdatetime)->tm_mday;
        break;
    case DTK_MONTH:
        result = (&sdatetime)->tm_mon;
        break;
    case DTK_YEAR:
        result = (&sdatetime)->tm_year;
        break;
    case DTK_DOW:
        result = tm->tm_year;
        result = j2day(date2j(tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday)) - 6;
        if (result < 0)
        {
            result += 7;
        }
        break;
    case DTK_DOY:
        if ((&sdatetime)->tm_mon <= 6)
        {
            result = (((&sdatetime)->tm_mon - 1) * 31) + (&sdatetime)->tm_mday;
        }
        else
        {
            result = 186 + (&sdatetime)->tm_mday;
            result += ((&sdatetime)->tm_mon - 7) * 30;
        }
        break;

    case DTK_QUARTER:
        result = ((&sdatetime)->tm_mon - 1) / 3 + 1;
        break;
    case DTK_DECADE:
        if ((&sdatetime)->tm_year > 0)
            result = ((&sdatetime)->tm_year / 10) * 10;
        else
            result = -((8 - ((&sdatetime)->tm_year - 1)) / 10) * 10;
        break;
    case DTK_CENTURY:
        result = ((&sdatetime)->tm_year + 99) / 100;
        break;

    default:
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                 errmsg("\"time\" units \"%s\" not recognized",
                        lowunits)));
        result = 0;
    }
    return result;
}

Datum shamsi_part_timestamp(PG_FUNCTION_ARGS)
{
    text *units = PG_GETARG_TEXT_PP(0);
    TimestampTz timestamp = PG_GETARG_TIMESTAMPTZ(1);
    pg_time_t t = timestamptz_to_time_t(timestamp);
    int result = time_t_to_shamsi_part(units, &t);
    PG_RETURN_INT32(result);
}

Datum shamsi_part_date(PG_FUNCTION_ARGS)
{
    text *units = PG_GETARG_TEXT_PP(0);
    DateADT date = PG_GETARG_DATEADT(1);
    TimestampTz timestamp = date2timestamp_no_overflow(date);
    pg_time_t t = timestamptz_to_time_t(timestamp);
    int result = time_t_to_shamsi_part(units, &t);
    PG_RETURN_INT32(result);
}
