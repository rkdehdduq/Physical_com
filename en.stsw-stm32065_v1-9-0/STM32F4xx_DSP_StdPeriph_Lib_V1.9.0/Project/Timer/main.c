// Timer 는 counter 임 
// 초당 생성되는 클럭펄스의 개수를 카운터 한다   
// 
#include "stm32f4xx.h"

int FND_cnt = 0;
unsigned char Font[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xD8, 0x80, 0x90};


void Delay(__IO uint32_t nCount)
{
for(; nCount != 0; nCount--);
}

// 유저 스위치 설정 PC13 
void SW_init(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
}

// 기본 LED 설정 PA5
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

//  FND 설정 
// A B C D E F G DP : PC 0~7
// common : PB 0~3 
void FND_Init(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// FND 출력
void Count_Progress(int d_3, int d_2, int d_1, int d_0)
{
  //  첫번째
  GPIO_Write(GPIOB, 0x0008); 
  GPIO_Write(GPIOC, Font[d_0]);
  Delay(1000);
  
  // 두번째 
  GPIO_Write(GPIOB, 0x0004); 
  GPIO_Write(GPIOC, Font[d_1]);
  Delay(1000);
  
  // 세번째 
  GPIO_Write(GPIOB, 0x0002); 
  GPIO_Write(GPIOC, Font[d_2]);
  Delay(1000);
  
  //네번째
  GPIO_Write(GPIOB, 0x0001); 
  GPIO_Write(GPIOC, Font[d_3]);
  Delay(1000);
}



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

void EXTI15_10_IRQHandler(void) 
{
/* Make sure that interrupt flag is set */
  if (EXTI_GetITStatus(EXTI_Line13) != RESET) 
  {
    FND_cnt =0 ; 
  
  /* Clear interrupt flag */
  EXTI_ClearITPendingBit(EXTI_Line13);
  }
}


void TIM2_Configuration(int interval_ms)
{
  
  // Timer2 에  clock 공급 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  
  // Timer 사용을  위한 구조체 변수 선언 
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  // prescaler , ARR( Period) 값 지정
  TIM_TimeBaseStructure.TIM_Prescaler = 26880 - 1;
  TIM_TimeBaseStructure.TIM_Period = interval_ms - 1;
  
  // up mode 
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  
  // 구조체 이용해서 Timer2 초기화  ( 설정값을 Timer2에게 전달) 
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  
  //---------위는 Timer 설정 코드 // ------------------------------------
  
  // 설정한 count값까지 도달하면 interrupt 발생하도록 활성화 
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  
  // Timer2 의 count 시작! 0으로 초기화 
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
    FND_cnt ++; 
    
    if ( FND_cnt >= 10000)
    { 
      FND_cnt = 0; 
    } 
    
    // clear interrupt flag
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  }
}


void TIM3_Configuration(int interval_ms)
{
  // TIM3에 clock 공급
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
  // 구조체 변수 선언 
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  //Prescaler , Period  값 설정 
  TIM_TimeBaseStructure.TIM_Prescaler = 26880 - 1;
  TIM_TimeBaseStructure.TIM_Period = interval_ms - 1;
  
  // up 모드 지정 
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  
  // 설정 저장 
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  // 
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  
  // TIM3 시작! 
  TIM_Cmd(TIM3, ENABLE);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
void TIM3_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    GPIO_ToggleBits(GPIOA, GPIO_Pin_5);

    // clear interrupt flag
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  }
}

int main()
{
  LED_init();
  FND_Init();
  SW_init();
  TIM3_Configuration(500); 
  TIM2_Configuration(1000);
  
  EXTI13_Configuration();
  
  
  while(1)
  {
    Count_Progress(FND_cnt/1000, ((FND_cnt/100)%10), ((FND_cnt/10)%10), (FND_cnt%10));
  }
  return 0;
}
