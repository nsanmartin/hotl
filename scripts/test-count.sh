DIR=$1 # Dir where to store data
N=$2
SEED=$3   # N to pass as SEED

BUILD=../build
P0=$BUILD/stdin-count
P1=$BUILD/stdin-count-cppmap
DGEN=$BUILD/rand-word8s 

DATA=$DIR/test_data_${N} #_${N}
DATA_DIFF=${DATA}_${SEED}.diff
HOTL=${DATA}_HOTL
CPPM=${DATA}_CPPM

if [[ -z "$DIR" || -z "$N" || -z "$SEED" ]] ; then
    echo usage: $0 DIR N SEED 
    exit 1
elif [[ ! -d "$BUILD" || ! -f "$P0" || ! -f "$P1" || ! -f "$DGEN" ]]; then
    echo You must build first
    exit 1
else

    echo $DGEN $N ---- $SEED ----

    $DGEN $N $SEED > $DATA

    wc $DATA

    echo $P0
    # time cat $DATA | $P0 | sort > $HOTL
    cat $DATA | $P0 | sort > $HOTL

    echo $P1
    cat $DATA | $P1 | sort > $CPPM
    # time cat $DATA | $P1 | sort > $CPPM

    echo diff:
    diff -q $HOTL $CPPM | tee $DATA_DIFF
    if [[ -s "$DATA_DIFF" ]]; then 
        exit 1
    fi
    rm $HOTL
    rm $CPPM
    rm $DATA
fi
