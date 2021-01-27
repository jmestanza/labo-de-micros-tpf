
#include "fever.h"
#include "i2c_config.h"


#define MAX30205_ADDRESS  0X48

#define MAX30205_TEMPERATURE   0X00
#define MAX30205_CONFIGURATION 0X01
#define MAX30205_THYST         0X02
#define MAX30205_TOS           0X03


 uint8_t rawTempRead[2];
 float temperature = 0.0;


 void fever_temp_meassurement(void)
 {

	  i2cReadMsg(MAX30205_ADDRESS, rawTempRead,2, MAX30205_TEMPERATURE);

 }


float fever_get_temperature (void)
{

	uint16_t body_temp = rawTempRead[0]<< 8;

    body_temp |= rawTempRead[1];

    temperature = (float)body_temp * 0.00390625;

    return temperature;
}


