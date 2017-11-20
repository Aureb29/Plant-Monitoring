#include "mbed.h"
#include "DS18B20.h"
#include "OneWireDefs.h"
#include "TSL2561.h"
#include "DHT22.h"


#define THERMOMETER DS18B20

Serial serie(SERIAL_TX, SERIAL_RX);
TSL2561 LUM(PB_7, PB_6); // Luminosité (I2C:SDA,SCL)
DHT22 Tem_Hum_A(PA_11); // Température & Humidité air
AnalogIn Humidite(PA_0);
Serial PC(PA_2, PA_3);
Serial sigfox(PA_9, PA_10);
DigitalOut myled(LED1);

int i, Hum_sol, Temp_sol, Lum, Temp_air, Hum_air;

int main()
{   


    serie.printf("New Program \r\n");
     // device( crcOn, useAddress, parasitic, mbed pin )
    THERMOMETER device(true, true, false, PB_0);
    
    while (!device.initialize());    // keep calling until it works
    
    while (true)
    {   
        myled = !myled;
        Tem_Hum_A.sample();
        device.readTemperature(); 
        Temp_air = (int)(Tem_Hum_A.getTemperature()/10);
        Hum_air = (int) (Tem_Hum_A.getHumidity()/10);
        Hum_sol = (int)(Humidite.read()) * 100;
        Temp_sol = (int)device.readTemperature();
        Lum = (int) LUM.lux();
        printf("Air Temperature : %d\n\r", Tem_Hum_A.getTemperature()/10);
        printf("Air Humidity is %d\n\r", Tem_Hum_A.getHumidity()/10);
        serie.printf("\rLum: %f \r\n", LUM.lux());
        serie.printf("Hum: %f \n\r",Humidite.read()*100);
        serie.printf("Temperature du sol: %d \n\r",Temp_sol);
        serie.printf("Lum: %d \r\n", Lum);
        printf("---------------------------------------------\n\r");
        sigfox.printf("AT$SS=%02x %02x %02x %02x %02x \r\n", Temp_sol, Temp_air, Lum, Hum_sol, Hum_air);
        wait(30);
    }
    return EXIT_SUCCESS;
}
