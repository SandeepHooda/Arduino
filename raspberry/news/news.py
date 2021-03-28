import sys
import requests
import urllib.request
import subprocess

newsHtmlFile="/home/pi/pythonwork/keypad/news/news.html"

newsHtmlFileHT="/home/pi/pythonwork/keypad/news/newsHT.html"

def downLoadWavFile(filePath):
    r = requests.get(filePath, allow_redirects=True) 
    open('/home/pi/pythonwork/keypad/news/news.mp3', 'wb').write(r.content)
    subprocess.run(["omxplayer", "-o", "local", "/home/pi/pythonwork/keypad/news/news.mp3"])
    
def downloadNews():
    r = requests.get("https://www.tribuneindia.com/", allow_redirects=True) 
    open(newsHtmlFile, 'wb').write(r.content)
    
def downloadNewsHT():
    subprocess.run(["/home/pi/pythonwork/keypad/news/getHT.sh"])
   
def cleasnseData(rawData):
    headLines = []
    for news in rawData:
        try:
            start = news.rindex(">") +1;
            
            news = news[start:]
            #print(news)
            headLines.append (news)
        except ValueError as ve:
            headLines.append (news);
    #print (headLines)  
    return headLines;
    
def extractNews():
    downloadNews();
    downloadNewsHT();
    headLines = []
    headLines.append("News from The tribune");
    #The trubune
    f = open(newsHtmlFile, "r")
    headLinesStr = f.read()
    
    start = 0;
    
    topNews = "<h1 class=\"card-title\">";
    start = headLinesStr.index(topNews,start) + len(topNews);
    topNews2 = "\">";
    start = headLinesStr.index(topNews2,start) + len(topNews2);
    end = headLinesStr.index("</a>",start)
    aHeadLine = headLinesStr[start:end]
    headLines.append(aHeadLine);
    
    
    headLineBegin = "<h4 class=\"ts-card-title\">";
    headLineBegin2 = "class=\"card-top-align\">";
    while(True):
        
        
        try:
            start = headLinesStr.index(headLineBegin,start) ;
        except ValueError as ve:
            break;
        
            
        start += len(headLineBegin);
        start = headLinesStr.index(headLineBegin2,start) + len(headLineBegin2);
        end = headLinesStr.index("</a>",start)
        aHeadLine = headLinesStr[start:end]
        aHeadLine = aHeadLine.replace("\n","");
        headLines.append(aHeadLine);
        
    
    headLineBegin = "<p class=\"card-text font-15\">";
    headLineBegin2 = "class=\"card-top-align\">";
    while(True):
        
        
        try:
            start = headLinesStr.index(headLineBegin,start) ;
        except ValueError as ve:
            break;
        
            
        start += len(headLineBegin);
        start = headLinesStr.index(headLineBegin2,start) + len(headLineBegin2);
        end = headLinesStr.index("</a>",start)
        aHeadLine = headLinesStr[start:end]
        aHeadLine = aHeadLine.replace("\n","");
        headLines.append(aHeadLine);
        
    #Hindustan times
    f = open(newsHtmlFileHT, "r")
   
    headLinesStr = f.read()
    #print(headLinesStr)
    headLineBegin = "<h2 class=\"hdg3\">";
    headLineBegin2 = ">";
    headLines.append("News from Hindustan times");
    i = 0;
    while(True):
        
        try:
            start = headLinesStr.index(headLineBegin,start) ;
        except ValueError as ve:
            break;
        
            
        start += len(headLineBegin);
        start = headLinesStr.index(headLineBegin2,start) + len(headLineBegin2);
        end = headLinesStr.index("</a>",start)
        aHeadLine = headLinesStr[start:end]
        aHeadLine = aHeadLine.replace("\n","");
        print(aHeadLine)
        headLines.append(aHeadLine);
        i +=1;
        if ( i > 20):
            break;
        
    #print(headLines)
    headLines.append("End of todays top stories");
    return cleasnseData(headLines);


extractNews()

        