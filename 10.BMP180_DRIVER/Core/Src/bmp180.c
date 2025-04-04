#include "bmp180.h"
#include <math.h>

/* 보정 데이터 레지스터 주소 */
#define BMP180_CALIBRATION_START 0xAA
#define BMP180_CALIBRATION_LENGTH 11

/* 보정 데이터 읽기 함수 */
static HAL_StatusTypeDef BMP180_ReadCalibrationData(BMP180_HandleTypeDef* hbmp)
{
    uint8_t calib_data[BMP180_CALIBRATION_LENGTH];
    
    if (HAL_I2C_Mem_Read(hbmp->hi2c, BMP180_I2C_ADDR << 1, BMP180_CALIBRATION_START, 
                         I2C_MEMADD_SIZE_8BIT, calib_data, BMP180_CALIBRATION_LENGTH, 100) != HAL_OK)
    {
        return HAL_ERROR;
    }
    
    hbmp->AC1 = (calib_data[0] << 8) | calib_data[1];
    hbmp->AC2 = (calib_data[2] << 8) | calib_data[3];
    hbmp->AC3 = (calib_data[4] << 8) | calib_data[5];
    hbmp->AC4 = (calib_data[6] << 8) | calib_data[7];
    hbmp->AC5 = (calib_data[8] << 8) | calib_data[9];
    hbmp->AC6 = (calib_data[10] << 8) | calib_data[11];
    hbmp->B1 = (calib_data[12] << 8) | calib_data[13];
    hbmp->B2 = (calib_data[14] << 8) | calib_data[15];
    
    return HAL_OK;
}

/* BMP180 초기화 */
HAL_StatusTypeDef BMP180_Init(BMP180_HandleTypeDef* hbmp, I2C_HandleTypeDef* hi2c)
{
    hbmp->hi2c = hi2c;
    hbmp->mode = BMP180_STANDARD;
    
    return BMP180_ReadCalibrationData(hbmp);
}

/* 온도 읽기 */
HAL_StatusTypeDef BMP180_ReadTemperature(BMP180_HandleTypeDef* hbmp)
{
    uint8_t data[2];
    
    // 온도 측정 시작
    if (HAL_I2C_Mem_Write(hbmp->hi2c, BMP180_I2C_ADDR << 1, BMP180_CONTROL_REG, 
                         I2C_MEMADD_SIZE_8BIT, &BMP180_READ_TEMP, 1, 100) != HAL_OK)
    {
        return HAL_ERROR;
    }
    
    HAL_Delay(5); // 변환 시간 대기
    
    // 결과 읽기
    if (HAL_I2C_Mem_Read(hbmp->hi2c, BMP180_I2C_ADDR << 1, BMP180_RESULT_REG, 
                         I2C_MEMADD_SIZE_8BIT, data, 2, 100) != HAL_OK)
    {
        return HAL_ERROR;
    }
    
    hbmp->UT = (data[0] << 8) | data[1];
    
    // 온도 계산
    int32_t X1 = ((hbmp->UT - hbmp->AC6) * hbmp->AC5) >> 15;
    int32_t X2 = (hbmp->B1 * ((X1 * X1) >> 12)) >> 11;
    hbmp->B5 = X1 + X2;
    hbmp->temperature = (hbmp->B5 + 8) >> 4;
    
    return HAL_OK;
}

/* 압력 읽기 */
HAL_StatusTypeDef BMP180_ReadPressure(BMP180_HandleTypeDef* hbmp)
{
    uint8_t data[3];
    uint8_t oss = hbmp->mode << 6;
    
    // 압력 측정 시작
    if (HAL_I2C_Mem_Write(hbmp->hi2c, BMP180_I2C_ADDR << 1, BMP180_CONTROL_REG, 
                         I2C_MEMADD_SIZE_8BIT, &BMP180_READ_PRES, 1, 100) != HAL_OK)
    {
        return HAL_ERROR;
    }
    
    // 변환 시간 대기 (모드에 따라 다름)
    HAL_Delay(26);
    
    // 결과 읽기
    if (HAL_I2C_Mem_Read(hbmp->hi2c, BMP180_I2C_ADDR << 1, BMP180_RESULT_REG, 
                         I2C_MEMADD_SIZE_8BIT, data, 3, 100) != HAL_OK)
    {
        return HAL_ERROR;
    }
    
    hbmp->UP = ((data[0] << 16) | (data[1] << 8) | data[2]) >> (8 - hbmp->mode);
    
    // 압력 계산
    int32_t B6 = hbmp->B5 - 4000;
    int32_t X1 = (hbmp->B2 * (B6 * B6 >> 12)) >> 11;
    int32_t X2 = hbmp->AC2 * B6 >> 11;
    int32_t X3 = X1 + X2;
    int32_t B3 = (((hbmp->AC1 * 4 + X3) << hbmp->mode) + 2) >> 2;
    X1 = hbmp->AC3 * B6 >> 13;
    X2 = (hbmp->B1 * (B6 * B6 >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    uint32_t B4 = hbmp->AC4 * (uint32_t)(X3 + 32768) >> 15;
    uint32_t B7 = ((uint32_t)hbmp->UP - B3) * (50000 >> hbmp->mode);
    
    if (B7 < 0x80000000) {
        hbmp->pressure = (B7 * 2) / B4;
    } else {
        hbmp->pressure = (B7 / B4) * 2;
    }
    
    hbmp->pressure = (hbmp->pressure + 8) >> 4;
    
    return HAL_OK;
}

/* 온도 값 가져오기 (섭씨) */
float BMP180_GetTemperature(BMP180_HandleTypeDef* hbmp)
{
    return (float)hbmp->temperature / 10.0f;
}

/* 압력 값 가져오기 (hPa) */
float BMP180_GetPressure(BMP180_HandleTypeDef* hbmp)
{
    return (float)hbmp->pressure / 100.0f;
} 