import os
import datetime
import requests
import urllib.request
import subprocess
import time
def downLoadWavFile(filePath):
    #smartLantern = requests.get("http://192.168.0.199/toggle", allow_redirects=True)
    r = requests.get(filePath, allow_redirects=True) 
    open('/home/pi/pythonwork/keypad/alarm.mp3', 'wb').write(r.content)
    subprocess.run(["omxplayer", "/home/pi/pythonwork/keypad/alarm.mp3"])
    
filePath = "/home/pi/pythonwork/keypad/alarm.txt";
continueAlarm = True;
while (continueAlarm):
    if os.path.exists(filePath):
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
                subprocess.run(["omxplayer", "/home/pi/pythonwork/keypad/alarm.mp3"])
                #downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q="+sayIt+"&tl=en")
                
            else:
                print("no Ring alarm")
                continueAlarm = False
    else:
        continueAlarm = False;
