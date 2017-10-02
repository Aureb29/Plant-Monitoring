#include "mbed.h"

#include "TSL2561.h"


//Déclaration variables
TSL2561 LUM(PB_7,PB_6); // Luminosité (I2C:SDA,SCL)
AnalogIn Humidite(PA_0);
Serial PC(PA_2, PA_3);

int main() {

    while(1) {
            printf("Lum: %f \r\n", LUM.lux());
            printf("Hum: %f \n\r",Humidite.read());
          
        wait(1);
    }
}