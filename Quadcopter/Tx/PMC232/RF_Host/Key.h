#ifndef _KEY_H_
#define _KEY_H_

//ADCͨ��
#define PITCH_ADC_CH		1
#define ROLL_ADC_CH			2
#define YAW_ADC_CH			3
#define THROTTLE_ADC_CH		4	//����
#define KEY_ADC_CH			7


#define PITCH_MIDDLE_POS_ADC_VAL	128
#define ROLL_MIDDLE_POS_ADC_VAL		128
#define YAW_MIDDLE_POS_ADC_VAL		128
#define THROTTLE_ACTIVE_ADC_VAL		10


#define KEY1_ACTIVE_ADC_VAL				0x0f90
#define KEY1_ACTIVE_ADC_VAL_LOW			(KEY1_ACTIVE_ADC_VAL-0x10)
#define KEY1_ACTIVE_ADC_VAL_HIGH		(KEY1_ACTIVE_ADC_VAL+0x10)

#define KEY2_ACTIVE_ADC_VAL				0xD13
#define KEY2_ACTIVE_ADC_VAL_LOW			(KEY2_ACTIVE_ADC_VAL-0x10)
#define KEY2_ACTIVE_ADC_VAL_HIGH		(KEY2_ACTIVE_ADC_VAL+0x10)

#define KEY3_ACTIVE_ADC_VAL				0xA90
#define KEY3_ACTIVE_ADC_VAL_LOW			(KEY3_ACTIVE_ADC_VAL-0x10)
#define KEY3_ACTIVE_ADC_VAL_HIGH		(KEY3_ACTIVE_ADC_VAL+0x10)

#define KEY4_ACTIVE_ADC_VAL				0X693
#define KEY4_ACTIVE_ADC_VAL_LOW			(KEY4_ACTIVE_ADC_VAL-0x10)
#define KEY4_ACTIVE_ADC_VAL_HIGH		(KEY4_ACTIVE_ADC_VAL+0x10)

#define KEY5_ACTIVE_ADC_VAL				176
#define KEY5_ACTIVE_ADC_VAL_LOW			(KEY5_ACTIVE_ADC_VAL-0x10)
#define KEY5_ACTIVE_ADC_VAL_HIGH		(KEY5_ACTIVE_ADC_VAL+0x10)

#define KEY6_ACTIVE_ADC_VAL				207
#define KEY6_ACTIVE_ADC_VAL_LOW			(KEY6_ACTIVE_ADC_VAL-10)
#define KEY6_ACTIVE_ADC_VAL_HIGH		(KEY6_ACTIVE_ADC_VAL+10)


void Key_Noise_Filter_Init	(void);
void Key_Process			(void);


#endif
