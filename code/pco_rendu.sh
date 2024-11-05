#!/bin/bash

SCRIPT=${BASH_SOURCE[0]}
SCRIPTPATH=$(realpath $(dirname $SCRIPT))
PROJECT_DIR=project_student
PROJECT_FILE=QtrainSimStudent.pro
PROG1_DIR=${PROJECT_DIR}/prog1
PROG2_DIR=${PROJECT_DIR}/prog2
REPORT_FILE=rendu.pdf
ARCHIVE=rendu.tar.gz

cd $SCRIPTPATH

if [ ! -f "${PROJECT_DIR}/${PROJECT_FILE}" ]
then
    echo "Could not find project file : $PROJECT_FILE in $(realpath $PROJECT_DIR)" >&2
    exit 1
fi

if [ ! -f "$REPORT_FILE" ]
then
    echo "Could not find $REPORT_FILE directory in $SCRIPTPATH" >&2
    exit 1
fi

echo "The following files are archived in $ARCHIVE : "
tar --exclude="$ARCHIVE" --exclude='*.o' --exclude='*.user' -czvf $ARCHIVE ${PROJECT_DIR}/${PROJECT_FILE} $PROG1_DIR $PROG2_DIR $REPORT_FILE
