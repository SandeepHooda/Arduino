from urllib.request import urlopen
from bs4 import BeautifulSoup
import requests
import urllib.request
import datetime


def downLoadWavFile(filePath):
    r = requests.get(filePath, allow_redirects=True) 
    open('/home/pi/pythonwork/gdrive/hw.mp3', 'wb').write(r.content)
    #subprocess.run(["omxplayer", "/home/pi/pythonwork/keypad/voice.mp3"])
now = datetime.datetime.now()
if (now.hour >=9 and now.hour <=19):
    print("Checking Homework")
    downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q=Your Attention please, School has uploaded homework for this week. Press D to stop this notification. &tl=en")
    html = urlopen("https://drive.google.com/drive/folders/1rRcYP6BzN1o9vsULGwJbFCd5TbwiPDwj").read()
    soup = BeautifulSoup(html,"html.parser")

    todaysTitle = soup.title.string

    titleFile = "/home/pi/pythonwork/gdrive/title.txt"
    f = open(titleFile, "r")
    oldTitle = f.read()
    if (oldTitle != todaysTitle):
        f = open(titleFile, "w")
        f.write(todaysTitle)
        f.close()
        print("Time to make a call")
        while True: # Press D to reboot
            subprocess.run(["omxplayer", "/home/pi/pythonwork/gdrive/hw.mp3"])
