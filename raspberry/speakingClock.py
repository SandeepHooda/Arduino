from gpiozero import Button
import time
import requests
import urllib.request
import json
import os
import subprocess
import os.path
import board
from digitalio import DigitalInOut, Direction

pad_pin = board.D23

pad = DigitalInOut(pad_pin)
pad.direction = Direction.INPUT

already_pressed = False
print("time speak clock")
#subprocess.run(["espeak" , "clock"])
def downLoadWavFile(filePath):
    r = requests.get(filePath, allow_redirects=True)
    open('/home/pi/speaking_clock/time.mp3', 'wb').write(r.content)
    subprocess.run(["omxplayer", "/home/pi/speaking_clock/time.mp3"])
    time.sleep(5)
    #os.remove("/home/pi/speaking_clock/time.mp3")
def speakTime():
    if os.path.exists('/home/pi/speaking_clock/time.mp3'): # clear any old file
       os.remove("/home/pi/speaking_clock/time.mp3")
    localtime = time.localtime()
    localtime = time.strftime("%I:%M %p", localtime)
    print(localtime)
    #subprocess.run(["espeak" , localtime])
    downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q="+localtime+"&tl=en");

while True:

    if pad.value and not already_pressed:
        speakTime();

    already_pressed = pad.value
    time.sleep(0.1)
