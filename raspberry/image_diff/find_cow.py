#!/usr/bin/python3
#from skimage import measure
import datetime
from skimage.metrics import structural_similarity
import cv2
import requests
from PIL import Image
from shutil import copyfile
import io
import os
# Imports the Google Cloud client library
from google.cloud import vision
import sys
sys.path.insert(1, "/home/pi/pythonwork/image_diff")
import findindex

def isNight():
    now = datetime.datetime.now()
    if  now.hour >= 18 or now.hour < 7 :
        return True;
    return False;

if (isNight()):
    #print("It is night")
    quit();
    
print("")
#Name the file
now = datetime.datetime.now();
month = str(now.month);
if (now.month <10):
    month = "0"+month

day = str(now.day);
if (now.day <10):
    day = "0"+day

hour = str(now.hour);
if (now.hour <10):
    hour = "0"+hour
    
minute = str(now.minute);
if (now.minute <10):
    minute = "0"+minute
    
filename_ts = month+"_"+day+"_"+hour+"_"+minute+".jpg";

print(filename_ts)


base_image = "/home/pi/pythonwork/image_diff/img/base.jpg";
base_image_org = "/home/pi/pythonwork/image_diff/img/base_orignal.jpg";
new_image = "/home/pi/pythonwork/image_diff/img/new_img.jpg";
new_image_org = "/home/pi/pythonwork/image_diff/img/new_img_orignal.jpg";
merged_image_org = "/home/pi/pythonwork/image_diff/img/merged_img_orignal.jpg";



#export GOOGLE_APPLICATION_CREDENTIALS="/home/pi/keys/cow_idonotremember-app-ec1f74a76fdd.json"

#click new photo
r = requests.get("http://192.168.0.104:8080/photo.jpg", allow_redirects=True) 
open(new_image_org, 'wb').write(r.content)

#corp the image
left = 0
top = 200
right = 3328
bottom = 1500
img = Image.open(new_image_org)
im1 = img.crop((left, top, right, bottom))
im1.save(new_image_org)
print("corp image saved")
#quit();


# reduce pixel of the photo
basewidth = 1280
img = Image.open(new_image_org)
wpercent = (basewidth / float(img.size[0]))
hsize = int((float(img.size[1]) * float(wpercent)))
img = img.resize((basewidth, hsize), Image.ANTIALIAS)
img.save(new_image)
#quit();

# load the two input images
imageA = cv2.imread(base_image)
imageB = cv2.imread(new_image)
# convert the images to grayscale
grayA = cv2.cvtColor(imageA, cv2.COLOR_BGR2GRAY)
grayB = cv2.cvtColor(imageB, cv2.COLOR_BGR2GRAY)

# compute the Structural Similarity Index (SSIM) between the two
# images, ensuring that the difference image is returned
(score, diff) = structural_similarity(grayA, grayB, full=True)
diff = (diff * 255).astype("uint8")
print("SSIM: {}".format(score))

#Merge orignal images for goole vision 
image1 = Image.open(base_image_org)
image2 = Image.open(new_image_org)
image1_size = image1.size
image2_size = image2.size
merged_image = Image.new('RGB',(2*image1_size[0], image1_size[1]), (250,250,250))# ndw image of RGB mode, double the width of image 1 , and white color
merged_image.paste(image1,(0,0))
merged_image.paste(image2,(image1_size[0],0))
merged_image.save(merged_image_org)
               
               
#copy this image to 
copyfile(new_image, base_image)
#/home/pi/pythonwork/image_diff/img/back

copyfile(new_image, "/home/pi/pythonwork/flask/static/home55/"+filename_ts)
copyfile(new_image, base_image)
copyfile(new_image_org, base_image_org)

if (score < .65):
    print("check with google")
    # Instantiates a client
    client = vision.ImageAnnotatorClient()
    # The name of the image file to annotate
    file_name = os.path.abspath(merged_image_org)
    # Loads the image into memory
    with io.open(file_name, 'rb') as image_file:
        content = image_file.read()
    """
    image_google = vision.Image(content=content)
    # Performs label detection on the image file
    response = client.label_detection(image=image_google)
    labels = response.label_annotations
    cowFound = False;
    for label in labels:
        src = label.description.lower();
        if ( findindex.index(src, "mammal") >= 0  or  findindex.index(src, "cow") >= 0 or findindex.index(src, "animal") >= 0 or findindex.index(src, "buffalo") >= 0 or findindex.index(src, "bull") >= 0):
            print(label.description.lower())
            cowFound = True;
            
        else:
            print(" general label" , label.description.lower())
    if (cowFound):
        r = requests.get("https://post-master.herokuapp.com/MakeACall?phone=919216411835&fromNumber=12111111112", allow_redirects=True)
    else :
        r = requests.get("https://post-master.herokuapp.com/MakeACall?phone=919216411835&fromNumber=12111111113", allow_redirects=True)
    """
    dnd_file = '/home/pi/pythonwork/image_diff/dnd.txt'
    if os.path.exists(dnd_file):
        print("DND mode")
    else:
        r = requests.get("https://post-master.herokuapp.com/MakeACall?phone=919216411835&fromNumber=12111111113", allow_redirects=True)
    
    
        
else:
    print("Almost identical images")
    



