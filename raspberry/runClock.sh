#!/bin/bash
# This script will run the speakingClock.py as a background task
# You will then be able close the terminal session.  To auto start
# Add the following command to the /etc/rc.local
# /home/pi/speaking_clock/runClock.sh start

progpath="/home/pi/pythonwork/speakingClock"
progname="speakingClock.py"
pythonPath="python"



echo "$0 ver 1.1 written by Sandeep Hooda"
echo $(date)
echo "-----------------------------------------------"
cd $progpath

# Check if progname exists
if [ ! -e $progpath/$progname ] ; then
  echo "ERROR   - Could Not Find $progpath/$progname"
  exit 1
fi

if [ -z "$( pgrep -f  $progpath/$progname )" ]; then
echo "speakingClock not running"
  if [ "$1" = "start" ]; then
     echo "START   - Start $progpath/$progname in Background mode..."
     /usr/bin/python3 $progpath/$progname > /home/pi/pythonwork/speakingClock/logs.txt 2>&1 &
  fi
else
echo "speakingClock is running "
  if [ "$1" = "stop" ]; then
    echo "STATUS  - Stopping $progpath/$progname ...."
    progPID=$( pgrep -f $progpath/$progname )
    sudo kill $progPID
  fi
fi

if [ -z "$( pgrep -f $progpath/$progname )" ]; then
    echo "STATUS  - $progpath/$progname is Not Running ..."
    echo "INFO    - To Start $progpath/$progname execute command below"
    echo "INFO    - $0 start"
else
    progPID=$(pgrep -f  $progpath/$progname)
    echo "STATUS  - $progpath/$progname is Running ..."
    echo "STATUS  - PID is $progPID"
    echo "INFO    - To Stop $progpath/$progname execute command below"
    echo "INFO    - $0 stop"
fi
echo "Done"

