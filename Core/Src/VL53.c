#include "VL53.h"

// vl530x api
#include "vl53l0x_api.h"

extern I2C_HandleTypeDef hi2c1;

// XSHUT for setting each VL53
// 1 -> PC11
// 2 -> PC10
// 3 -> PD2
GPIO_TypeDef *TOF_XSHUT_PORT[3] = { GPIOB, GPIOB, GPIOB };
const uint16_t TOF_XSHUT_PIN[3] = { GPIO_PIN_13, GPIO_PIN_12, GPIO_PIN_5 };
const int VL53_ADDRESS[3] = { 0, 1, 2 };

char isVL53_InitSuccess[3];
int8_t VL53_Status[3];
int VL53_RangeResult[3];
VL53L0X_RangingMeasurementData_t RangingData[3];

VL53L0X_Dev_t vl53l0x_c[3];
VL53L0X_DEV vl53_dev[3];

int GroundHeight[3];
int CakeStatus[3];

int CakeHeightGate[3][3];

void VL53_Init() {

	uint32_t refSpadCount[3];
	uint8_t isApertureSpads[3];
	uint8_t VhvSettings[3];
	uint8_t PhaseCal[3];

	for (size_t i = 0; i < 3; i++) {
		vl53_dev[i] = &vl53l0x_c[i];
		HAL_GPIO_WritePin(TOF_XSHUT_PORT[i], TOF_XSHUT_PIN[i], GPIO_PIN_RESET); // put XSHUT to low (Disable VL53)
		HAL_Delay(100);
	}

	for (size_t i = 0; i < 3; i++) {
		isVL53_InitSuccess[i] = 1;
		VL53L0X_Error Status = VL53L0X_ERROR_NONE;

		HAL_GPIO_WritePin(TOF_XSHUT_PORT[i], TOF_XSHUT_PIN[i], GPIO_PIN_SET); // put XSHUT to high (Enable VL53)
		HAL_Delay(100);

		vl53_dev[i]->I2cHandle = &hi2c1;
		vl53_dev[i]->I2cDevAddr = 0x29 << 1; // 0x52 -> Default address
		Status = VL53L0X_SetDeviceAddress(vl53_dev[i],
				(0x29 + VL53_ADDRESS[i] + 1) << 1);
		vl53_dev[i]->I2cDevAddr = (0x29 + VL53_ADDRESS[i] + 1) << 1;
		if (Status != VL53L0X_ERROR_NONE) {
			isVL53_InitSuccess[i] = 0;
		}

		// VL53L0X init for Single Measurement
		Status = VL53L0X_DataInit(vl53_dev[i]);
		if (Status != VL53L0X_ERROR_NONE) {
			isVL53_InitSuccess[i] = 0;
		}

		VL53L0X_StaticInit(vl53_dev[i]);
		VL53L0X_PerformRefCalibration(vl53_dev[i], &VhvSettings[i],
				&PhaseCal[i]);
		VL53L0X_PerformRefSpadManagement(vl53_dev[i], &refSpadCount[i],
				&isApertureSpads[i]);

		/* Set VL53 range profiles */
		/* Change the data below base on user manual. */

		// Enable Sigma
		// Standard :      (18 * 65536)
		// High accuracy : (18 * 65536)
		// Long range :    (60 * 65536)
		// High speed :    (32 * 65536)
		VL53L0X_SetLimitCheckEnable(vl53_dev[i],
		VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
		VL53L0X_SetLimitCheckValue(vl53_dev[i],
		VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, (FixPoint1616_t) (18 * 65536));

		// Enable Return Signal Rate
		// Standard :      (0.25 * 65536)
		// High accuracy : (0.25 * 65536)
		// Long range :    (0.10 * 65536)
		// High speed :    (0.25 * 65536)
		VL53L0X_SetLimitCheckEnable(vl53_dev[i],
		VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
		VL53L0X_SetLimitCheckValue(vl53_dev[i],
		VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,
				(FixPoint1616_t) (0.25 * 65536));

		// Set the total measurement time
		// Standard :       30000
		// High accuracy : 200000
		// Long range :     33000
		// High speed :     20000
		VL53L0X_SetMeasurementTimingBudgetMicroSeconds(vl53_dev[i], 200000);

		// Enable Long Distance
		// VL53L0X_SetVcselPulsePeriod(vl53_dev[i], VL53L0X_VCSEL_PERIOD_PRE_RANGE, 18);
		// VL53L0X_SetVcselPulsePeriod(vl53_dev[i], VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 14);
	}

}

void VL53_FirstMeasurement() {
	const int MeasurementTime = 3;
	int AverageHeight[3] = { 0 };

	for (size_t Time = 0; Time < MeasurementTime; Time++) {
		for (size_t i = 0; i < 3; i++) {
			VL53_Status[i] = VL53L0X_PerformSingleRangingMeasurement(
					vl53_dev[i], &(RangingData[i]));
			AverageHeight[i] += RangingData[i].RangeMilliMeter;
		}
	}
	for (size_t i = 0; i < 3; i++) {
		GroundHeight[i] = AverageHeight[i] / 3.0;
	}

	for (size_t i = 0; i < 3; i++) {
		CakeHeightGate[i][0] = GroundHeight[i] - CAKE_HEIGHT / 2;
		CakeHeightGate[i][1] = CakeHeightGate[i][0] - CAKE_HEIGHT;
		CakeHeightGate[i][2] = CakeHeightGate[i][1] - CAKE_HEIGHT;
	}
}

// Measurement distance
void VL53_Update() {
	for (size_t i = 0; i < 3; i++) {
		VL53_Status[i] = VL53L0X_PerformSingleRangingMeasurement(vl53_dev[i],
				&(RangingData[i]));
		VL53_RangeResult[i] = RangingData[i].RangeMilliMeter;
	}

	for (size_t i = 0; i < 3; i++) {
		if (VL53_RangeResult[i] < CakeHeightGate[i][2]) {
			CakeStatus[i] = 3;
		}
		else if (VL53_RangeResult[i] < CakeHeightGate[i][1]) {
			CakeStatus[i] = 2;
		}
		else if (VL53_RangeResult[i] < CakeHeightGate[i][0]) {
			CakeStatus[i] = 1;
		}
		else {
			CakeStatus[i] = 0;
		}
	}
}
