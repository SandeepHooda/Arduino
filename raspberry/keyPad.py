# This example is a hello world example
# for using a keypad with the Raspberry Pi

import RPi.GPIO as GPIO
import time
import sys
import time
import datetime
import requests
import urllib.request
import json
import os
import subprocess
import os.path


L1 = 19
L2 = 13
L3 =  6
L4 =  5

C1 =  21
C2 =  20 
C3 =  16
C4 =  12

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)

GPIO.setup(L1, GPIO.OUT)
GPIO.setup(L2, GPIO.OUT)
GPIO.setup(L3, GPIO.OUT)
GPIO.setup(L4, GPIO.OUT)

GPIO.setup(C1, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(C2, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(C3, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(C4, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

def downLoadWavFile(filePath):
    #smartLantern = requests.get("http://192.168.0.199/toggle", allow_redirects=True)
    r = requests.get(filePath, allow_redirects=True) 
    open('/home/pi/pythonwork/keypad/voice.mp3', 'wb').write(r.content)
    subprocess.run(["omxplayer", "/home/pi/pythonwork/keypad/voice.mp3"])
    time.sleep(3)
def speakTime():
    if os.path.exists('/home/pi/pythonwork/keypad/voice.mp3'): # clear any old file
       os.remove("/home/pi/pythonwork/keypad/voice.mp3")
    localtime = time.localtime()
    localtime ="Time is "+ time.strftime("%I:%M %p", localtime) 
    print(localtime)
    try:
       downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q="+localtime+"&tl=en") 
    except (requests.ConnectionError, requests.Timeout) as exception: 
       subprocess.run(["espeak" , localtime])

def isNight():
    now = datetime.datetime.now()
    if now.hour >= 22 and now.hour < 6 :
        return True;
    return False;
    
def startWork(charPressed):
    if (charPressed == '#' or isNight()):
        speakTime();
   
        
        

def readLine(line, characters):
    GPIO.output(line, GPIO.HIGH)
    charPressed = '';
    if(GPIO.input(C1) == 1):
        charPressed = characters[0];
    if(GPIO.input(C2) == 1):
        charPressed = characters[1];
    if(GPIO.input(C3) == 1):
        charPressed = characters[2];
    if(GPIO.input(C4) == 1):
        charPressed = characters[3];
    if (charPressed != ''):
        print(charPressed)
        startWork(charPressed)
        time.sleep(.5)
    
    GPIO.output(line, GPIO.LOW)

try:
    while True:
        readLine(L1, ["1","2","3","A"])
        readLine(L2, ["4","5","6","B"])
        readLine(L3, ["7","8","9","C"])
        readLine(L4, ["*","0","#","D"])
        time.sleep(0.1)
except KeyboardInterrupt:
    print("\nApplication stopped!")

    