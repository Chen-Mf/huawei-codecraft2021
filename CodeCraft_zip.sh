#!/bin/bash

SCRIPT=$(readlink -f "$0")
BASEDIR=$(dirname "$SCRIPT")
cd $BASEDIR

if [ ! -d CodeCraft-2021 ]
then
    echo "ERROR: $BASEDIR is not a valid directory of SDK_python for CodeCraft-2021."
    echo "  Please run this script in a regular directory of SDK_python."
    exit -1
fi

rm -f CodeCraft-2021.zip
zip -r CodeCraft-2021.zip *