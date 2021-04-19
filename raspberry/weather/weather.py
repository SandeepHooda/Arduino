
import requests
import urllib.request
import json
import re

weatherFile = '/home/pi/pythonwork/keypad/weather/weather.html';
weatherFileHourly = '/home/pi/pythonwork/keypad/weather/weatherHourly.html';

class weatherObj:  
    def __init__(self, date, forecast, percent, wind):
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
        self.percent = percent.replace("%", "")
        self.wind = wind.replace("km/h", "");
    def __str__(self):
        return self.date +" , " +self.forecast + " , "+ self.percent +" ," +self.wind
        

def downloadWeather():
    r = requests.get("https://weather.com/en-IN/weather/tenday/l/6a6f9bf8a5d6b2953fd3a5e17b0d4bc9d4eb2f4265f03368500f754be20d0958", allow_redirects=True) 
    open(weatherFile, 'wb').write(r.content)
    
def downloadWeatherHourly():
    r = requests.get("https://weather.com/en-IN/weather/hourbyhour/l/6a6f9bf8a5d6b2953fd3a5e17b0d4bc9d4eb2f4265f03368500f754be20d0958", allow_redirects=True) 
    open(weatherFileHourly, 'wb').write(r.content)
    
def extractWeatherFieldsDay():
    downloadWeather();
    return extractWeatherFields(weatherFile);

def extractWeatherFieldsHour():
    downloadWeatherHourly();
    weatherForecast = extractWeatherFields(weatherFileHourly);
    rainForecast =[];
    i = 0;
    for obj in weatherForecast:
        i += 1;
        if ( i < 14 and ( int(obj.percent)  > 40 or int(obj.wind)  > 20) ) :
            rainForecast.append(obj)
            #print(obj)
    return rainForecast;


def extractWeatherFields(fileName):
    downloadWeather();
    weatherForecast = []
    f = open(fileName, "r")
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
        
        #find wind speed
        windSpeedString = "<span data-testid=\"Wind\" class=\"Wind--windWrapper--1Va1P undefined\">"
        start = weatherData.index(windSpeedString, start) + len(windSpeedString)
        windSpeedString = "<!-- -->"
        start = weatherData.index(windSpeedString, start) + len(windSpeedString)
        end = weatherData.index("</span>",start)
        wind = weatherData[start:end]
        
        weather_Obj = weatherObj(date, forecast, percent, wind)
        weatherForecast.append( weather_Obj )
        #print(weather_Obj)
    return weatherForecast;

def findRain():
    weatherForecast = extractWeatherFieldsDay();
    rainForecast =[];
    i = 0;
    for obj in weatherForecast:
        i += 1;
        if ( i == 1 or (obj.forecast != "Sunny" and obj.forecast != "Mostly Sunny") ) :
            rainForecast.append(obj)
            print(obj)
    return rainForecast;
    

#findRain();
#extractWeatherFieldsHour();

