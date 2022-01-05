CREATE EXTENSION shamsi_ext;

SELECT shamsi_part('year', '2019-07-07 14:10:52.84937+04:30');
SELECT shamsi_part('month', '2019-07-07 14:10:52.84937+04:30');
SELECT shamsi_part('day', '2019-07-07 14:10:52.84937+04:30');
SELECT shamsi_part('hour', '2019-07-07 14:10:52.84937+04:30');
SELECT shamsi_part('minute', '2019-07-07 14:10:52.84937+04:30');
SELECT shamsi_part('second', '2019-07-07 14:10:52.84937+04:30');
SELECT shamsi_part('doy', '2019-07-07'); -- 1398/04/16
SELECT shamsi_part('doy', '2019-09-22'); -- 1398/06/31
SELECT shamsi_part('doy', '2020-03-19'); -- 1398/12/29
SELECT shamsi_part('doy', '2021-03-20'); -- 1399/12/30
SELECT shamsi_part('doy', '2021-03-21'); -- 1400/01/01
SELECT shamsi_part('quarter', '2021-03-21');
SELECT shamsi_part('quarter', '2021-03-20');
SELECT shamsi_part('decade', '2021-03-21');
SELECT shamsi_part('decade', '2021-03-20');
SELECT shamsi_part('century', '2020-03-20');
SELECT shamsi_part('century', '2022-03-20');
SELECT shamsi_part('century', '2022-03-21');
SELECT shamsi_part('dow', '2001-10-16');
SELECT shamsi_part('dow', '2020-10-17');
SELECT shamsi_part('dow', '2020-10-23');

DROP EXTENSION shamsi_ext;