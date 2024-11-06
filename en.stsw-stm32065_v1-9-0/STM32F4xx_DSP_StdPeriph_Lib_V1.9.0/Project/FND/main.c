#include "stm32f4xx.h"

void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

// 포트 c의 13번 포트  :: user switch
void Switch_Init(void)
{
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void FND_Init(void)
{
  // c번포트,out mode, 0번~7번, outtype pp, no pull  ( A B C D E F G DP )  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 
  | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  // B번포트 , out mode , 0~3번 , outtype pp, no pull ( FND 의 common:  12 ,9, 8, 6 핀 ) 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// font리스트 : 0~ 9 까지 
unsigned char Font[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xD8, 0x80, 0x90};

// 카운트 
void Count_Progress(int d_3, int d_2, int d_1, int d_0)
{
GPIO_Write(GPIOB, 0x0008);
GPIO_Write(GPIOC, Font[d_0]);
Delay(10000);
GPIO_Write(GPIOB, 0x0004);
GPIO_Write(GPIOC, Font[d_1]);
Delay(10000);
GPIO_Write(GPIOB, 0x0002);
GPIO_Write(GPIOC, Font[d_2]);
Delay(10000);
GPIO_Write(GPIOB, 0x0001);
GPIO_Write(GPIOC, Font[d_3]);
Delay(10000);
}

// pc 13번 
void EXTI13_Configuration(void)
{
RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure; 
EXTI_InitStructure.EXTI_Line = EXTI_Line13;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure);
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; 
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
}

int cnt= 0; 

void EXTI15_10_IRQHandler(void) 
{
  if (EXTI_GetITStatus(EXTI_Line13) != RESET) 
  {
    cnt++;
  }
EXTI_ClearITPendingBit(EXTI_Line13);
}


int main()
{
  FND_Init();
  Switch_Init();
  EXTI13_Configuration();
  while(1)
  {
    Count_Progress(cnt/1000, ((cnt/100)%10), ((cnt/10)%10), (cnt%10));
  }
  return 0;
}