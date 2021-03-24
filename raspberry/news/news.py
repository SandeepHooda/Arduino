import sys
import requests
import urllib.request

newsHtmlFile="/home/pi/pythonwork/keypad/news/news.html"

def downLoadWavFile(filePath):
    r = requests.get(filePath, allow_redirects=True) 
    open('/home/pi/pythonwork/keypad/news/news.mp3', 'wb').write(r.content)
    subprocess.run(["omxplayer", "-o", "local", "/home/pi/pythonwork/keypad/news/news.mp3"])
    
def downloadNews():
    r = requests.get("https://www.tribuneindia.com/", allow_redirects=True) 
    open(newsHtmlFile, 'wb').write(r.content)
    
def extractNews():
    downloadNews()
    headLines = []
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
    
    while(True):
        
        headLineBegin = "<h4 class=\"ts-card-title\">";
        try:
            start = headLinesStr.index(headLineBegin,start) ;
        except ValueError as ve:
            break;
        
            
        start += len(headLineBegin);
        headLineBegin2 = "class=\"card-top-align\">";
        start = headLinesStr.index(headLineBegin2,start) + len(headLineBegin2);
        end = headLinesStr.index("</a>",start)
        aHeadLine = headLinesStr[start:end]
        aHeadLine = aHeadLine.replace("\n","");
        
        headLines.append(aHeadLine);
        
    while(True):
        
        headLineBegin = "<p class=\"card-text font-15\">";
        try:
            start = headLinesStr.index(headLineBegin,start) ;
        except ValueError as ve:
            break;
        
            
        start += len(headLineBegin);
        headLineBegin2 = "class=\"card-top-align\">";
        start = headLinesStr.index(headLineBegin2,start) + len(headLineBegin2);
        end = headLinesStr.index("</a>",start)
        aHeadLine = headLinesStr[start:end]
        aHeadLine = aHeadLine.replace("\n","");
        
        headLines.append(aHeadLine);
        
    print(headLines)
    return headLines;

#downloadNews()
#extractNews()

        