#!/bin/sh
# This script generates and runs the algorithm test binary
# and generates function plots from the resulting log data.

MIN_SIZE=1
MAX_SIZE=100000
QMAKE=`which qmake 2>/dev/null`
GNUPLOT=`which gnuplot 2>/dev/null`
SATF=./satf
LOG_DIR=logs
PLOT_DIR=plots

# Check for optional commandline arguments
if [ $# -eq 2 ] ; then
    MIN_SIZE=$1 ; MAX_SIZE=$2
fi

# Check if executable exists, otherwise build it
if test ! -x $SATF ; then
    echo "Building \"$SATF\"..."; $QMAKE && mak
fi
if test ! -x $SATF ; then
    echo "Unable to build \"$SATF\"!"; exit 1
fi

# Run executable to generate performance measurement data
echo "Running executable \"$SATF\" to generate measurement data..."
${SATF} ${MIN_SIZE} ${MAX_SIZE}

# Sort contents of files in the last generated log dir numerically.
# Also retrieve the maximum computation time of all algorithms for
# use in gnuplot later on.
LAST_LOG=`ls $LOG_DIR | tail -n1`
LAST_LOG_DIR=`echo $LOG_DIR/$LAST_LOG`
echo "Sorting log files in \"$LAST_LOG_DIR\" in arithmetical order..."
for FILE in `ls $LAST_LOG_DIR`; do
    sort -n $LAST_LOG_DIR/$FILE -o $LAST_LOG_DIR/$FILE
done
#TODO: Retrieve this!
MAX_TIME=10

# Generate function plots for each log file in LAST_LOG_DIR
if [ -x $GNUPLOT ]; then
    LAST_PLOT_DIR=`echo $PLOT_DIR/$LAST_LOG`
    mkdir -p $LAST_PLOT_DIR
    echo "Generating plots in \"$LAST_PLOT_DIR\"..."
    for FILE in `ls $LAST_LOG_DIR`; do
        `gnuplot << EOF
            set xlabel "data size [number of elements]"
            set xrange [$MIN_SIZE:$MAX_SIZE]
            set ylabel "used time [milliseconds]"
            set yrange [0:$MAX_TIME]
            set grid
            set terminal png
            set output '$LAST_PLOT_DIR/$FILE.png'
            plot '$LAST_LOG_DIR/$FILE' using 1:2 '%lf,%lf' title "$LAST_LOG_DIR/$FILE" with lines
            quit
            EOF`
    done
fi
