//#include "stm32f4xx.h"
//
//int degree =5; 
//int intensity = 10;
//int cnt =0 ; 
//int getdata =40;
//
//void LED_init(void)
//{
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//  GPIO_InitTypeDef GPIO_InitStructure;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//}
//
//void TIM4_Configuration(void)
//{
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//  TIM_TimeBaseStructure.TIM_Prescaler = 2837 - 1; 
//  TIM_TimeBaseStructure.TIM_Period = 189 - 1; 
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//  TIM_Cmd(TIM4, ENABLE);
//}
//
//void PWM_TIM4_Configuration(void)
//{
//  TIM_OCInitTypeDef TIM_OCInitStructure;
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//  TIM_OCInitStructure.TIM_Pulse = 0;
//  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
//  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
//  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
//  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
//}
//
//void RGB_LED_init(void)
//{
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
//  GPIO_InitTypeDef GPIO_InitStructure;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8| GPIO_Pin_9;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
//  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
//  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
//  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);
//}
//
//void TIM2_Configuration(int interval_ms)
//{
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//  NVIC_InitTypeDef NVIC_InitStructure;
//  TIM_TimeBaseStructure.TIM_Prescaler = 26880 - 1;
//  TIM_TimeBaseStructure.TIM_Period = interval_ms - 1;
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//  TIM_Cmd(TIM2, ENABLE);
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//}
//
//
//void TIM2_IRQHandler(void)
//{
//  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//  {
////    // red
////    TIM4->CCR1 = intensity;
////    // green
////    TIM4->CCR2 = intensity;
////    // blue
////    TIM4->CCR3 = intensity;
//    
//    if (cnt % 3 == 0)
//    {
//      TIM4->CCR1 = intensity;
//      TIM4->CCR2 = intensity;
//      TIM4->CCR3 = 0;
//    }
//    if( cnt % 3 ==1 ) 
//    {
//      TIM4->CCR1 = 0;
//      TIM4->CCR2 = intensity;
//      TIM4->CCR3 = intensity;
//    }
//    if( cnt % 3 ==2)
//    {
//      TIM4->CCR1 = intensity;
//      TIM4->CCR2 = 0;
//      TIM4->CCR3 = intensity;
//    }
//    cnt ++ ;
//    
//    TIM4->CCR4 = degree+ getdata;
//    degree++;
//    if (degree > 23)
//    {
//      degree = 5;
//    }
//    if (intensity > 25600)
//    {
//      intensity = 10;
//     }
//    intensity *= 2;
//    GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
//    // clear interrupt flag
//    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//    }
//}
//
//int main()
//{
//  LED_init();
//  RGB_LED_init();
//  degree =5;
//  intensity = 10;
//  TIM4_Configuration();
//  PWM_TIM4_Configuration();
//  TIM2_Configuration(1000);
//  while(1)
//  {
//  }
//return 0;
//}
//
//
//
#include "stm32f4xx.h"
int degree = 5;
int intensity = 10;
void TIM4_Configuration(void)
{
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_TimeBaseStructure.TIM_Prescaler = 2837 - 1; 
TIM_TimeBaseStructure.TIM_Period = 189 - 1; 
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
TIM_Cmd(TIM4, ENABLE);
}


void PWM_TIM4_Configuration(void)
{
TIM_OCInitTypeDef TIM_OCInitStructure;
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
TIM_OCInitStructure.TIM_Pulse = 0;
TIM_OC1Init(TIM4, &TIM_OCInitStructure);
TIM_OC2Init(TIM4, &TIM_OCInitStructure);
TIM_OC3Init(TIM4, &TIM_OCInitStructure);
TIM_OC4Init(TIM4, &TIM_OCInitStructure);
}

void RGB_LED_init(void)
{
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_Init(GPIOB, &GPIO_InitStructure);
GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);
}

void TIM2_Configuration(int interval_ms)
{
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
NVIC_InitTypeDef NVIC_InitStructure;
TIM_TimeBaseStructure.TIM_Prescaler = 26880 - 1;
TIM_TimeBaseStructure.TIM_Period = interval_ms - 1;
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
TIM_Cmd(TIM2, ENABLE);
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler(void)
{
if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
{
TIM4->CCR1 = intensity;
TIM4->CCR2 = intensity;
TIM4->CCR3 = intensity;
TIM4->CCR4 = degree;
degree++;
if (degree > 23)
{
degree = 5;
}
if (intensity > 25600)
{
intensity = 10;
}
intensity *= 2;
GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
// clear interrupt flag
TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
}
void LED_init(void)
{
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_Init(GPIOA, &GPIO_InitStructure);
}

int main()
{
LED_init();
RGB_LED_init();
degree = 5;
intensity = 10;
TIM4_Configuration();
PWM_TIM4_Configuration();
TIM2_Configuration(1000);
while(1)
{
}
return 0;
}
