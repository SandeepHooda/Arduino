from gpiozero import Button
import time
import datetime
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
touchCount = 0;
firstTouchTime = 0;
#subprocess.run(["espeak" , "clock"])
def downLoadWavFile(filePath):
    #smartLantern = requests.get("http://192.168.0.199/toggle", allow_redirects=True)
    r = requests.get(filePath, allow_redirects=True) 
    open('/home/pi/pythonwork/speakingClock/time.mp3', 'wb').write(r.content)
    subprocess.run(["omxplayer", "/home/pi/pythonwork/speakingClock/time.mp3"])
    time.sleep(3)
def speakTime():
    if os.path.exists('/home/pi/pythonwork/speakingClock/time.mp3'): # clear any old file
       os.remove("/home/pi/pythonwork/speakingClock/time.mp3")
    localtime = time.localtime()
    localtime ="Time is "+ time.strftime("%I:%M %p", localtime) 
    print(localtime)
    #subprocess.run(["espeak" , localtime])
    try:
       downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q="+localtime+"&tl=en") 
    except (requests.ConnectionError, requests.Timeout) as exception: 
       subprocess.run(["espeak" , localtime])
    
def speakBillOutStanding():
   if os.path.exists('/home/pi/pythonwork/speakingClock/outstanding.mp3'):
       os.remove('/home/pi/pythonwork/speakingClock/outstanding.mp3')
   r = requests.get("https://utility-bills.herokuapp.com/CheckUtilityBills_Pi", allow_redirects=True)
   outstandingAmt = r.content.decode("utf-8") 
   print(outstandingAmt)
   try:
       downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q="+outstandingAmt+"&tl=en")
   except (requests.ConnectionError, requests.Timeout) as exception:
       subprocess.run(["espeak" , outstandingAmt])

while True:

    if pad.value and not already_pressed:
        speakTime();
        touchCount += 1;
        print ("touch count ", touchCount)
        if ( time.time() - firstTouchTime ) > 30 : # time out so reset the first touch time to now
           touchCount = 1;
           firstTouchTime = time.time()

        if touchCount >= 2 : #touched three times in 15 seconds. This is command for shutdown 
            #downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q=uhhh, You touched me five  times in 30 seconds. It is annoying. I am going away. You better behave.&tl=en");
            #time.sleep(2)
            #os.system("sudo shutdown -h now")
            hour = datetime.datetime.now().hour
            if hour > 5 and hour < 21:
                speakBillOutStanding()
            
        
        
        

    already_pressed = pad.value
    time.sleep(0.1)
