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

## 1. First of all we want to acquire the data from our sensors. 

To do so we chose to use Mbed to program our NUCLEO-L432KC.
In order to acquire the sensors values on our microprocessor we used a Labdeck Bread Board and connected the sensors pins to the NUCLEO pins.

![alt tag](https://user-images.githubusercontent.com/31851288/35196651-446d266a-fed5-11e7-87d6-69debb05b10d.png)
For all sensors we chose to power them all with the 3V3 pin of the NUCLEO and to use the GND pin as the ground.
- The luminosity sensor has his SDA pin connected to the PA 10 pin of the NUCLEO, his SCL pin to the PA 9. In order to have your sensor working, you need to solder your jumper on the back of the circuit in order to have the ADDR pin connected to the GND.
- The DHT22 has his SIG pin connected to the pin PA 11 of the NUCLEO.
- The Moisture sensor his SIG pin to the A0 pin.
- The soil temperature sensor, the OneWire, has his SIG pin connected to the pin PA 8.

To read the values we are receiving, we need to do a printf of the sensor' values to the serial port to analyse with Putty for exemple.

## 2. Then we want to read the values on the screen the SSD1306.

The screen is added on the Labdeck and we want to connect his pins to the NUCLEO.
- The CS pin of the screen is connected to the D3 of the microprocessor.
- The Reset pin is connected to the D6 of the SSD1306.
- The DC pin to the A5.
- The CLK to the A4 pin.
- The Data pin is connected to the D2.

The power and ground are with the 3V3 and the GND of the NUCLEO like the sensors.

## 3. Now that we have all the data collected and printed on the screen we want to send them with our Snootlab to use them on the Internet.

The first thing to do in order to be able to use your snootlab module and receive data on actoboard 
To do this, click on the tab "New data source" then on "Akeru". 
It is now up to you to enter your parameters. 
Here is an example below of the parameterization of the received data that we have carried out because it is a very important part for the reception of the data. 
We have defined our 5 variables that we will receive from the following form: variable:: uint8 which means that the received value will be an int 8. The small i to the right of the Data format box allows you to have information about other possibilities such as receiving float or boolean or to choose 8 or 16 bits. 

The Snootlab module is also added on the Labdeck.
The power system is the same as the one for the sensors and the screen.
The RX pin of the Snootlab is connected to the PA 2 pin and the TX to the pin PA 3.
To send the values to the Sigfox you need to do a printf on the sigfox (used as a serial) with a line in a certain format.
The message of the printf needs to begin by "AT$SS" to tell sigfox you are sending informations.
Then you need to send you data as an hexa value.
You can now receive this data on Actoboard.

To verify that the data has been received, you just have to go back to the board.
![alt tag](https://user-images.githubusercontent.com/31851288/35219780-40b5b4e8-ff74-11e7-8167-dae878cf41e2.PNG)

Select the module that sent the data and then go to "Settings" where all the right part called latest messages will correspond to all the received messages (with in detail the time and the day of the reception as well as the value of the sensors).
![alt tag](https://user-images.githubusercontent.com/31851288/35219783-43b839ea-ff74-11e7-95c8-9e09063858b9.PNG)

Last thing to add in the "settings"tab, the "Forwarding URL" checkbox will be the way to retrieve data from node-red.
![alt tag](https://user-images.githubusercontent.com/31851288/35219791-4701bfc2-ff74-11e7-8c7c-186c345006c6.PNG)

## 4. From Actoboard you can post the data received from the Sigfox.

The first step to recover sigfox data on Node-red is to add an http node and perform a POST on the sigfox address. 
![alt tag](https://user-images.githubusercontent.com/31851288/35232395-a4922cea-ff9b-11e7-9956-c0256cb562ed.PNG)

Once the POST is completed, Node-red will receive the expected but concatenated values. That's why we wrote several small functions in order to retrieve each value individually and be able to manipulate it. 
![alt tag](https://user-images.githubusercontent.com/31851288/35232397-a4f8667c-ff9b-11e7-9929-d947176bb402.PNG)

We have installed the "Dashboard" package which is a graphical interface for Node-red and interesting to make graphics or display values. Each value is displayed on our dashboard with underneath its history over the last hours.
![alt tag](https://user-images.githubusercontent.com/31851288/35232396-a4c7c1ac-ff9b-11e7-8203-78661f8c7d2f.PNG)

## 5. PCB 

Once we have made sure that the sensors work correctly, that the sensor data is sent from the snootlab and that they are received on node-red, we can proceed to the realization of the PCB.
Instead of having a labdec insert that takes up a bit of space, we made our own PCB to be able to place it in a case later on, we chose KiCad as the software to make our PCB, you will find below a link to get you started with this software: 
http://kicad-pcb.org/help/getting-started/
