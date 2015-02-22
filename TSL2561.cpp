/*
 * mbed library program
 *  Luminosity sensor -- LIGHT-TO-DIGITAL CONVERTER (light intensity to a digital signal output)
 *  TSL2561 by Texas Advanced Optoelectronic Solutions Inc.
 *
 * Copyright (c) 2015 Kenji Arai / JH1PJL
 *  http://www.page.sannet.ne.jp/kenjia/index.html
 *  http://mbed.org/users/kenjiArai/
 *      Created: Feburary  21st, 2015
 *      Revised: Feburary  22nd, 2015
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "TSL2561.h"

TSL2561::TSL2561 (PinName p_sda, PinName p_scl) : _i2c(p_sda, p_scl)
{
    TSL2561_addr = TSL2561_ADDRESS_GND;
    init();
}

TSL2561::TSL2561 (PinName p_sda, PinName p_scl, uint8_t addr) : _i2c(p_sda, p_scl)
{
    TSL2561_addr = addr;
    init();
}

TSL2561::TSL2561 (I2C& p_i2c) : _i2c(p_i2c)
{
    TSL2561_addr = TSL2561_ADDRESS_GND;
    init();
}

TSL2561::TSL2561 (I2C& p_i2c, uint8_t addr) : _i2c(p_i2c)
{
    TSL2561_addr = addr;
    init();
}

/////////////// Read Lux from sensor //////////////////////
/*
For 0    < CH1/CH0 < 0.50 Lux = 0.0304  x CH0-0.062  x CH0 x ((CH1/CH0)1.4)
For 0.50 < CH1/CH0 < 0.61 Lux = 0.0224  x CH0-0.031  x CH1
For 0.61 < CH1/CH0 < 0.80 Lux = 0.0128  x CH0-0.0153 x CH1
For 0.80 < CH1/CH0 < 1.30 Lux = 0.00146 x CH0-0.00112x CH1
For        CH1/CH0 > 1.30 Lux = 0
 */
float TSL2561::lux()
{
    double lux0, lux1;
    double ratio;
    double dlux;

    dt[0] = CMD_MULTI + TSL2561_DATA0LOW;
    _i2c.write((int)TSL2561_addr, (char *)dt, 1, true);
    _i2c.read(TSL2561_addr, (char *)dt, 2, false);
    ch0 = dt[1] << 8 | dt[0];
    dt[0] = CMD_MULTI + TSL2561_DATA1LOW;
    _i2c.write((int)TSL2561_addr, (char *)dt, 1, true);
    _i2c.read(TSL2561_addr, (char *)dt, 2, false);
    ch1 = dt[1] << 8 | dt[0];
    lux0 = ch0;
    lux1 = ch1;
    ratio = lux1 / lux0;
    if (ratio <= 0.5) {
        dlux = 0.03040 * lux0 - 0.06200 * lux0 * pow(ratio,1.4);
    } else if (ratio <= 0.61) {
        dlux = 0.02240 * lux0 - 0.03100 * lux1;
    } else if (ratio <= 0.80) {
        dlux = 0.01280 * lux0 - 0.01530 * lux1;
    } else if (ratio <= 1.30) {
        dlux = 0.00146 * lux0 - 0.00112 * lux1;
    } else {
        dlux = 0;
    }
    return (float)dlux;
}

/////////////// Initialize ////////////////////////////////
void TSL2561::init()
{
    _i2c.frequency(100000);
    power_up();
    read_ID();
}

/////////////// ID ////////////////////////////////////////
uint16_t TSL2561::read_ID()
{
    dt[0] = CMD_SINGLE + TSL2561_ID;
    _i2c.write((int)TSL2561_addr, (char *)dt, 1, true);
    _i2c.read(TSL2561_addr, (char *)dt, 2, false);
    id_number = dt[0] << 8 | dt[1];
    return id_number;
}

uint8_t TSL2561::who_am_i()
{
    read_ID();
    if ((id_number >> 4) == I_AM_TSL2561) {
        return 1;
    } else {
        return 0;
    }
}

/////////////// Power ON/OFF //////////////////////////////
void TSL2561::power_up()
{
    dt[0] = CMD_SINGLE + TSL2561_CONTROL;
    dt[1] = 3;
    _i2c.write((int)TSL2561_addr, (char *)dt, 2, false);
}

void TSL2561::power_down()
{
    dt[0] = CMD_SINGLE + TSL2561_CONTROL;
    dt[1] = 0;
    _i2c.write((int)TSL2561_addr, (char *)dt, 2, false);
}

/////////////// I2C Freq. /////////////////////////////////
void TSL2561::frequency(int hz)
{
    _i2c.frequency(hz);
}
