#!/bin/bash

possibleName=($SHELL "-"`basename $SHELL`)
if [ $0 == ${possibleName[0]} ] || [ $0 == ${possibleName[1]} ]; then
  echo -e "\t[USAGE]"
  echo -e "\t. ./checkStatus.sh"
  exit
fi

echo -e "\t+1) CHECK '$HOME/sessa-$USER'"
if [ -e "$HOME/sessa-$USER" ]; then
  echo -e "\t\tNeed to delete 'sessa-$USER' file"
  echo -en "\t\tDELETE sessa-$USER [y/n]: "
  read -sn1 ask
  echo
  if [ $ask = 'y' ]; then
    rm $HOME/sessa-$USER
  else
    echo -e "\tEXIT checkStatus"
    exit
  fi
fi
if [ -e "$HOME/SESSA v2-$USER" ]; then
  echo -e "\t\tNeed to delete 'SESSA v2-$USER' file"
  echo -en "\t\tDELETE SESSA v2-$USER [y/n]: "
  read -sn1 ask
  echo
  if [ $ask = 'y' ]; then
    rm $HOME/SESSA\ v2-$USER
  else
    echo -e "\tEXIT checkStatus"
    exit
  fi
fi
echo -e "\t-1) CHECK '$HOME/sessa-$USER Completed\n"

echo -e "\t+2) CHECK 'Xvfb DISPLAY' Setting"
lineNum=`ps -ef | grep 'Xvfb :99' | wc -l`
if [ $lineNum -eq 2 ]; then
  echo -e "\t\tDetect Xvfb with 99..."
  echo -e "\t\tset DISPLAY"
  export DISPLAY=:99
else
  echo -e "\t\tXvfb with 99 was not detected..."
  echo -e "\t\tBuild Xvfb & set DISPLAY"
  Xvfb :99 -screen 0 1024x768x24 &
  export DISPLAY=:99
fi
echo -e "\t\tYou should declare\n"
echo -e "\t\t\texport DISPLAY=:99\n\n"
echo -e "\t-2) CHECK 'Xvfb DISPLAY' Complted\n"

echo -e "\t+3) CHECK 'SharedMemory' Allocated"
ipcs -m | gawk '/0x00000cca/{print $2} /0x00000ccb/{print $2}' > .tmp/dump2/ipcs.txt
cat .tmp/dump2/ipcs.txt | while read ipcNum; do
  echo -e "\t\tremove $ipcNum shared Memory"
  ipcrm -m $ipcNum
done
echo -e "\t-3) CHECK 'SharedMemory' Completed\n"
rm .tmp/dump2/ipcs.txt

echo -en "\t+4) Clear the ex log files? [y/n]: "
read -sn1 ask
echo
if [ $ask = 'y' ]; then
  cat /dev/null > .tmp/log/log.txt
  cat /dev/null > .tmp/log/err.txt
  cat /dev/null > .tmp/log/pyLog.txt
  echo -e "\t\tclear all log files"
  
  rm -f .tmp/dump/* .tmp/dump2/*
  echo -e "\t\tclear all dump files"
fi
echo -e "\t-4) Clear the ex log files Completed\n"

echo -en "\t+5) Clear the ex Data files? [y/n]: "
read -sn1 ask
echo
if [ $ask = 'y' ]; then
  rm -rf ./data/*
  echo -e "\t\tremove all data files"
fi
echo -e "\t-5) Clear the ex Data files Completed\n"

echo -en "\t+6) Clear the ex losses files? [y/n]: "
read -sn1 ask
echo
if [ $ask = 'y' ]; then
  rm -f ./losses/*
  echo -e "\t\tremove losses file from PY"
fi
echo -e "\t-6) Clear the ex losses files Completed\n"

echo -e "\tNow ready for XPSDNN .main"
