#ifndef _VL53_H_
#define _VL53_H_

// This array will store the amount of cake in that position.
extern int CakeStatus[3];
extern int VL53_RangeResult[3];

// 1. Call this function to init.
void VL53_Init();

// 2. Call this function to measurement the height of ground.
void VL53_FirstMeasurement();

// 3. Call this function to update data.
void VL53_Update();

#endif
