# Plant-Monitoring
This project is a plant monitoring station.
The plant monitor is developed using a NUCLEO-L432KC.
The sensors used are the following:
- Temperature and humidity sensor for ambiant informations the DHT22.
- Luminosity sensor the TSL2561.
- Moisture sensor used for the soil moisture informations the Moisture Sensor v1.4 from Grove.
- Temperature of the soil sensor the OneWire.

The informations from this 4 sensors are displayed on a LCD screen the SSD1306 which is a 128x64 display.
In this project we are using a Sigfox module called the Snootlab. 
This module allows us to send data to the cloud that are then collectable on Actoboard.
Actoboard is a platform on which you can edit dashboards to overview all your data in one place.
It also allows us to forward the information to other websites.

![alt tag](https://user-images.githubusercontent.com/31851288/35196651-446d266a-fed5-11e7-87d6-69debb05b10d.png)
