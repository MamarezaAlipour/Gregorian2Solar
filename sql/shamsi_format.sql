CREATE EXTENSION shamsi_ext;

SELECT shamsi_format('2022-01-05 14:10:52.84937+04:30'::timestamptz, false);
SELECT shamsi_format('2022-01-05 14:10:52.84937+04:30'::timestamptz);
SELECT shamsi_format('2022-01-05 14:10:52.84937+04:30'::date);
SELECT shamsi_format('2015-10-14'::date);
SELECT shamsi_format('2001-05-27'::date);
SELECT shamsi_format('2000-06-14'::date);
SELECT shamsi_format('1998-02-10'::date);
DROP EXTENSION shamsi_ext;