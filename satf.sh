#!/bin/bash
#
#   Copyright (c) 2009 Sascha Peilicke <sasch.pe@gmx.de>
#   This application is free software; you can redistribute it and/or modify it
#   under the terms of the GNU Library General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or (at your
#   option) any later version.

#   This application is distributed in the hope that it will be useful, but WITHOUT
#   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
#   License for more details.

#   You should have received a copy of the GNU Library General Public License
#   along with this application; see the file COPYING.LIB.  If not, write to the
#   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
#   02110-1301, USA.
#

MIN_SIZE=1
MAX_SIZE=1000
CMAKE=`which cmake 2>/dev/null`
GNUPLOT=`which gnuplot 2>/dev/null`
BINARY=satf
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

# Build executable if not available
if test ! -x $BUILD_DIR/$BINARY ; then
    rm -rf $BUILD_DIR 2>/dev/null
    echo "Building \"$BINARY\"..."
    mkdir $BUILD_DIR
    cd $BUILD_DIR
    $CMAKE ..
    make
    cd ..
    if test ! -x $BUILD_DIR/$BINARY ; then
        echo "Unable to build \"$BINARY\"!"; exit 1
    fi
fi 

# Run executable to generate performance measurement data
echo "Running executable \"$BINARY\" to generate measurement data..."
build/${BINARY} --min=${MIN_SIZE} --max=${MAX_SIZE}

# Sort contents of files in the last generated log dir numerically.
# Also retrieve the maximum computation time of all algorithms for
# use in gnuplot later on. This allows to use the same y-scale for
# all plots
LAST_LOG=`ls $LOG_DIR | tail -n1`
LAST_LOG_DIR=`echo $LOG_DIR/$LAST_LOG`
OVERALL_MAX_TIME=0
echo "Sorting log files in \"$LAST_LOG_DIR\" in arithmetical order..."
for DIR in `ls $LAST_LOG_DIR`; do
    for FILE in `ls $LAST_LOG_DIR/$DIR`; do
        sort -n $LAST_LOG_DIR/$DIR/$FILE -o $LAST_LOG_DIR/$DIR/$FILE
        # Reasonably assume that the computation time of the last entry
        # is always the maximum. This is right as long as computation time
        # of an algorithm increases with the size of the processed data :-)
        MAX_TIME=`tail -n1 $LAST_LOG_DIR/$DIR/$FILE | awk '{print $2}'`
        if [ $MAX_TIME -gt $OVERALL_MAX_TIME ]; then
            OVERALL_MAX_TIME=$MAX_TIME
        fi
    done
done
if [ $OVERALL_MAX_TIME -eq 0 ]; then
    OVERALL_MAX_TIME=1 # Make gnuplot happy in corner-cases
fi
echo "Overall max computation time was $OVERALL_MAX_TIME millisecond"

# Generate function plots for each log file in LAST_LOG_DIR and
# output into distinct files
LAST_PLOT_DIR=`echo $PLOT_DIR/$LAST_LOG`
UNAME=`uname -a`
echo "Generating plots in \"$LAST_PLOT_DIR\"..."
ALL_IN_ONE_PLOT_LINE=""
for DIR in `ls $LAST_LOG_DIR`; do
    mkdir -p $LAST_PLOT_DIR/$DIR
    for FILE in `ls $LAST_LOG_DIR/$DIR`; do
        `gnuplot << EOF
            set xlabel "data size [number of elements]"
            set xrange [$MIN_SIZE:$MAX_SIZE]
            set ylabel "used time [milliseconds]"
            set yrange [0:$OVERALL_MAX_TIME]
            set grid
            set pointsize 0.2
            set key top left
            set terminal png font "arial" 8
            set title "algorithm: $FILE $DIR\n$UNAME"
            set output '$LAST_PLOT_DIR/$DIR/$FILE.png'
            plot '$LAST_LOG_DIR/$DIR/$FILE' using 1:2 title "$FILE"
            quit
            EOF`
        # Build up the input for gnuplot 'plot' command to use for the combined plot
        ALL_IN_ONE_PLOT_LINE+=`echo -e "'$LAST_LOG_DIR/$DIR/$FILE' using 1:2 title \"$FILE\", "`
    done

    # Finally generate a function plot with all algorithms combined and remove the trailing ', '
    ALL_IN_ONE_PLOT_LINE=`echo ${ALL_IN_ONE_PLOT_LINE/%, /}`
    `gnuplot << EOF
        set xlabel "data size [number of elements]"
        set xrange [$MIN_SIZE:$MAX_SIZE]
        set ylabel "used time [milliseconds]"
        set yrange [0:$OVERALL_MAX_TIME]
        set grid
        set pointsize 0.2
        set key top left
        set terminal png font "arial" 8
        set title "all algorithms: $DIR\n$UNAME"
        set output '$LAST_PLOT_DIR/$DIR/all.png'
        plot $ALL_IN_ONE_PLOT_LINE
        quit
        EOF`
done
