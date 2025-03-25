#!/bin/bash

USER="spondlab"
HOME="/home/"$USER

while [ 1 ]; do
  echo -e "\n\tHello Welcome to installing XPSDNN\nBefore U need to correct USER name in 'aptInstall.sh' script\nCorrect Already? [y/n]: "
  read -sn1 ask
  if [ $ask = 'y' ]; then
    break
  else if [ $ask = 'n' ]; then
    echo -e "'aptInstall.sh line number: 3'"
    sleep 3
    exit
  fi
done

if [ $EUID -ne 0 ]; then
  echo -e "\n\t\taptInstall require executed with 'sudo'"
  echo -e "\t\tsudo ./aptInstall.sh\n"
  exit
fi

# install Path
installPath=$HOME"/install/"`dirname $0`


if [ -d $HOME/XPSDNN/ ]; then
  echo -e "\n\t\tAlready XPSDNN directory exist\n"
  sleep 1
else
  mkdir $HOME/XPSDNN
fi

if [ -f $installPath/makeDir.sh ]; then
  echo -e "\n\t\tNow build directory structure...\n"
  cp -p $installPath/makeDir.sh $HOME/XPSDNN/
  cd $HOME/XPSDNN/
  ./makeDir.sh $USER
  rm ./makeDir.sh
else
  echo -e "\n\t\tThere are no makeDir.sh shell script\n"
  sleep 3
  exit
fi

if [ -e $installPath/code_DNNXPS.tar ]; then
  echo -e "\n\t\tNow moving codes...\n"
  cp -p $installPath/code_DNNXPS.tar $HOME/XPSDNN/
  cd $HOME/XPSDNN/
  tar -xvf ./code_DNNXPS.tar 2>&1 1>/dev/null
  rm ./code_DNNXPS.tar
else
  echo -e "\n\t\tThere are no code_DNNXPS.tar archive file\n"
  sleep 3
  exit
fi

if [ -e $installPath/valid_data.tar ]; then
  echo -e "\n\t\tNow moving validation data...\n"
  cp -p $installPath/valid_data.tar $HOME/XPSDNN/
  cd $HOME/XPSDNN/
  tar -xvf ./valid_data.tar 2>&1 1>/dev/null
  rm ./valid_data.tar
else
  echo -e "\n\t\tThere are no valid_data.tar archive file\n"
  sleep3
  exit
fi

if [ -f $installPath/Sessa-2.2.0-Linux.sh ]; then
  echo -e "\n\t\tNow Started to load SESSA\n"
  cp -p $installPath/Sessa-2.2.0-Linux.sh $HOME/XPSDNN/.SESSA
  cd $HOME/XPSDNN/.SESSA/
  sleep 2
  ./Sessa-2.2.0-Linux.sh

  echo -e "\n\t\tSESSA installed complete"
  chown -R $USER ./
  chgrp -R $USER ./
  cd $HOME/XPSDNN/.SESSA/
  rm Sessa-2.2.0-Linux.sh
  
  cd $installPath
else
  echo -e "\n\t\tThere are no Sessa-2.2.0-Linux.sh shell script\n"
  sleep 3
  exit
fi

echo -e "\n\t\tNow started to download compile utilities and python3\n"
apt-get install gcc g++ make python3-pip dialog xvfb
echo -e "\n\t\tDownload Completed\n"

echo -e "\n\t\tNow Compile XPSDNN codes..."
cd $HOME/XPSDNN/.codes
echo -e "\n\t\tCompiling...\n"
make
make clean
echo -e "\n\t\tCompile Completed\n"

echo -e "\n\t\tNow Started to download libgfortran.so.3"

echo "Downloading gcc-6-base" && \
  cd /tmp/ && wget http://archive.ubuntu.com/ubuntu/pool/universe/g/gcc-6/gcc-6-base_6.4.0-17ubuntu1_amd64.deb && \
  echo "Downloading libgfortran3" && \ 
  cd /tmp/ && wget http://archive.ubuntu.com/ubuntu/pool/universe/g/gcc-6/libgfortran3_6.4.0-17ubuntu1_amd64.deb && \
  echo "Installing gcc-6-base" && \
  cd /tmp/ && sudo dpkg -i gcc-6-base_6.4.0-17ubuntu1_amd64.deb && \ 
  echo "Installing libgfortran3" && \
  cd /tmp/ && sudo dpkg -i libgfortran3_6.4.0-17ubuntu1_amd64.deb

echo -e "\n\t\t"

echo -e "\n\t\tNow collecting above pyLibraries\n"

REQUIRE=$installPath/requirements.txt
cat $REQUIRE
echo -e "\n\t\tEND\n"
sleep 2

if [ -f $REQUIRE ]; then
  pip3 install -r requirements.txt 2>&1 1>/dev/null
fi

echo -e "\n\t\tAll of the Requirements installed and ready\n"
sleep 2

echo -e "\n\t\tCopy other needed shell scripts...\n"
cd $installPath
cp xpsControl $HOME/XPSDNN/
cp config $HOME/XPSDNN/
cp README $HOME/XPSDNN/

echo "All installing finished"
sleep 2

read -n1 -p "Do you want to remove installing files [y/n]? " answer
case $answer in
T | y)
  echo
  echo "removing all files..."
  rm -rf $installPath;;
F | f)
  echo
  exit;;
esac

# clear
