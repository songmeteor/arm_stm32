
#include "bmp180.h"
#include <stdio.h>

#define BMP180_CALIBRATION_START 0xAA
#define BMP180_CALIBRATION_LENGTH 11

extern I2C_HandleTypeDef hi2c1;


HAL_StatusTypeDef BMP180_init(BMP180_get_cal_param* bmp)
{
    bmp->mode = BMP180_STANDARD;

    return BMP180_read_calibration_data(bmp);
}


// step 1. EEPROM에서 데이터 읽기

HAL_StatusTypeDef BMP180_read_calibration_data(BMP180_get_cal_param* bmp)
{
	uint8_t calib_data[BMP180_CALIBRATION_LENGTH*2];

	if (HAL_I2C_Mem_Read(&hi2c1, BMP180_I2C_ADDR << 1, BMP180_CALIBRATION_START,
			I2C_MEMADD_SIZE_8BIT, calib_data, BMP180_CALIBRATION_LENGTH*2, 100) != HAL_OK)
	{
	    return HAL_ERROR;
	}

	bmp->AC1 = (calib_data[0] << 8) | calib_data[1];
	bmp->AC2 = (calib_data[2] << 8) | calib_data[3];
	bmp->AC3 = (calib_data[4] << 8) | calib_data[5];
	bmp->AC4 = (calib_data[6] << 8) | calib_data[7];
	bmp->AC5 = (calib_data[8] << 8) | calib_data[9];
	bmp->AC6 = (calib_data[10] << 8) | calib_data[11];
	bmp->B1 = (calib_data[12] << 8) | calib_data[13];
	bmp->B2 = (calib_data[14] << 8) | calib_data[15];
	bmp->MB = (calib_data[16] << 8) | calib_data[17];
	bmp->MC = (calib_data[18] << 8) | calib_data[19];
	bmp->MD = (calib_data[20] << 8) | calib_data[21];

	 return HAL_OK;
}

// step 2 온도 읽기 & 보정값 계산

HAL_StatusTypeDef BMP180_read_temp(BMP180_get_cal_param* bmp)
{
	uint8_t data[2] = {0};  //F6, F7주소에 있는 값 저장
	uint8_t BMP180_READ_TEMP = 0x2E;

	 if (HAL_I2C_Mem_Write(&hi2c1, BMP180_I2C_ADDR << 1, BMP180_CONTROL_REG,1,   //
	                       &BMP180_READ_TEMP, 1, 1000) != HAL_OK)
	   {
	      return HAL_ERROR;
	   }

	 HAL_Delay(5); //5ms 대기

	 if (HAL_I2C_Mem_Read(&hi2c1, BMP180_I2C_ADDR << 1, BMP180_RESULT_REG,
	                      1, data, 2, 1000) != HAL_OK)
	 {
	     return HAL_ERROR;
	 }

	 bmp->UT = (data[0] << 8) | data[1]; // 온도값 저장

	 long X1 = ((bmp->UT-bmp->AC6) * bmp->AC5 ) >> 15;
	 long X2 = ((long)bmp->MC << 11) / (X1 + bmp->MD);
	 bmp->B5 = X1 + X2;
	 bmp->temp = (bmp->B5 + 8) >> 4;
	 bmp->temp = bmp->temp / 10;

	 return HAL_OK;
}

//step 3 압력 읽기 & 보정값 계산

HAL_StatusTypeDef BMP180_read_pres(BMP180_get_cal_param* bmp)
{
	uint8_t data[3] = {0};
	uint8_t BMP180_READ_PRES = 0x34 + (bmp->mode << 6);

	 if (HAL_I2C_Mem_Write(&hi2c1, BMP180_I2C_ADDR << 1, BMP180_CONTROL_REG, 1,
	                      &BMP180_READ_PRES, 1, 1000) != HAL_OK)
	   {
	      return HAL_ERROR;
	   }

	 HAL_Delay(26); // BMP180_ULTRAHIGHRES 모드에서 대기시간이 모드들 중 최대값인 25.5라서 26으로 설정

    if (HAL_I2C_Mem_Read(&hi2c1, BMP180_I2C_ADDR << 1, BMP180_RESULT_REG,
                         1, data, 3, 1000) != HAL_OK)
    {
        return HAL_ERROR;
    }

    bmp->UP = ((data[0] << 16) | (data[1] << 8) | data[2]) >> (8 - bmp->mode);

    long B6 = bmp->B5 - 4000;
    long X1 = (bmp->B2 * (B6 * B6 / 4096)) / 2048;
    long X2 = (bmp->AC2 * B6 ) / 2048;
    long X3 = X1 + X2;
    long B3 = (((bmp->AC1 * 4 + X3) << bmp->mode) + 2) / 4;
    X1 = (bmp->AC3 * B6) >> 13;
    X2 = (bmp->B1 * (B6 * B6 >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    unsigned long B4 = bmp->AC4 * (unsigned long)(X3 + 32768) >> 15;
    unsigned long B7 = ((unsigned long)bmp->UP - B3) * (50000 >> bmp->mode);

 	if(B7 < 0x80000000)
 	{
 		bmp->pres = (B7 * 2) / B4;
 	}
 	else
 	{
 		bmp->pres = (B7 / B4) * 2;
 	}

 	X1 = (bmp->pres >> 8) * (bmp->pres >> 8);
 	X1 = (X1 * 3038) >> 16;
 	X2 = (-7357 * bmp->pres) >> 16;
 	bmp->pres = bmp->pres + ((X1 + X2 + 3791) >> 4);
 	bmp->pres = bmp->pres / 100;

 	return HAL_OK;
}
