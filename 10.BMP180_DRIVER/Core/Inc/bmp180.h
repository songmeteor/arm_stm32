#ifndef __BMP180_H
#define __BMP180_H

#include "main.h"

#define BMP180_I2C_WRITE_ADDR 0xEF // BMP180 I2C write 주소
#define BMP180_I2C_READ_ADDR 0xEE // BMP180 I2C read 주소

#define BMP180_I2C_ADDR 	0x77

#define BMP180_CONTROL_REG 0xF4
#define BMP180_RESULT_REG  0xF6
//#define BMP180_READ_TEMP   0x2E //주소가 아니라 값인거 같음
//#define BMP180_READ_PRES   0x34

#define BMP180_ULTRALOWPOWER 0
#define BMP180_STANDARD      1
#define BMP180_HIGHRES       2
#define BMP180_ULTRAHIGHRES  3

typedef struct {
    short AC1, AC2, AC3;
    unsigned short AC4, AC5, AC6;
    short B1, B2;
    long B5;
    long UT, UP;
    short MB, MC, MD;
    short mode;   //over sampling setting. 0:ultra low power 1: standard, 2: high 3: ultra high resolution
    long temp;
    long pres;

} BMP180_get_cal_param;

HAL_StatusTypeDef BMP180_read_calibration_data(BMP180_get_cal_param* bmp);
HAL_StatusTypeDef BMP180_read_temp(BMP180_get_cal_param* bmp);
HAL_StatusTypeDef BMP180_read_pres(BMP180_get_cal_param* bmp);
HAL_StatusTypeDef BMP180_init(BMP180_get_cal_param* bmp);

#endif /* __BMP180_H */


