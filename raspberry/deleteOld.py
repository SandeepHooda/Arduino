import pathlib
import os , time, sys

hists = os.listdir('/home/pi/pythonwork/flask/static/home55')
now = time.time()

#Delete DND file
dnd_file = '/home/pi/pythonwork/image_diff/dnd.txt'
if os.path.exists(dnd_file):
    os.remove(dnd_file)
    
#print(hists)
for x in hists:
    fname = pathlib.Path('/home/pi/pythonwork/flask/static/home55/'+x)
    print(os.stat(fname).st_mtime)
    if os.stat(fname).st_mtime < now - 1 * 86400:
        if os.path.isfile(fname):
            os.remove(os.path.join(path, fname))