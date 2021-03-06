#!/usr/bin/env python3

# NOTE: this example requires PyAudio because it uses the Microphone class

import speech_recognition as sr
import os
import time
import datetime
import requests
import urllib.request
import json
import subprocess
import os.path
harvard = sr.AudioFile('/home/pi/pythonwork/speach2text/test.wav')
r = sr.Recognizer()
print("Listining ...")
subprocess.run(["arecord", "--device=hw:1,0", "--format", "S16_LE", "--rate", "44100","-d","3", "-c1" , "test.wav"])
# recognize speech using Sphinx
try:
    # for testing purposes, we're just using the default API key
    # to use another API key, use `r.recognize_google(audio, key="GOOGLE_SPEECH_RECOGNITION_API_KEY")`
    # instead of `r.recognize_google(audio)`
    print(".")
    with harvard as source:
        audio = r.record(source)
        value = r.recognize_google(audio)
    if str is bytes:
        reply = "{}".format(value).encode("utf-8")
    else:
        reply = "{}".format(value)
    print("You said: %s" % reply)
   
except sr.UnknownValueError:
    print("Google Speech Recognition could not understand audio")
except sr.RequestError as e:
    print("Could not request results from Google Speech Recognition service; {0}".format(e))
