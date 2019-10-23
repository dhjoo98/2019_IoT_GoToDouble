from flask import Flask
from flask import jsonify
from datetime import datetime
import random

app = Flask(__name__)
i = 0
reverse_flag = 1

@app.route('/')
def index():
    return 'Server Works!'

@app.route('/fposition')
def response_fposition():
    global i, reverse_flag
    i += 1 * reverse_flag

    if i >= 180 or i<= 0:
        reverse_flag *= -1

    distance = random.randrange(0, 70)
    response_json = jsonify(
        code=0,
        angle=i,
        distance=distance,
        time=datetime.now()
    )
    response_json.headers.add('Access-Control-Allow-Origin', '*')
    return response_json

@app.route('/sposition')
def response_sposition():
    global i, reverse_flag
    i += 1 * reverse_flag

    if i >= 180 or i<= 0:
        reverse_flag *= -1

    distance = random.randrange(0, 70)
    response_json = jsonify(
        code=0,
        angle=i,
        distance=distance,
        time=datetime.now()
    )
    response_json.headers.add('Access-Control-Allow-Origin', '*')
    return response_json
