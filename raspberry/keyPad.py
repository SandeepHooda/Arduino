# This example is a hello world example
# for using a keypad with the Raspberry Pi
import speech_recognition as sr
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
from time import sleep
import calendar
from datetime import date
from urllib.request import urlopen
from bs4 import BeautifulSoup
import re
from num2words import num2words
sys.path.insert(1, "/home/pi/pythonwork/keypad/weather")
import weather
sys.path.insert(2, "/home/pi/pythonwork/keypad/news")
import news


#from datetime import datetime

alarmFilePath = "/home/pi/pythonwork/keypad/alarm.txt";
insideAlarmMode = False;
insideTimerMode = False;
alarmTime = "";
alarmHour =0;
alarmMinute =0;
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

#Set buzzer - pin 23 as output
buzzer=23 
GPIO.setup(buzzer,GPIO.OUT)

def beep():
    GPIO.output(buzzer,GPIO.HIGH)
    print ("Beep")
    sleep(0.01) # Delay in seconds
    GPIO.output(buzzer,GPIO.LOW)
    

def downLoadWavFile(filePath):
    #smartLantern = requests.get("http://192.168.0.199/toggle", allow_redirects=True)
    r = requests.get(filePath, allow_redirects=True) 
    open('/home/pi/pythonwork/keypad/voice.mp3', 'wb').write(r.content)
    subprocess.run(["omxplayer", "/home/pi/pythonwork/keypad/voice.mp3"])
    beep();
#downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q=Happy to., Press. A. to set alarm. Press. B. to get outstanding bills. Press. Hash. to get current time. Press 1 . 2. or 3 for setting choco alarm. Press C. to turn on. light for 2 minutes. &tl=en")
#downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q=Press 0. for setting timer. Press 9. to know about school homework. Press 4,5,6 for music. &tl=en")
print("Start work")
def speakTime():
    if os.path.exists('/home/pi/pythonwork/keypad/voice.mp3'): # clear any old file
       os.remove("/home/pi/pythonwork/keypad/voice.mp3")
    localtime = time.localtime()
    if (isNight()):
        localtime ="Time is "+ time.strftime("%I:%M %p", localtime)+" ."
    else:
        day =calendar.day_name[date.today().weekday()]
        localtime ="Time is "+day+" . "+time.strftime("%d %B %I:%M %p", localtime)+" ."
        
        
    print(localtime)
    try:
       downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q="+localtime+"&tl=en")
       if (isNight() == False):
           speakRainForecast();
    except (requests.ConnectionError, requests.Timeout) as exception: 
       subprocess.run(["espeak" , localtime])
def speakOutStandingBill():
   #downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q=Happy to help,Please, wait while I get the balances.&tl=en")
   subprocess.run(["omxplayer", "/home/pi/pythonwork/keypad/wait.mp3"])
   r = requests.get("https://utility-bills.herokuapp.com/CheckUtilityBills_Pi", allow_redirects=True)
   outstandingAmt = r.content.decode("utf-8") 
   print(outstandingAmt)
   downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q="+outstandingAmt+"&tl=en")
  

def isNight():
    now = datetime.datetime.now()
    if  now.hour >= 23 or now.hour < 6 :
        return True;
    return False;

def setAlarm(charPressed):
    global insideAlarmMode
    global alarmTime
    global alarmMinute
    global alarmHour
    if (charPressed == '#'):#Exit alarm mode
        insideAlarmMode = False;
        if (len(alarmTime) > 0):
            if (len(alarmTime) <= 2):
                alarmHour = int(alarmTime)
            else:
                start = len(alarmTime) -2
                alarmMinute = int(alarmTime[start:])
                alarmHour = int(alarmTime[:start])
        print("alarm time " ,alarmHour,":",alarmMinute)
        f = open("/home/pi/pythonwork/keypad/alarm.txt", "w")
        f.write(alarmTime)
        f.close()
        am_pm = ". AM";
        if (alarmHour >=12 ):
            am_pm = ". PM"
            if (alarmHour >12 ):
                alarmHour = alarmHour -12;
        sayIt = "Sure, I have set the alarm for "+str(alarmHour)
        if (alarmMinute >0):
            sayIt += " : "+str(alarmMinute)
        sayIt += am_pm;
        print(sayIt)
        try:
            downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q="+sayIt+"&tl=en") 
        except (requests.ConnectionError, requests.Timeout) as exception: 
           subprocess.run(["espeak" , "Sure, I have set the alarm for "+sayIt])
    else:
        alarmTime += charPressed;

