
import requests
import urllib.request
import json
import re

weatherFile = '/home/pi/pythonwork/keypad/weather/weather.html';

class weatherObj:  
    def __init__(self, date, forecast, percent):
        date = date.replace("Sun", "Sunday, ")
        date = date.replace("Mon", "Monday, ")
        date = date.replace("Tue", "Tuesday, ")
        date = date.replace("Wed", "Wednesday, ")
        date = date.replace("Thu", "Thursday, ")
        date = date.replace("Fri", "Friday, ")
        date = date.replace("Sat", "Saturday, ") 
        self.date = date
        
        forecast = forecast.replace("T-Storms", "Thunderstorm, ")
        self.forecast = forecast
        self.percent = percent
    def __str__(self):
        return self.date +" , " +self.forecast + " , "+ self.percent +" ,"
        

def downloadWeather():
    r = requests.get("https://weather.com/en-IN/weather/tenday/l/6a6f9bf8a5d6b2953fd3a5e17b0d4bc9d4eb2f4265f03368500f754be20d0958", allow_redirects=True) 
    open(weatherFile, 'wb').write(r.content)
    

def extractWeatherFields():
    downloadWeather();
    weatherForecast = []
    f = open(weatherFile, "r")
    weatherData = f.read()
    
    start = 0;
    while(True):
        #Find date
        dateString = "<h2 data-testid=\"daypartName\" class=\"DetailsSummary--daypartName--1Mebr\">";
        try:
            start = weatherData.index(dateString,start) ;
        except ValueError as ve:
            break;
        
            
        start += len(dateString);
        end = weatherData.index("</h2>",start)
        date = weatherData[start:end]
        
        
        #Find forecast
        forecastString = "<span class=\"DetailsSummary--extendedData--aaFeV\">";
        start = weatherData.index(forecastString, start) + len(forecastString)
        end = weatherData.index("</span>",start)
        forecast = weatherData[start:end]
        
        #find percent
        percentString = "<span data-testid=\"PercentageValue\">";
        start = weatherData.index(percentString, start) + len(percentString)
        end = weatherData.index("</span>",start)
        percent = weatherData[start:end]
        weather_Obj = weatherObj(date, forecast, percent)
        weatherForecast.append( weather_Obj )
        #print(weather_Obj)
    return weatherForecast;

def findRain():
    weatherForecast = extractWeatherFields();
    rainForecast =[];
    i = 0;
    for obj in weatherForecast:
        i += 1;
        if ( i == 1 or (obj.forecast != "Sunny" and obj.forecast != "Mostly Sunny") ) :
            rainForecast.append(obj)
            print(obj)
    return rainForecast;
    



