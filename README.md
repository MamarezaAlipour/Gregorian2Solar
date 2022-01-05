# Shamsi Extension (A Solar utils for PostgreSQL)
Shamsi Ext is a package for converting Gregorian dates to Solar Hijri.
This package consists of the following functions:

extensions
```sql
shamsi_format(TIMESTAMP WITH TIME ZONE, with_time BOOLEAN DEFAULT TRUE)
shamsi_format(DATE)
shamsi_part(TEXT, TIMESTAMP WITH TIME ZONE)
shamsi_part(TEXT, DATE)
```

Examples:
```sql
SELECT shamsi_format(now()); -->   1399/07/08 23:45:37
SELECT shamsi_format(now(), false); -->   1399/07/08
SELECT shamsi_format('2019-07-07', false); -->  1398/04/16
SELECT shamsi_format('2021-03-20'); -->  1399/12/30 00:00:00
SELECT shamsi_format('2021-03-20'::date); -->  1399/12/30
SELECT shamsi_part('year', '2019-07-07 14:10:52.84937+04:30'); --> 1398
SELECT shamsi_part('minute', '2019-07-07 14:10:52.84937+04:30'); --> 10
SELECT shamsi_part('doy', '2019-07-07 14:10:52.84937+04:30'); --> 109
SELECT shamsi_part('dow', '2019-07-07'); --> 4
```

`shamsi_part` function can accept these values as the first parameter:
* year
* month
* day
* hour
* minute
* second
* doy (day of year)
* dow (day of week with index zero as saturday)
* quarter
* decade
* century

# Installation

Dependencies:
```shell
sudo apt install postgresql-server-dev-14
```

Run make to build and install
```shell
make
sudo make install
```

Run this command in your psql shell:
```shell
\d your_db_name

> CREATE EXTENSION jalali_utils;
```

# Running tests
```shell
make installcheck
```
# Gregorian2Solar
