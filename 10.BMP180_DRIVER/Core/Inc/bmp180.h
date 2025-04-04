#ifndef __BMP180_H
#define __BMP180_H

#include "main.h"

#define BMP180_I2C_WRITE_ADDR 0x77 // BMP180 I2C write 주소
#define BMP180_I2C_READ_ADDR 0x78 // BMP180 I2C read 주소

#define BMP180_CONTROL_REG 0xF4
#define BMP180_RESULT_REG  0xF6
#define BMP180_READ_TEMP   0x2E //주소가 아니라 값인거 같음
#define BMP180_READ_PRES   0x34

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

} BMP180_get_cal_param;


#endif /* __BMP180_H */




//#include "main.h"
//
//
///* BMP180 레지스터 주소 */
//#define BMP180_CONTROL_REG 0xF4
//#define BMP180_RESULT_REG  0xF6
//#define BMP180_READ_TEMP   0x2E
//#define BMP180_READ_PRES   0x34
//
///* BMP180 모드 설정 */
//#define BMP180_ULTRALOWPOWER 0
//#define BMP180_STANDARD      1
//#define BMP180_HIGHRES       2
//#define BMP180_ULTRAHIGHRES  3
//
///* BMP180 구조체 */
//typedef struct {
//    I2C_HandleTypeDef* hi2c;
//    int16_t AC1, AC2, AC3;
//    uint16_t AC4, AC5, AC6;
//    int16_t B1, B2, B5;
//    int32_t UT, UP;
//    int32_t temperature;
//    int32_t pressure;
//    uint8_t mode;
//} BMP180_HandleTypeDef;
//
///* 함수 선언 */
//HAL_StatusTypeDef BMP180_Init(BMP180_HandleTypeDef* hbmp, I2C_HandleTypeDef* hi2c);
//HAL_StatusTypeDef BMP180_ReadTemperature(BMP180_HandleTypeDef* hbmp);
//HAL_StatusTypeDef BMP180_ReadPressure(BMP180_HandleTypeDef* hbmp);
//float BMP180_GetTemperature(BMP180_HandleTypeDef* hbmp);
//float BMP180_GetPressure(BMP180_HandleTypeDef* hbmp);
//
//#ifdef __cplusplus
//}
//#endif
//
//#endif /* __BMP180_H */
