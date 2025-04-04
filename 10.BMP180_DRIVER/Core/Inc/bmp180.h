#ifndef __BMP180_H
#define __BMP180_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/* BMP180 I2C 주소 */
#define BMP180_I2C_ADDR 0x77

/* BMP180 레지스터 주소 */
#define BMP180_CONTROL_REG 0xF4
#define BMP180_RESULT_REG  0xF6
#define BMP180_READ_TEMP   0x2E
#define BMP180_READ_PRES   0x34

/* BMP180 모드 설정 */
#define BMP180_ULTRALOWPOWER 0
#define BMP180_STANDARD      1
#define BMP180_HIGHRES       2
#define BMP180_ULTRAHIGHRES  3

/* BMP180 구조체 */
typedef struct {
    I2C_HandleTypeDef* hi2c;
    int16_t AC1, AC2, AC3;
    uint16_t AC4, AC5, AC6;
    int16_t B1, B2, B5;
    int32_t UT, UP;
    int32_t temperature;
    int32_t pressure;
    uint8_t mode;
} BMP180_HandleTypeDef;

/* 함수 선언 */
HAL_StatusTypeDef BMP180_Init(BMP180_HandleTypeDef* hbmp, I2C_HandleTypeDef* hi2c);
HAL_StatusTypeDef BMP180_ReadTemperature(BMP180_HandleTypeDef* hbmp);
HAL_StatusTypeDef BMP180_ReadPressure(BMP180_HandleTypeDef* hbmp);
float BMP180_GetTemperature(BMP180_HandleTypeDef* hbmp);
float BMP180_GetPressure(BMP180_HandleTypeDef* hbmp);

#ifdef __cplusplus
}
#endif

#endif /* __BMP180_H */ 