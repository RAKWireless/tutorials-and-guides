//Business Logic functions

#ifndef B_LOGIC_H
#define B_LOGIC_H

#include "bar_module.h"
#include "acce_module.h"
#include "gnss_module.h"

/* DISCLAIMER!
The atmospheric pressure values ​​to determine the floor of the building of 15 floors, they were taken at a temperature of 20°C, 2600 meters above sea level, and on a sunny day, the pressure data may vary according to
atmospheric conditions. It is recommended to take readings at different times of the day and under different weather conditions
and use them according to your application*/

float floor_values[] = {753.9, 753.51, 753.24, 753.01, 752.79, 752.57, 752.39, 752.17, 751.91, 751.66, 751.42, 751.22, 751, 750.77, 750.6};
float pressure;
acc_data accelerometer;

struct gnss_data_buff{
	int32_t d_lat, d_lon;
};
//@brief function to determine the floor based on the taken measurements, please take your own measurements for your applications
uint8_t getFloor(){
    uint8_t floor, i;
    pressure = read_bar();
    for (i = 0; i<(sizeof(floor_values)/sizeof(floor_values[0])); i++){
        if((pressure < floor_values[i]) && (pressure > floor_values[i+1])){
            floor = i;
            break;
        }

        else{
            floor = 0XFF;
        }
    }
    return(floor);
}
/*This function determines if the object is moving or not, taking the norm of the data of the 3 axes*/
uint8_t getMovData(){
    float result;
    uint8_t movement;
    accelerometer = read_acc();

    result = sqrt(accelerometer.x*accelerometer.x+accelerometer.y*accelerometer.y+accelerometer.z*accelerometer.z);
    if((result>0.93)&&(result<1.03)){movement = 0x00;}
    else{movement = 0x01;}

    return(movement);
}
//Function to return the latitude and longitude of the gnss data
gnss_data_buff get_gnss_buff()
{
  gnss_data_buff gnss_buffer_data;
    
    gnss_buffer_data.d_lat = read_gnss().d_lat;
    gnss_buffer_data.d_lon = read_gnss().d_lon;
    
  return(gnss_buffer_data);
}

#endif