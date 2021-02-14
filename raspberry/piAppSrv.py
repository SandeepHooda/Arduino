from flask import Flask, redirect, url_for, request,  render_template
app = Flask(__name__)

reminderFile = '/home/pi/pythonwork/keypad/alarmMessage.txt'
reminderTime = '/home/pi/pythonwork/keypad/alarm.txt'
@app.route('/')
@app.route('/index')
def index():
    f = open(reminderTime, "r")
    reminderText = f.read()
    f = open(reminderFile, "r")
    reminderText = "Time "+reminderText +":"+ f.read()
    return render_template('index.html', rText = reminderText)

@app.route('/addReminder',methods = ['POST', 'GET'])
def addReminder():
    rText = request.args.get('rText')
    print(rText)
    f = open(reminderFile, "w")
    f.write(rText)
    f.close()
    return redirect(url_for('index'))
