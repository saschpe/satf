#!/bin/sh
#
# This script builds and runs the algorithm test binary
# and generates function plots from the resulting log data.
#
#   Copyright (c) 2009 Sascha Peilicke <sasch.pe@gmx.de>
#
#   Permission is hereby granted, free of charge, to any person
#   obtaining a copy of this software and associated documentation
#   files (the "Software"), to deal in the Software without
#   restriction, including without limitation the rights to use,
#   copy, modify, merge, publish, distribute, sublicense, and/or sell
#   copies of the Software, and to permit persons to whom the
#   Software is furnished to do so, subject to the following
#   conditions:
#
#   The above copyright notice and this permission notice shall be
#   included in all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
#   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
#   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
#   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
#   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
#   OTHER DEALINGS IN THE SOFTWARE.
#

MIN_SIZE=1
MAX_SIZE=1000
QMAKE=`which qmake 2>/dev/null`
GNUPLOT=`which gnuplot 2>/dev/null`
BINARY=satf
PROJECT=satf.pro
BUILD_DIR=build
LOG_DIR=logs
PLOT_DIR=plots

# Check for optional commandline arguments
if [ $# -eq 1 ] && [ $1 = "clean" ]; then
    rm -rf $BUILD_DIR $LOG_DIR $PLOT_DIR
    exit 0
fi
if [ $# -eq 2 ]; then
    MIN_SIZE=$1; MAX_SIZE=$2
fi

# Rebuild executable
rm -rf $BUILD_DIR 2>/dev/null
echo "Building \"$BINARY\"..."
mkdir $BUILD_DIR
cd $BUILD_DIR
$QMAKE ../$PROJECT
make
cd ..
if test ! -x $BUILD_DIR/$BINARY ; then
    echo "Unable to build \"$BINARY\"!"; exit 1
fi

# Run executable to generate performance measurement data
echo "Running executable \"$BINARY\" to generate measurement data..."
build/${BINARY} ${MIN_SIZE} ${MAX_SIZE}

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
UNAME=`uname -a`
mkdir -p $LAST_PLOT_DIR
echo "Generating plots in \"$LAST_PLOT_DIR\"..."
ALL_IN_ONE_PLOT_LINE=""
for FILE in `ls $LAST_LOG_DIR`; do
    `gnuplot << EOF
        set xlabel "data size [number of elements]"
        set xrange [$MIN_SIZE:$MAX_SIZE]
        set ylabel "used time [milliseconds]"
        set yrange [0:$OVERALL_MAX_TIME]
        set grid
        set terminal png
        set title "Algorithm: $FILE\n$UNAME"
        set output '$LAST_PLOT_DIR/$FILE.png'
        plot '$LAST_LOG_DIR/$FILE' using 1:2 title "$LAST_LOG_DIR/$FILE" with lines
        quit
        EOF`
    # Build up the input for gnuplot 'plot' command to use for the combined plot
    ALL_IN_ONE_PLOT_LINE+=`echo -e "'$LAST_LOG_DIR/$FILE' using 1:2 title \"$LAST_LOG_DIR/$FILE\" with lines, "`
done

# Finally generate a function plot with all algorithms combined and remove the trailing ', '
ALL_IN_ONE_PLOT_LINE=`echo ${ALL_IN_ONE_PLOT_LINE/%, /}`
`gnuplot << EOF
    set xlabel "data size [number of elements]"
    set xrange [$MIN_SIZE:$MAX_SIZE]
    set ylabel "used time [milliseconds]"
    set yrange [0:$OVERALL_MAX_TIME]
    set grid
    set terminal png
    set title "All algorithms\n$UNAME"
    set output '$LAST_PLOT_DIR/all.png'
    plot $ALL_IN_ONE_PLOT_LINE
    quit
    EOF`
