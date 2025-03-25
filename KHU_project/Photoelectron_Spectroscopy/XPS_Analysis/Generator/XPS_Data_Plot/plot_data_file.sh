#!/bin/bash
#CSIAN

if [ $# != 2 ]
then
        echo "./plot_data_file.sh <MKDIR NAME> <DATA FOLDER>"
        exit
fi

mkdir ./Plot_Data/$1

for file in $(ls $2)
do
        echo $file
        Rscript ./.codes/plot_XPS_file.R $2 $file $1
done
