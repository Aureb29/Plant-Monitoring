#include "mbed.h"
#include "DS18B20.h"
#include "OneWireDefs.h"
#include "TSL2561.h"
//------------------------------------
// Hyperterminal configuration
// 9600 bauds, 8-bit data, no parity
//------------------------------------


//#define THERMOMETER DS18S20
#define THERMOMETER DS18B20

Serial serie(SERIAL_TX, SERIAL_RX);
TSL2561 LUM(PB_7, PB_6); // Luminosité (I2C:SDA,SCL)
AnalogIn Humidite(PA_0);
Serial PC(PA_2, PA_3);
DigitalOut myled(LED1);

int main()
{   
    
    int i = 1;
    serie.printf("Hello World !\n");
     // device( crcOn, useAddress, parasitic, mbed pin )
    THERMOMETER device(true, true, false, D3);
    
    while (!device.initialize());    // keep calling until it works
    
    while (true)
    {   
        wait(1);
        serie.printf("This program runs since %d seconds.\n", i++);
        myled = !myled;
        printf("Lum: %f \r\n", LUM.lux());
        printf("Hum: %f \n\r",Humidite.read());
        // changing the resolutions only affects the DS18B20. The DS18S20 is fixed.
        device.setResolution(nineBit);
        device.readTemperature(); 
        wait(2);
        device.setResolution(tenBit);
        device.readTemperature(); 
        wait(2);
        device.setResolution(elevenBit);
        device.readTemperature(); 
        wait(2);
        device.setResolution(twelveBit);
        device.readTemperature(); 
        wait(2);
    }
    return EXIT_SUCCESS;
}
