"""
Flask Documentation:     https://flask.palletsprojects.com/
Jinja2 Documentation:    https://jinja.palletsprojects.com/
Werkzeug Documentation:  https://werkzeug.palletsprojects.com/
This file creates your application.
"""

# from crypt import methods
import site 

from app import app, Config,  mongo, Mqtt
from flask import escape, render_template, request, jsonify, send_file, redirect, make_response, send_from_directory 
from json import dumps, loads 
from werkzeug.utils import secure_filename
from datetime import datetime,timedelta, timezone
from os import getcwd
from os.path import join, exists
from time import time, ctime
from math import floor
 



#####################################
#   Routing for your application    #
#####################################


@app.route('/api/weather/<start>/<end>', methods=['GET']) 
def get_all(start,end):   
   
    if request.method == "GET":
        try:
            st = escape(start)
            e  = escape(end)
            get = mongo.getAllInRange(st,e)
            if get:
                return jsonify({"status":"found","data": get})
            
        except Exception as e:
            print(f"getAllInRange error: f{str(e)}")

    # FILE DATA NOT EXIST
    return jsonify({"status":"not found","data":[]})

@app.route('/api/weather/snapshot', methods=['GET']) 
def get_snap():   
   
    if request.method == "GET":
        try:
            get = mongo.getSnapshot()
            if get:
                return jsonify({"status":"found","data": get})
            
        except Exception as e:
            print(f"getAllInRange error: f{str(e)}")

    # FILE DATA NOT EXIST
    return jsonify({"status":"not found","data":[]})

@app.route('/api/mmar/temperature/celsius/<start>/<end>', methods=['GET']) 
def get_temp_celsius(start,end):   
   
    if request.method == "GET":
        try:
            st = escape(start)
            e  = escape(end)
            get = mongo.celTemperatureMMAR(st,e)
            if get:
                return jsonify({"status":"found","data": get})
            
        except Exception as e:
            print(f"celTemperatureMMAR error: f{str(e)}")

    # FILE DATA NOT EXIST
    return jsonify({"status":"not found","data":[]})

@app.route('/api/mmar/hi/celsius/<start>/<end>', methods=['GET']) 
def get_hi_celsius(start,end):   
   
    if request.method == "GET":
        try:
            st = escape(start)
            e  = escape(end)
            get = mongo.celHeatIndexMMAR(st,e)
            if get:
                return jsonify({"status":"found","data": get})
            
        except Exception as e:
            print(f"celHeatIndexMMAR error: f{str(e)}")

    # FILE DATA NOT EXIST
    return jsonify({"status":"not found","data":[]})

@app.route('/api/mmar/temperature/fahrenheit/<start>/<end>', methods=['GET']) 
def get_temp_fahrenheit(start,end):   
   
    if request.method == "GET":
        try:
            st = escape(start)
            e  = escape(end)
            get = mongo.farTemperatureMMAR(st,e)
            if get:
                return jsonify({"status":"found","data": get})
            
        except Exception as e:
            print(f"farTemperatureMMAR error: f{str(e)}")

    # FILE DATA NOT EXIST
    return jsonify({"status":"not found","data":[]})

@app.route('/api/mmar/hi/fahrenheit/<start>/<end>', methods=['GET']) 
def get_hi_fahrenheit(start,end):   
   
    if request.method == "GET":
        try:
            st = escape(start)
            e  = escape(end)
            get = mongo.farHeatIndexMMAR(st,e)
            if get:
                return jsonify({"status":"found","data": get})
            
        except Exception as e:
            print(f"farHeatIndexMMAR error: f{str(e)}")

    # FILE DATA NOT EXIST
    return jsonify({"status":"not found","data":[]})

@app.route('/api/mmar/humidity/<start>/<end>', methods=['GET']) 
def get_humid(start,end):   
   
    if request.method == "GET":
        try:
            st = escape(start)
            e  = escape(end)
            get = mongo.humidityMMAR(st,e)
            if get:
                return jsonify({"status":"found","data": get})
            
        except Exception as e:
            print(f"humidityMMAR error: f{str(e)}")

    # FILE DATA NOT EXIST
    return jsonify({"status":"not found","data":[]})

@app.route('/api/mmar/soilmoisture/<start>/<end>', methods=['GET']) 
def get_moist(start,end):   
   
    if request.method == "GET":
        try:
            st = escape(start)
            e  = escape(end)
            get = mongo.soilMoistureMMAR(st,e)
            if get:
                return jsonify({"status":"found","data": get})
            
        except Exception as e:
            print(f"soilMoistureMMAR error: f{str(e)}")

    # FILE DATA NOT EXIST
    return jsonify({"status":"not found","data":[]})

# TRY SOMETHING WITH ALTITUDE. YOU'LL REMEMBER WHAT IT WAS// no i freaking did not


   



   






@app.route('/api/file/get/<filename>', methods=['GET']) 
def get_images(filename):   
    '''Returns requested file from uploads folder'''
   
    if request.method == "GET":
        directory   = join( getcwd(), Config.UPLOADS_FOLDER) 
        filePath    = join( getcwd(), Config.UPLOADS_FOLDER, filename) 

        # RETURN FILE IF IT EXISTS IN FOLDER
        if exists(filePath):        
            return send_from_directory(directory, filename)
        
        # FILE DOES NOT EXIST
        return jsonify({"status":"file not found"}), 404


@app.route('/api/file/upload',methods=["POST"])  
def upload():
    '''Saves a file to the uploads folder'''
    
    if request.method == "POST": 
        file     = request.files['file']
        filename = secure_filename(file.filename)
        file.save(join(getcwd(),Config.UPLOADS_FOLDER , filename))
        return jsonify({"status":"File upload successful", "filename":f"{filename}" })

 


###############################################################
# The functions below should be applicable to all Flask apps. #
###############################################################


@app.after_request
def add_header(response):
    """
    Add headers to both force latest IE rendering engine or Chrome Frame,
    and also tell the browser not to cache the rendered page. If we wanted
    to we could change max-age to 600 seconds which would be 10 minutes.
    """
    response.headers['X-UA-Compatible'] = 'IE=Edge,chrome=1'
    response.headers['Cache-Control'] = 'public, max-age=0'
    return response

@app.errorhandler(405)
def page_not_found(error):
    """Custom 404 page."""    
    return jsonify({"status": 404}), 404



