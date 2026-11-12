# Copyright (C) 2026 Nicolas San Martin
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

P0=$1 # program to compare ex. stdin-count
P1=$2 # second program to compare ex. stdin-count-cppmap
DATA=$3 # file with data to fill P0 and P1's stdin

echo $DATA # print data source
echo $P0
time cat $DATA | $P0 | wc

echo $P1
time cat $DATA | $P1 | wc

echo diff:
diff <(cat $DATA | $P0 | sort) <(cat $DATA | $P1 | sort)
echo END