def setTimer(charPressed):
    global insideTimerMode
    global alarmTime
    global alarmMinute
    global alarmHour
    if (charPressed == '#'):#Exit timer mode
        insideTimerMode = False;
        now = datetime.datetime.now()
        now_plus = now + datetime.timedelta(minutes = int(alarmTime))
        f = open("/home/pi/pythonwork/keypad/timer.txt", "w")
        f.write( now_plus.strftime( '%H%M'))
        f.close()
        sayIt= "Sure, I have set timer for "+alarmTime+", minutes .";
        try:
            downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q="+sayIt+"&tl=en") 
        except (requests.ConnectionError, requests.Timeout) as exception: 
           subprocess.run(["espeak" , sayIt])
    else:
        alarmTime += charPressed;

def light():
    r = requests.get("http://192.168.0.199/smartMode/off", allow_redirects=True)
    r = requests.get("http://192.168.0.199/forceMode/on", allow_redirects=True)
    sleep(120)
    r = requests.get("http://192.168.0.199/forceMode/off", allow_redirects=True)
def addToDo(task):
    url = "https://idonotremember-app.appspot.com/AddToDo?task="+task
    requests.get(url,  allow_redirects=True)
def schoolHW():
        html = urlopen("https://drive.google.com/drive/folders/1rRcYP6BzN1o9vsULGwJbFCd5TbwiPDwj").read()
        soup = BeautifulSoup(html,"html.parser")
        todaysTitle = soup.title.string
        print(todaysTitle)
        todaysTitle = re.sub('[^0-9]',' ', todaysTitle)
        split = re.split(' ', todaysTitle)
        homework = "";
        for x in split:
            if (x != ''):
                print(num2words(x))
                homework = homework + " . "+num2words(x);
        downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q=Hi Shaurya , School homework is as of "+homework+"&tl=en")

def playMusic(charPressed):
    f = open("/home/pi/pythonwork/keypad/music/"+charPressed+".txt", "w")
    f.write(charPressed)
    f.close()
def speakRainForecast():
    rainForecast= weather.findRain();
    for obj in rainForecast:
        downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q=Weather forecast,  "+obj.date +" , " +obj.forecast + " , chances of rain,  "+ obj.percent +" % ."+"&tl=en")
def readNews():
    allNews = news.extractNews();
    for anews in allNews:
        downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q= "+anews +" ."+"&tl=en")
    
def speakHourlyForecast():
    hourlyData = weather.extractWeatherFieldsHour();
    if (len(hourlyData) > 0):
        forecastData = "";
        for obj in hourlyData:
            forecastData += "Time, "+obj.date ;
            if (int(obj.percent)  > 40):
                forecastData += ", Chance of rain are high. Chances are "+obj.percent+" percent. ";
            else:
                forecastData += ". It will be windy. Wind speed will be "+obj.wind+", km/h . ";
        downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q="+forecastData+".&tl=en")
        #print(forecastData);
    else:
        downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q=Today, it will be sunny and quite for next 12 hours.&tl=en")
    
