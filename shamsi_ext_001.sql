CREATE OR REPLACE FUNCTION shamsi_format(
    timestamp WITH TIME ZONE,
    with_time BOOLEAN DEFAULT TRUE
)
RETURNS TEXT AS 'MODULE_PATHNAME', 'shamsi_format_timestamp'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION shamsi_format(
    date
)
RETURNS TEXT AS 'MODULE_PATHNAME','shamsi_format__date'
LANGUAGE C STRICT;


CREATE OR REPLACE FUNCTION shamsi_part(
    text,
    timestamp WITH TIME ZONE
)
RETURNS INTEGER AS 'MODULE_PATHNAME','shamsi_part_timestamp'
LANGUAGE C STRICT;

CREATE OR REPLACE FUNCTION shamsi_part(
    text,
    date
)
RETURNS INTEGER AS 'MODULE_PATHNAME','shamsi_part_date'
LANGUAGE C STRICT;