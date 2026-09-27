// need to implement the acccelorometer driver to properly finish this, using a stub value for now. 

#include "launch.h"
#include <stdint.h>
#include "ms5607.h"


typedef enum {
    PAD_IDLE,
    BOOST,
    COAST,
    APOGEE,
    DESCENT,
    FAIL
} FLIGHTSTATE;


typedef struct {
    FLIGHTSTATE STATE;  
    float PAD_PRESSURE;          
} Flight_Controller;

Flight_Controller fc;
uint16_t prom[8];

void init_flightController(Flight_Controller *fc) {
    fc->STATE = PAD_IDLE;
}

bool Baro_Init() {
    MS5607_ReadMemory(prom);
    return MS5607_VerifyDeviceCRC(prom);
    
}

void pad_init(Flight_Controller *fc) {
    float accum_pressure = 0;
    int count = PAD_SAMPLE_RATE_HZ * (PAD_ALT_AVG_WINDOW_MS / 1000);
    uint32_t raw_pressure;
    uint32_t raw_temp;
     
    for (int i = 0; i < count; i++) {
        MS5607_StartPressureConversion(OSR_4096);
        MS5607_ReadConversionResult(&raw_pressure);
        MS5607_StartTemperatureConversion(OSR_4096);
        MS5607_ReadConversionResult(&raw_temp); 
        uint32_t conv[2] = { raw_pressure, raw_temp };
        float result[2];
        // need to figure out how to read prom array from barometer
        MS5607_CalculateAbsoluteTP(conv, prom, result);
        accum_pressure += result[0];
        osDelay(1000 / PAD_SAMPLE_RATE_HZ);
    }

    fc->PAD_PRESSURE = (accum_pressure / count);
}

float ReadAccelAxisG(void) {
    return 10.0;
}

void LaunchDetect_Update(Flight_Controller *fc) {
    float accel_g;
    uint32_t raw_pressure;
    uint32_t raw_temp;
    float alt;
    uint32_t accel_start_ms = 0;
    uint32_t timeout_clock_ms = 0;
    while (1) {
        accel_g = ReadAccelAxisG();
        MS5607_StartPressureConversion(OSR_4096);
        MS5607_ReadConversionResult(&raw_pressure);
        MS5607_StartTemperatureConversion(OSR_4096);
        MS5607_ReadConversionResult(&raw_temp); 
        uint32_t conv[2] = { raw_pressure, raw_temp };
        float result[2];
        // need to figure out how to read prom array from barometer
        MS5607_CalculateAbsoluteTP(conv, prom, result);

        if (accel_g > ACCEL_THRESHOLD) {
            if (accel_start_ms == 0) {
                accel_start_ms = HAL_GetTick();
            }
        } else {
            if (timeout_clock_ms == 0) {          
               accel_start_ms = 0;
           }
        }

        if (accel_start_ms != 0 && timeout_clock_ms == 0 &&
            (HAL_GetTick() - accel_start_ms) >= ACCEL_SUSTAIN_TIME) {
            timeout_clock_ms = HAL_GetTick();   
        }

        if (timeout_clock_ms != 0) {
            MS5607_CalculateAltitude(fc->PAD_PRESSURE, result[0], &alt);
            if (alt >= LAUNCH_ALT_RISE_THRESHOLD_M) {
                // theoretically breaks out of the entire funciton and goes to update flight data which moves the state to boost
                return;
            }
        }

        if (timeout_clock_ms != 0 &&
            (HAL_GetTick() - timeout_clock_ms) >= LAUNCH_CONFIRM_TIMEOUT_MS) {
            accel_start_ms = 0;
            timeout_clock_ms = 0;
        }
        // i got ai to help with this, because we need to pace like how much cpu time this process takes up + give time for the sensors to actually collect data?
        osDelay(1000 / PAD_SAMPLE_RATE_HZ);
    } 
}


void updateFlightData(Flight_Controller *fc) {
    switch (fc->STATE) {
        case PAD_IDLE:
            if (Baro_Init()) {
                pad_init(fc);
                LaunchDetect_Update(fc);
                fc->STATE = BOOST;
            }  else {
                fc->STATE = FAIL;
            }
            break;
        
        case BOOST:
            //do something
            fc->STATE = COAST;
            break;
        
        case COAST:
            //do something
            fc->STATE = APOGEE;
            break;

        case APOGEE:
            //do something
            fc->STATE = DESCENT;
            break;
        
        case DESCENT:
            //do something
            break;

        case FAIL:
            printf("BAROMETER PROM INTEGRITY CHECK FAILED")
    }
}

