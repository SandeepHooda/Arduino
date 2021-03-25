import io
import os
import sys
sys.path.insert(1, "/home/pi/pythonwork/image_diff")
import findindex

#export GOOGLE_APPLICATION_CREDENTIALS="/home/pi/pythonwork/image_diff/gcp_keys/cow_idonotremember-app-ec1f74a76fdd.json"
# Imports the Google Cloud client library
from google.cloud import vision

# Instantiates a client
client = vision.ImageAnnotatorClient()

# The name of the image file to annotate
file_name = os.path.abspath('/home/pi/pythonwork/image_diff/img/cow.jpg')

# Loads the image into memory
with io.open(file_name, 'rb') as image_file:
    content = image_file.read()

image = vision.Image(content=content)

# Performs label detection on the image file
response = client.label_detection(image=image)
labels = response.label_annotations

print('Labels:')
for label in labels:
    src = label.description.lower();
    if ( findindex.index(src, "mammal") >= 0  or  findindex.index(src, "cow") >= 0 or findindex.index(src, "animal") >= 0 or findindex.index(src, "buffalo") >= 0 or findindex.index(src, "bull") >= 0):
        print(label.description.lower())
    else:
        print(" general label" , label.description.lower())
       