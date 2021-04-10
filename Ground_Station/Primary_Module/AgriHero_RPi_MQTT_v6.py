#AgriHero RPi MQTT Code
#This code has been modified out of the code at https://core-electronics.com.au/tutorials/getting-started-with-home-automation-using-mqtt.html
#MQTT Server used: test.mosquitto.org

# MQTT Client demo
# Continuously monitor two different MQTT topics for data,
# check if the received data matches two predefined 'commands'
print("Beginning")
import paho.mqtt.client as mqtt

import requests
import json

import datetime

hum="0"
temp="0"
rain="0"
soil="0"
gas="0"
color="0"
date="23/02/21"
time="15:49:18"

check=0




# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() - if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("agrihero/humidity")
    client.subscribe("agrihero/temperature")
    client.subscribe("agrihero/rain")
    client.subscribe("agrihero/soilmoisture")
    client.subscribe("agrihero/gaspressure")
    client.subscribe("agrihero/color")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
#     hum="0"
#     temp="0"
#     rain="0"
#     soil="0"
#     gas="0"
#     color="0"
#     check=0
    #date="23/02/21"
    #time="15:49:18"
    print(msg.topic+" "+str(msg.payload))
    if(msg.topic == "agrihero/humidity"):
        global hum
        global check
        hum = str(msg.payload.decode("utf-8"))
        print(hum)
        check=1
    if(msg.topic == "agrihero/temperature"):
        global temp
        temp = str(msg.payload.decode("utf-8"))
        print(msg.payload)
        check=2
    if(msg.topic == "agrihero/rain"):
        global rain
        rain = str(msg.payload.decode("utf-8"))
        print(rain)
        check=3
    if(msg.topic == "agrihero/soilmoisture"):
        global soil
        soil = str(msg.payload.decode("utf-8"))
        print(soil)
        check=4
    if(msg.topic == "agrihero/gaspressure"):
        global gas
        gas = str(msg.payload.decode("utf-8"))
        print(gas)
        check=5
    if(msg.topic == "agrihero/color"):
        global color
        color = str(msg.payload.decode("utf-8"))
        print(color)
        check=6
    print(check)
    if(check==6):
        check=0
        print(hum)
        print(temp)
        print(rain)
        print(soil)
        print(gas)
        print(color)
        try:
            x = datetime.datetime.now()
            year=str(x.year)
            year=year[2]+year[3]
            month=str(x.month)
            if(x.month<=10):
                month = "0" + month
            day=str(x.day)
            if(x.day<=10):
                day = "0" + day
            hour=str(x.hour)
            if(x.hour<=10):
                hour = "0" + hour
            minute=str(x.minute)
            if(x.minute<=10):
                minute = "0" + minute
            second=str(x.second)
            if(x.second<=10):
                second = "0" + second

            date = day + "/" + month + "/" + year
            time = hour + ":" + minute + ":" + second
#             print(date)
            print(str(time))
            print(str(date))
            username = 'adminone'
            headers = {"Content-Type": "application/json"}
            url = f'https://agrihero-webapp.herokuapp.com//update/{username}'
            payload = {
                "u_id": "1",
                "time": str(time),
                "date": str(date),
                "temperature": str(temp),
                "humidity": str(hum),
                "av_soil_humidity": str(soil),
                "rain": str(rain),
                "wind_speed": "3",
                "camera_analysis": "NA",
                "water_status": "NA",
                "gas": str(gas),
                "color": str(color),
                "status": "All Systems are Good"
            }

            r = requests.post(url, data=json.dumps(payload), headers=headers)
            print("Data sent")
            print(r.content)
        except:
            print("try didnt work")


# Create an MQTT client and attach our routines to it.
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("test.mosquitto.org", 1883, 60)

# Process network traffic and dispatch callbacks. This will also handle
# reconnecting. Check the documentation at
# https://github.com/eclipse/paho.mqtt.python
# for information on how to use other loop*() functions





client.loop_forever()




