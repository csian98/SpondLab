#!/bin/bash
# CSIAN

if [ $# != 1 ]
then
    echo "./plot_data.sh <./FILE NAME>"
    exit
fi

if [ -e $1 ]
then
    Rscript ./.codes/plot_XPS.R $1
else
    echo "There is no " $1
fi



