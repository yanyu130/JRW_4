#ifndef PID_H
#define PID_H

// PID结构
typedef struct
{
    float P;
    float I;
    float D;
    float Desired;
    float Error;
    float PreError;
    float PrePreError;
    float Increment;
    float Integ;
		float iLimit;
    float Deriv;
    float Output;
 
}PID_Typedef;



extern PID_Typedef pitch_angle_PID;	  //pitch角度环PID
extern PID_Typedef pitch_rate_PID;		//pitch角速率环PID

extern PID_Typedef roll_angle_PID;    //roll角度环PID
extern PID_Typedef roll_rate_PID;     //roll角速率环PID

extern PID_Typedef yaw_angle_PID;     //yaw角度环PID
extern PID_Typedef yaw_rate_PID;      //yaw角速率环PID  

 
#endif
