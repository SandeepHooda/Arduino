#!/bin/bash
# This script will run the keyPad.py as a background task
# You will then be able close the terminal session.  To auto start
# Add the following command to the /etc/rc.local
# /home/pi/pythonwork/keypad/keyPad.sh start

#flask run --host=0.0.0.0 2>&1 &

export FLASK_APP=/home/pi/pythonwork/flask/piAppSrv.py


progname="/usr/bin/flask"



echo "$0 ver 1.1 written by Sandeep Hooda"
echo $(date)
echo "-----------------------------------------------"


# Check if progname exists
if [ ! -e $progname ] ; then
  echo "ERROR   - Could Not Find $progname"
  exit 1
fi

if [ -z "$( pgrep -f  $progname )" ]; then
echo "flask not running"
  if [ "$1" = "start" ]; then
     echo "START   - Start $progname in Background mode..."
     flask run --host=0.0.0.0 2>&1 &
  fi
else
echo "flask is running "
  if [ "$1" = "stop" ]; then
    echo "STATUS  - Stopping $progname ...."
    progPID=$( pgrep -f $progname )
    sudo kill $progPID
  fi
fi

if [ -z "$( pgrep -f $progname )" ]; then
    echo "STATUS  - $progname is Not Running ..."
    echo "INFO    - To Start $progname execute command below"
    echo "INFO    - $0 start"
else
    progPID=$(pgrep -f  $progname)
    echo "STATUS  - $progname is Running ..."
    echo "STATUS  - PID is $progPID"
    echo "INFO    - To Stop $progname execute command below"
    echo "INFO    - $0 stop"
fi
echo "Done"

