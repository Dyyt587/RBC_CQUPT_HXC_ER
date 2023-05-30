#ifndef __pid_H
#define __pid_H

#include "stm32f4xx_hal.h"

enum {
    LLAST = 0,                //上上次
    LAST = 1,                 //上次
    NOW = 2,                  //本次

    POSITION_PID,             //位置式PID
    DELTA_PID,                //增量式PID
};
typedef struct __pid_t {
    float p;                  //Kp
    float i;                  //Ki
    float d;                  //Kd

    float set[3];             //目标值,包含NOW， LAST， LLAST上上次
    float get[3];             //测量值
    float err[3];             //误差

    float pout;               //p输出
    float iout;               //i输出
    float dout;               //d输出

    float pos_out;            //本次位置式输出
    float last_pos_out;       //上次位置式输出
    float delta_u;            //本次增量值 Δ
    float delta_out;          //本次增量式输出 = last_delta_out + delta_u
    float last_delta_out;     //上次增量式输出

    float max_err;            //最大误差
    float deadband;           //err < deadband return  死区
    uint32_t pid_mode;        //pid模式   <位置式>   <增量式>
    uint32_t MaxOutput;       //输出限幅
    uint32_t IntegralLimit;   //积分限幅

    void (*f_param_init)(struct __pid_t *pid,  //PID参数初始化
                         uint32_t pid_mode,
                         uint32_t maxOutput,
                         uint32_t integralLimit,
                         float p,
                         float i,
                         float d);

    void (*f_pid_reset)(struct __pid_t *pid, float p, float i, float d);        //pid三个参数修改

} pid_t;

void PID_struct_init(
    pid_t *pid,                      //PID结构体
    uint32_t mode,                   //PID模式 (增量式 DELTA_PID,,位置式 POSITION_PID)
    uint32_t maxout,                 //PID最大输出
    uint32_t intergral_limit,        //积分限幅

    float kp,                        //KP 比例
    float ki,                        //KI 积分
    float kd);                       //KD 微分

float pid_calc(pid_t *pid, float get, float set);

void abs_limit(float *a, float ABS_MAX);

#endif
