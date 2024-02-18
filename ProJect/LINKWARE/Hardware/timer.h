#ifndef TIMER_H
#define TIMER_H
#include "main.h"

#define TIM_PSC_APB1 ((APB1_TIMER_CLOCKS/PWM_FREQUENCE)/PWM_RESOLUTION -1)
#define PWM_FREQUENCE 100             // ±÷”   
#define PWM_RESOLUTION 10000         // 50hz,SERVO_DEFAULT_DUTY = 1500ms* 50 / 100
#define SERVO_DEFAULT_DUTY 1500      // 50hz,1500us,1.5ms
#define TIM_PSC_APB2 ((APB2_TIMER_CLOCKS/PWM_FREQUENCE)/PWM_RESOLUTION -1)
#define APB1_TIMER_CLOCKS 90000000
#define APB2_TIMER_CLOCKS 180000000

extern void TIM1_Init(uint16_t arr, uint16_t psc);
extern void TIM3_Init(uint16_t arr, uint16_t psc);
extern void TIM5_Init(void);
extern void TIM6_Init(uint16_t arr, uint16_t psc);
extern void TIM12_Init(uint16_t arr, uint16_t psc);
#endif
