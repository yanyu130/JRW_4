#include "Control.h"
#include "mini51series.h"
#include "Timer_Ctrl.h"
#include "RC.h"
#include "motor.h"
#include "IMU.h"

float Thro=0, Roll=0, Pitch=0, Yaw=0;
int16_t Motor[4]={0};   //定义电机PWM数组，分别对应M1-M4

//IMU
typedef struct float_xyz
{
    float X;
    float Y;
    float Z;
    
}S_FLOAT_XYZ;

S_FLOAT_XYZ DIF_ACC;		//实际去期望相差的加速度


void CtrlAttiAng(void)
{
	static float yawHold=0;
		static uint32_t tPrev=0;
		float yawRateTarget=0;
		float angTarget[3]={0};
		float dt=0,t=0;
//		t=micros();
		t = millis();
		dt=(tPrev>0)?(t-tPrev):0;
		tPrev=t;
		
		angTarget[ROLL]=(float)(RC_DATA.ROOL);
		angTarget[PITCH]=(float)(RC_DATA.PITCH);
		

//		if(headFreeMode)
//		{
//			#ifdef YAW_CORRECT
//        float radDiff = -(imu.yaw - headHold) * M_PI_F / 180.0f; 
//			#else
//				float radDiff = (imu.yaw - headHold) * M_PI_F / 180.0f; 
//			#endif
//        float cosDiff = cosf(radDiff);
//        float sinDiff = sinf(radDiff);
//        float tarPitFree = angTarget[PITCH] * cosDiff + angTarget[ROLL] * sinDiff;
//        angTarget[ROLL] = angTarget[ROLL] * cosDiff - angTarget[PITCH] * sinDiff;
//        angTarget[PITCH] = tarPitFree;
//		}
 
		PID_Postion_Cal(&pitch_angle_PID,angTarget[PITCH],imu.pitch,dt);
		PID_Postion_Cal(&roll_angle_PID,angTarget[ROLL],imu.roll,dt);	 
}

void CtrlAttiRate(void)
{
	float yawRateTarget=0;
	static uint32_t tPrev=0; 

	float dt=0,t=0;
//	t=micros();
	t=millis();
	dt=(tPrev>0)?(t-tPrev):0;
	tPrev=t;
		
		yawRateTarget=-(float)RC_DATA.YAW;

		PID_Postion_Cal(&pitch_rate_PID,pitch_angle_PID.Output,imu.gyro[PITCH]*180.0f/M_PI_F,dt);	
		PID_Postion_Cal(&roll_rate_PID,roll_angle_PID.Output,imu.gyro[ROLL]*180.0f/M_PI_F,dt);//gyroxGloble
		PID_Postion_Cal(&yaw_rate_PID,yawRateTarget,imu.gyro[YAW]*180.0f/M_PI_F,dt);//DMP_DATA.GYROz

		Pitch = pitch_rate_PID.Output;
    Roll  = roll_rate_PID.Output;
    Yaw   = yaw_rate_PID.Output; 
}

void CtrlMotor(void)
{
	//负反馈
	//DIF_ACC.Z =  imu.accb[2] - CONSTANTS_ONE_G;
	//Thro -= 150*DIF_ACC.Z;
	
	//将输出值融合到四个电机 
		Motor[2] = (int16_t)(Thro - Pitch - Roll - Yaw );    //M3  
		Motor[0] = (int16_t)(Thro + Pitch + Roll - Yaw );    //M1
		Motor[3] = (int16_t)(Thro - Pitch + Roll + Yaw );    //M4 
		Motor[1] = (int16_t)(Thro + Pitch - Roll + Yaw );    //M2
	
		MotorPwmOutput(Motor[0],Motor[1],Motor[2],Motor[3]);   
}
