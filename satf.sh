#!/bin/sh
# This script generates and runs the algorithm test binary
# and generates function plots from the resulting log data.

MIN_SIZE=1
MAX_SIZE=1000
QMAKE=`which qmake 2>/dev/null`
GNUPLOT=`which gnuplot 2>/dev/null`
SATF=./satf
LOG_DIR=logs/

# Check for optional commandline arguments
if [ $# -eq 2 ] ; then
    MIN_SIZE=$1
    MAX_SIZE=$2
fi

# Check if executable exists, otherwise build it
if test ! -x $SATF ; then
    echo "Building '$SATF'..."; $QMAKE && make
fi
if test ! -x $SATF ; then
    echo "Unable to build '$SATF'!"; exit 1
fi

# Run executable to generate performance measurement data
echo "Running '$SATF'..."
${SATF} ${MIN_SIZE} ${MAX_SIZE}

# Sort contents of files in the last generated log dir numerically
LAST_LOG=`ls $LOG_DIR | tail -n1`
LAST_LOG_DIR=`echo $LOG_DIR$LAST_LOG`
echo "Sort log files in '$LAST_LOG_DIR' in arithmetical order..."
for FILE in `ls $LAST_LOG_DIR`; do
    sort -n $LAST_LOG_DIR/$FILE > $LAST_LOG_DIR/$FILE
done

# Generate function plots
if [ -d $LOGDIR ] && [ -x $GNUPLOT ]; then
    echo "Processing performance measurement data..."

fi
