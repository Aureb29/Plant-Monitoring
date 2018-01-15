#include "mbed.h"
#include "DS18B20.h"
#include "OneWireDefs.h"
#include "TSL2561.h"
#include "DHT22.h"
#include "bold_font.h"
#include "standard_font.h"
#include "ssd1306.h"

#define THERMOMETER DS18B20

Serial serie(SERIAL_TX, SERIAL_RX);
TSL2561 LUM(PA_10, PA_9); // Luminosity pins (PA_10 -> SDA / PA_9 -> SCL) 
DHT22 Tem_Hum_A(PA_11); // Temperature & Humidity of air pin
AnalogIn Humidite(PA_0); // Moisture pin
SSD1306 ecran(D3 /* cs */, D6/* reset */, A5 /* dc */, A4 /* clock */, D2 /* data */); // Display pins
//Serial PC(PA_2, PA_3);
Serial sigfox(PA_2, PA_3); // Sigfox pins (PA_2 for sigfox RX / PA_3 for sigfox TX)
DigitalOut myled(LED1); // Blink led to check program

int i, Hum_sol, Temp_sol, Lum, Temp_air, Hum_air; // variables for sensors data

int main()
{   
    /********** Display initialisation *************/
    ecran.initialise();
    ecran.clear();
    ecran.set_contrast(255);
    ecran.set_font(bold_font, 8);
    
    serie.printf("New Program \r\n");
    /********** OneWire initialisation *************/
    THERMOMETER device(true, true, false, PA_8); // device( crcOn, useAddress, parasitic, mbed pin )
    
    while (!device.initialize());    // keep calling until it works
    
    while (true)
    {   
        myled = !myled; // Blink Led
        Tem_Hum_A.sample(); // Launch DHT22
        device.readTemperature(); // Launch OneWire
        Temp_air = (int)(Tem_Hum_A.getTemperature()/10); // Read DHT22 temperature
        Hum_air = (int) (Tem_Hum_A.getHumidity()/10); // Read DHT22 humidity
        Hum_sol = (int)(Humidite.read()* 100) ; // Read moisture
        Temp_sol = (int)device.readTemperature(); // Read OneWire temperature
        Lum = (int) LUM.lux(); // Read Luminosity
        
        /***************** Serial displayed data *********************/
        
        printf("Temperature air: %d\n\r", Tem_Hum_A.getTemperature()/10);
        printf("Hum air: %d\n\r", Tem_Hum_A.getHumidity()/10);
        serie.printf("\rLum: %f \r\n", LUM.lux());
        serie.printf("Temperature sol: %d \n\r",Temp_sol);
        serie.printf("Hum sol: %f \n\r",Humidite.read()*100);
        printf("------------------------------------------------------------\n\r");
        
        /************************ Sending data on Sigfox device *********************/
        
        sigfox.printf("AT$SS=%02x %02x %02x %02x %02x \r\n", Temp_sol, Temp_air, Lum, Hum_sol, Hum_air);
        
        /********************** OLED displayed data ********************/
        
        ecran.set_font(bold_font, 8);
        ecran.printf("Valeurs");
        ecran.printf("\r\n"); 
        ecran.update();
        ecran.set_font(standard_font, 6);
        ecran.printf("Temp air = %d",Temp_air );
        ecran.printf("\r\n");
        ecran.printf("Hum air = %d", Tem_Hum_A.getHumidity()/10);
        ecran.printf("\r\n");
        ecran.printf("Lum = %f", LUM.lux());
        ecran.printf("\r\n");
        ecran.printf("Temp sol = %d",Temp_sol );
        ecran.printf("\r\n");
        ecran.printf("Hum sol = %f", Humidite.read()*100);
        ecran.printf("\r\n");
        ecran.update();
        ecran.clear();
        
        
        
        wait(10); // waiting because sigfox only allows 140 sends per day
    }
    return EXIT_SUCCESS;
}
