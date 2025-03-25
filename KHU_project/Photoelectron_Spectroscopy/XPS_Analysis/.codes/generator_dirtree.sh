#!/bin/bash
# CSIAN
cd ..
ROOT_PATH=$(pwd)"/Generator"
mkdir Generator
cd $ROOT_PATH
mkdir data_set XPS_Data_Plot .codes .tmp_data tmp_test relative

cd XPS_Data_Plot
mkdir Plot_Data .codes
cd $ROOT_PATH

cd .codes
mkdir c_files .SESSA
cd $ROOT_PATH

cd .tmp_data
mkdir raw_data tmp
cd tmp
cd $ROOT_PATH

cd tmp_test
mkdir spc graph tmp .codes
