from flask import Flask, redirect, url_for, request,  render_template, send_from_directory
from flask import Response
import json

import os
app = Flask(__name__)
app.config['LOG_FILE'] = '/home/pi/pythonwork/flask/logs/application.log'

reminderFile = '/home/pi/pythonwork/keypad/alarmMessage.txt'
reminderTime = '/home/pi/pythonwork/keypad/alarm.txt'
dnd_file = '/home/pi/pythonwork/image_diff/dnd.txt'
@app.route('/')
@app.route('/index')
def index():
    reminder_Text = "";
    reminder_Time = "";
    if os.path.exists(reminderTime):
        f = open(reminderTime, "r")
        reminder_Time = f.read()
        
    if os.path.exists(reminderFile):
        f = open(reminderFile, "r")
        reminder_Text =  f.read()
    
    
    return render_template('index.html', rText = reminder_Text, rTime=reminder_Time)


@app.route('/cow')
def cow():
    hists = os.listdir('/home/pi/pythonwork/flask/static/home55')
    hists.sort(reverse=True)
    hists = ['home55/' + file for file in hists]
    return render_template('cow.html', hists = hists)

@app.route('/dnd')
def dnd():
    dict1 = {"status": "OFF"}
    if os.path.exists(dnd_file):
        os.remove(dnd_file)
        dict1 = {"status": "OFF"}
        
    else:
        f = open(dnd_file, "w")
        f.write("DND turn on")
        f.close()
        dict1 = {"status": "ON"}
    return Response(json.dumps(dict1), mimetype='application/json')

@app.route('/img/<path:path>')
def send_img(path):
    return send_from_directory('img', path)

@app.route('/deleteReminder',methods = ['POST', 'GET'])
def deleteReminder():
    if os.path.exists(reminderTime):
        os.remove(reminderTime)
    if os.path.exists(reminderFile):
        os.remove(reminderFile)
    return redirect(url_for('index'))
        
@app.route('/addReminder',methods = ['POST', 'GET'])
def addReminder():
    rText = request.args.get('rText')
    print(rText)
    f = open(reminderFile, "w")
    f.write(rText)
    f.close()
    
    rTime = request.args.get('rTime')
    print(rTime)
    f = open(reminderTime, "w")
    f.write(rTime)
    f.close()
    return redirect(url_for('index'))
