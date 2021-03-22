import sys
import requests
import urllib.request
import subprocess
sys.path.insert(1, "/home/pi/pythonwork/keypad/weather")
import weather


def downLoadWavFile(filePath):
    r = requests.get(filePath, allow_redirects=True) 
    open('/home/pi/pythonwork/keypad/weather/hourly.mp3', 'wb').write(r.content)
    subprocess.run(["omxplayer", "-o", "local", "--loop", "/home/pi/pythonwork/keypad/weather/hourly.mp3"])
    


hourlyData = weather.extractWeatherFieldsHour();
if (len(hourlyData) > 0):
    forecastData = "";
    for obj in hourlyData:
        forecastData += "Time, "+obj.date ;
        if (int(obj.percent)  > 40):
            forecastData += ", Chance of rain are high. Chances are "+obj.percent+" percent. ";
        else:
            forecastData += ". It will be windy. Wind speed will be "+obj.wind+", km/h . ";
    downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q="+forecastData+". Press. D. to stop this message.&tl=en")
    #print(forecastData);
else:
    downLoadWavFile("http://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&q=Today, it will be sunny and quite. Press. D. to stop this message.&tl=en")
    