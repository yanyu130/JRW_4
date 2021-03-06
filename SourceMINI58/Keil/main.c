/******************************************************************************
 * @file     main.c
 * @version  
 * $Revision: 
 * $Date: 
 * @brief    
 *
 * @note
 * Copyright (C) 
*****************************************************************************/
#include <stdio.h>

#include "def.h"
#include "ConfigTable.h"
#include "led.h"
#include "Timer_Ctrl.h"
#include "Motor.h"
#include "Battery.h"
#include "UartCtrl.h"
#include "FlashCtrl.h"
#include "RC.h"
#include "Control.h"
#include "I2CDev.h"
#include "MPU6050.h"
#include "IMUSO3.h"
#include "IMU.h"
#include "DMP.h"
#include "Comm.h"
#include "FailSafe.h"
#include "Report.h"

void setupSystemClock(void)
{
		SYS_UnlockReg();
	
		/* Enable HIRC clock */
		CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

		/* Waiting for HIRC clock ready */
		CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

		/* Switch HCLK clock source to HIRC */
		CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV_HCLK(1));
	
		/* To update the variable SystemCoreClock */
    SystemCoreClockUpdate();
	
		SYS_LockReg();
}

void MotorTest(void)
{
//	static bool falg = false;
	//		if(GetFrameCount() > 6000 && GetFrameCount() < 8000)
//		{
//			Motor_Start();
//			MotorPwmOutput(20,20,20,20);
//		}
//		else if(GetFrameCount() >= 8000  && GetFrameCount() < 10000)
//		{
//			MotorPwmOutput(40,40,40,40);
//		}
//		else if(GetFrameCount() >= 10000   && GetFrameCount() < 12000)
//		{
//			MotorPwmOutput(60,60,60,60);
//		}
//		else if(GetFrameCount() >= 12000  && GetFrameCount() < 14000)
//		{
//			MotorPwmOutput(80,80,80,80);
//		}
//		else if(GetFrameCount() >= 14000 && GetFrameCount() < 16000)
//		{
//			MotorPwmOutput(100,100,100,100);
//		}
//		else 
//		if(GetFrameCount() >= 40000 && !falg)
//		{
//			falg = true;
//			MotorPwmOutput(0,0,0,0);
//			Motor_Stop();
//		}
}

void setup()
{
//	uint8_t i=0;
	bool bMPU6050Status = TRUE;
	//初始化系统时钟
	setupSystemClock();
	
	//初始化串口
	setupUART();
	UART_NVIC_INIT();

	//初始化System_tick
	setup_system_tick(SYSTEM_TICK_FREQ);
	
	//初始化LED
	LED_Init();
	LED_ON();
	
	//初始化IIC
	I2C_Init();
	DelayMsec(300);		//延迟下，再去读传感器，不延迟下，读取传感器会失败
	
	//初始化SENSOR
	#ifdef IMU_SW											//软件姿态解算
		bMPU6050Status = MPU6050_initialize();
		//陀螺仪初始化失败，灯循环闪烁
		if(bMPU6050Status != TRUE)
		{
			while(1) 
			{
				printf("MPU6050 initial faile\n");
				DelayMsec(100);
				LED_ON();
				DelayMsec(100);
				LED_OFF();
			}
		}
	#else
		MPU6050_initialize();
		DelayMsec(1000);			//必须加延迟，否则读取陀螺仪数据出错
		MPU6050_DMP_Initialize();     //初始化DMP引擎
	#endif
	
	//初始化FLASH
	FlashInit();
	LoadParamsFromFlash();
	
	//初始化低电压检测
	//BatteryCheckInit();
	
	//初始化遥控
	Comm_Init();
	
	
	
	//测试用，延迟启动时间
//	for(i=0;i<6;i++)
//	{
//		LED_ON();
//		DelayMsec(1000);
//		LED_OFF();
//	}

	//初始化电机
	Motor_Init();
	//Motor_Start();
	//电机怠转
	//MotorPwmOutput(20,20,20,20);
	
	//初始化自稳定
	IMU_Init();			// sample rate and cutoff freq.  sample rate is too low now due to using dmp.
	
	//printf("\n\nCPU @ %dHz\n", SystemCoreClock);

}



void loop()
{
		static uint32_t nextTick = 0;
		while(getSystemTime()<nextTick){}
		nextTick = getSystemTime()+TICK_FRAME_PERIOD;	//循环间隔FRAME
	
		//处理蓝牙命令
		CommandProcess();
			
		//读取遥控命令
		Comm_Process();
	
		if(GetFrameCount()%10 == 0)
		{
			
			//读取姿态传感器数据
			#ifdef IMU_SW												//软件姿态解算
				IMUSO3Thread();
			#else
				DMP_Routing();	//DMP 线程  所有的数据都在这里更新
			#endif
			
			//imu校准
			if(imuCaliFlag)
			{
					if(IMU_Calibrate())
					{
						imuCaliFlag=0;
						gParamsSaveEEPROMRequset=1;	//请求记录到EEPROM
						imu.caliPass=1;
						LED_OFF();
					}
			}
				
			//PID二环角速度
			CtrlAttiRate();
			//控制电机
			CtrlMotor();
		}

		if(GetFrameCount()%20 == 0)
		{
			//处理遥控数据
			
			//PID一环角度控制
			CtrlAttiAng();
		}

		if(GetFrameCount()%1000 == 0)
		{
			//电池电池检测
			//BatteryCheck();
			
			
			//遥控通信丢失处理
			
			//更新LED灯状态
			UpdateLED();
		}
		
		//故障保护
		if(GetFrameCount()%100 == 0)
		{
			//飞控翻覆时，停止电机
			 FailSafeCrash();
			
			//电池低电压处理
			//printf("Convert result is %d\n", GetBatteryAD());
			
			
		}
		
		//打印调试信息
		if(GetFrameCount()%100 == 0)
		{
			ReportMessage();
		}
		
		
		IncFrameCount(1);
}


int main()
{
  setup();
	while(TRUE) loop();
}

/*** (C) COPYRIGHT ***/