def startWork(charPressed):
    if ((charPressed == '#' or isNight()) and charPressed != 'D' and charPressed != 'C'):
        speakTime();
    elif (charPressed == 'B'):
        speakOutStandingBill();
    elif (charPressed == 'A' or charPressed == '0'):
        global insideAlarmMode
        global insideTimerMode
        global alarmTime
        global alarmHour
        global alarmMinute
        if (charPressed == 'A' ):
            insideAlarmMode = True;
        elif (charPressed == '0' ):
            insideTimerMode = True;
        print("Alarm Mode");
        alarmTime = "";
        alarmHour =0;
        alarmMinute =0;
        #subprocess.run(["omxplayer", "/home/pi/pythonwork/keypad/alarmHelp.mp3"])
        #beep();
        #downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q=Happy to. Enter the time in 24 hours format. Then press hash key to set the alarm.&tl=en")
    elif (charPressed == '*'):
        #subprocess.run(["omxplayer", "/home/pi/pythonwork/keypad/help.mp3"])
        #subprocess.run(["omxplayer", "/home/pi/pythonwork/keypad/help2.mp3"])
        recording = sr.AudioFile('/home/pi/pythonwork/keypad/record.wav')
        r = sr.Recognizer()
        beep();
        subprocess.run(["arecord", "--device=hw:1,0", "--format", "S16_LE", "--rate", "44100","-d","2", "-c1" , "record.wav"])
        try:
            with recording as source:
                audio = r.record(source)
                value = r.recognize_google(audio)
            if str is bytes:
                reply = "{}".format(value).encode("utf-8")
            else:
                reply = "{}".format(value)
            
            reply= reply.lower();
            print("You said: %s" % reply)
            beep();
            if (reply == "help"):
                subprocess.run(["omxplayer", "/home/pi/pythonwork/keypad/help.mp3"])
                subprocess.run(["omxplayer", "/home/pi/pythonwork/keypad/help2.mp3"])
            elif (reply == "time"):
                speakTime();
            elif (reply == "school" or reply == "homework"):
                schoolHW();
            
            elif (reply == "sandeep"):
                r = requests.get("https://post-master.herokuapp.com/MakeACall?phone=919216411835&fromNumber=12111111111", allow_redirects=True)
            elif (reply == "kusum"):
                r = requests.get("https://post-master.herokuapp.com/MakeACall?phone=917837394152&fromNumber=12111111111", allow_redirects=True)
            elif (reply == "bill" or reply == "bil" or reply == "dil" or reply == "film"  or reply == "bel" or reply == "bills" or reply == "balance"):
                speakOutStandingBill();
                
            elif (reply == "light"):
                light();
            elif (reply =="nature"):
                playMusic('4');
            elif (reply =="music"):
                playMusic('6');
            elif (reply =="instrumental"):
                playMusic('5');
            elif (reply == "weather"):
                speakHourlyForecast();
                speakRainForecast();
            elif (reply == "news"):
                readNews();
            else:
                print ("#", reply,"#")
                if (reply  and len(reply) > 1):
                    print("getting cmd")
                    splitStr = reply.split()
                    cmd = splitStr[0]
                    print (cmd)
                    if (len(splitStr) >1 and (cmd == "reminder" or cmd == "remind" or cmd == "task" or cmd == "todo")):
                        i =1
                        task = ""
                        while i< len(splitStr):
                            task = task +" "+splitStr[i]
                            i = i+1;
                        print("reminder task ", task)
                        addToDo(task.strip());
                    else :
                      subprocess.run(["omxplayer", "/home/pi/pythonwork/keypad/repeat.mp3"])  
                else:
                    subprocess.run(["omxplayer", "/home/pi/pythonwork/keypad/repeat.mp3"])
                
        except Exception as e:
            print(e)
            print("Sorry, Say it, One more time, please...")
            #print ("#", reply.strip(),"#")
            subprocess.run(["omxplayer", "/home/pi/pythonwork/keypad/repeat.mp3"])
            #downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q=Sorry, Say it, One more time, please.&tl=en") 
        
        #downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q=Happy to., Press. A. to set alarm. Press. B. to get outstanding bills. Press. Hash. to get current time.&tl=en")
    elif (charPressed == 'D'):
        subprocess.run(["sudo", "reboot", "-h" ,"now" ])
    elif (charPressed == '1' or charPressed == '2' or charPressed == '3'):
        #Choco susu next time current time +1
        now = datetime.datetime.now()
        delta = 15 * int(charPressed);
        if charPressed == '3':
            delta = 60;
        print(delta)
        now_plus = now + datetime.timedelta(minutes = delta)
        chocoAlarm = str(now_plus.hour )+str(now_plus.minute)
        chocoAlarm_speak = str(now_plus.hour )+":"+str(now_plus.minute)
        print(chocoAlarm)
        f = open("/home/pi/pythonwork/keypad/ChocoNextSusu.txt", "w")
        f.write(chocoAlarm)
        f.close()
        downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q=All , Reminder set for choco walk "+chocoAlarm_speak+"&tl=en")
        
    elif (charPressed == '4' or charPressed == '5' or charPressed == '6'):
        playMusic(charPressed);
        #subprocess.run(["bash", "/home/pi/pythonwork/keypad/music/play.sh","2>&1"])
        #/home/pi/pythonwork/keypad/music/play.sh
    elif (charPressed == 'C' ):
        light();
    elif (charPressed == '7' ):
        r = requests.get("http://192.168.0.198/bellRequest/on", allow_redirects=True)
    elif (charPressed == '8' ):
        r = requests.get("https://post-master.herokuapp.com/MakeACall?phone=919216411835&fromNumber=12111111111", allow_redirects=True)
        beep();
    elif (charPressed == '9' ):
        r = requests.get("https://post-master.herokuapp.com/MakeACall?phone=917837394152&fromNumber=12111111111", allow_redirects=True)
        beep();
    elif (charPressed == '19' ):
        schoolHW();



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
        beep();
        if (insideAlarmMode ):
            setAlarm(charPressed)
        elif (insideTimerMode ):
            setTimer(charPressed)
        else:
            startWork(charPressed)
        time.sleep(.5)
    
    GPIO.output(line, GPIO.LOW)
beep();
try:
    while True:
        readLine(L1, ["1","2","3","A"])
        readLine(L2, ["4","5","6","B"])
        readLine(L3, ["7","8","9","C"])
        readLine(L4, ["*","0","#","D"])
        time.sleep(0.1)
except KeyboardInterrupt:
    print("\nApplication stopped!")

    