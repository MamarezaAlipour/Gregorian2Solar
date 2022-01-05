MODULES = shamsi_ext
EXTENSION = shamsi_ext
TESTS = $(wildcard sql/*.sql)
REGRESS = $(patsubst sql/%.sql,%,$(TESTS))
DATA = shamsi_ext_001.sql
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)