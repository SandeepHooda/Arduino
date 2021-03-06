import os
import datetime
import requests
import urllib.request
import subprocess
import time
import RPi.GPIO as GPIO
from time import sleep

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
#Set buzzer - pin 23 as output
buzzer=23 
GPIO.setup(buzzer,GPIO.OUT)

def beep(beepTime):
    GPIO.output(buzzer,GPIO.HIGH)
    print ("Beep")
    sleep(beepTime) # Delay in seconds
    GPIO.output(buzzer,GPIO.LOW)
def downLoadWavFile(filePath):
    #smartLantern = requests.get("http://192.168.0.199/toggle", allow_redirects=True)
    r = requests.get(filePath, allow_redirects=True) 
    open('/home/pi/pythonwork/keypad/alarm.mp3', 'wb').write(r.content)
    subprocess.run(["omxplayer", "-o", "local", "--loop", "/home/pi/pythonwork/keypad/alarm.mp3"])
    
filePath = "/home/pi/pythonwork/keypad/alarm.txt";
filePathTimer = "/home/pi/pythonwork/keypad/timer.txt";
filePathChoco = "/home/pi/pythonwork/keypad/ChocoNextSusu.txt";


if os.path.exists(filePathChoco):
    #Remider check for choco susu
    f = open(filePathChoco, "r")
    alarmTimeChoco = f.read()
    now = datetime.datetime.now()
    timeNow = str(now.hour)+str(now.minute)
    print(timeNow, alarmTimeChoco)
    if (timeNow == alarmTimeChoco):
        beep(2)
        subprocess.run(["omxplayer", "/home/pi/pythonwork/keypad/ChocoWalk.mp3"])
        subprocess.run(["omxplayer", "/home/pi/pythonwork/keypad/ChocoWalk.mp3"])
        os.remove(filePathChoco)
        
if os.path.exists(filePathTimer):
    print("timer")
    #Timer go off
    f = open(filePathTimer, "r")
    alarmTimeTimer = f.read()
    now = datetime.datetime.now()
    timeNow = now.strftime( '%H%M')
    print(timeNow, alarmTimeTimer)
    if (timeNow == alarmTimeTimer):
        beep(2)
        os.remove(filePathTimer)
        subprocess.run(["omxplayer", "-o", "local", "--loop", "/home/pi/pythonwork/keypad/timer.mp3"])
        

#downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q=You have a scheduled timer at this time. Press. D. to stop the alarm.&tl=en")
#Generic alarm check
f = open(filePath, "r")
alarmHour =0;
alarmMinute =0;
alarmTime = f.read()
if (len(alarmTime) > 0):
    if (len(alarmTime) <= 2):
        alarmHour = int(alarmTime)
    else:
        start = len(alarmTime) -2
        alarmMinute = int(alarmTime[start:])
        alarmHour = int(alarmTime[:start])

if (alarmHour > 0):
    now = datetime.datetime.now()
    print("Alarm will go off at" , alarmHour, alarmMinute)
    if now.hour == alarmHour and now.minute == alarmMinute :
        print("ring Alarm")
        sayIt = "Your Attention please, You have a scheduled reminder at this time. Press. D. to stop the alarm."
        reminderFile = '/home/pi/pythonwork/keypad/alarmMessage.txt'
        reminderText ="";
        if os.path.exists(reminderFile):
            f = open(reminderFile, "r")
            reminderText = f.read()
        if (reminderText != ''):
            downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q="+reminderText+". Press. D. to stop the alarm.&tl=en")
        else:
            subprocess.run(["omxplayer", "-o", "local", "--loop", "/home/pi/pythonwork/keypad/alarm.mp3"])
                
                
    else:
        print("no Ring alarm")
                
    
