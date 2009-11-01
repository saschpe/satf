#!/bin/sh
# This script generates and runs the algorithm test binary
# and generates function plots from the resulting log data.

MIN_SIZE=1
MAX_SIZE=1000
QMAKE=`which qmake 2>/dev/null`
GNUPLOT=`which gnuplot 2>/dev/null`
SATF=satf
PROJECT_FILE=satf.pro
BUILD_DIR=build
LOG_DIR=logs
PLOT_DIR=plots

# Check for optional commandline arguments
if [ $# -eq 2 ] ; then
    MIN_SIZE=$1 ; MAX_SIZE=$2
fi

# Check if executable exists, otherwise build it
if test ! -x $BUILD_DIR/$SATF ; then
    echo "Building \"$SATF\"..."
    mkdir $BUILD_DIR
    cd $BUILD_DIR
    $QMAKE ../$PROJECT_FILE
    make
    cd ..
fi
if test ! -x $BUILD_DIR/$SATF ; then
    echo "Unable to build \"$SATF\"!"; exit 1
fi

# Run executable to generate performance measurement data
echo "Running executable \"$SATF\" to generate measurement data..."
build/${SATF} ${MIN_SIZE} ${MAX_SIZE}

# Sort contents of files in the last generated log dir numerically.
# Also retrieve the maximum computation time of all algorithms for
# use in gnuplot later on. This allows to use the same y-scale for
# all plots
LAST_LOG=`ls $LOG_DIR | tail -n1`
LAST_LOG_DIR=`echo $LOG_DIR/$LAST_LOG`
OVERALL_MAX_TIME=0
echo "Sorting log files in \"$LAST_LOG_DIR\" in arithmetical order..."
for FILE in `ls $LAST_LOG_DIR`; do
    sort -n $LAST_LOG_DIR/$FILE -o $LAST_LOG_DIR/$FILE
    # Reasonably assume that the computation time of the last entry
    # is always the maximum. This is right as long as computation time
    # of an algorithm increases with the size of the processed data :-)
    MAX_TIME=`tail -n1 $LAST_LOG_DIR/$FILE | awk '{print $2}'`
    if [ $MAX_TIME -gt $OVERALL_MAX_TIME ]; then
        OVERALL_MAX_TIME=$MAX_TIME
    fi
done
if [ $OVERALL_MAX_TIME -eq 0 ]; then
    OVERALL_MAX_TIME=1 # Make gnuplot happy in corner-cases
fi
echo "Overall max computation time was $OVERALL_MAX_TIME millisecond"

# Generate function plots for each log file in LAST_LOG_DIR and
# output into distinct files
LAST_PLOT_DIR=`echo $PLOT_DIR/$LAST_LOG`
mkdir -p $LAST_PLOT_DIR
echo "Generating plots in \"$LAST_PLOT_DIR\"..."
for FILE in `ls $LAST_LOG_DIR`; do
    `gnuplot << EOF
        set xlabel "data size [number of elements]"
        set xrange [$MIN_SIZE:$MAX_SIZE]
        set ylabel "used time [milliseconds]"
        set yrange [0:$OVERALL_MAX_TIME]
        set grid
        set terminal png
        set output '$LAST_PLOT_DIR/$FILE.png'
        plot '$LAST_LOG_DIR/$FILE' using 1:2 title "$LAST_LOG_DIR/$FILE" with lines
        quit
        EOF`
done
