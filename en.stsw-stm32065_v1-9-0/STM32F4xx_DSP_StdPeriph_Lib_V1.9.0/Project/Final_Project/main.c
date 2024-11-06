#include "stm32f4xx.h"

int intensity = 10;
uint16_t ADC1_data = 0;
uint16_t test_send = 0b0000111100001111;
uint8_t u16_2_8 = 0;
uint16_t send_count = 0;
uint8_t GetADC_Array[20];
unsigned char getdata;
int degree =5;


void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
 }

unsigned char Font[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 
0x92, 0x82, 0xD8, 0x80, 0x90};

void Count_Progress( int d_2, int d_1 , int d_0)
{
  GPIO_Write(GPIOA, 0x8000);
  GPIO_Write(GPIOC, Font[d_0]);
  Delay(1000);
  GPIO_Write(GPIOA, 0x4000); 
  GPIO_Write(GPIOC, Font[d_1]);
  Delay(1000);
  GPIO_Write(GPIOA, 0x2000); 
  GPIO_Write(GPIOC, Font[d_2]);
  Delay(1000);
}

void Serial_Send(unsigned char t)
{
  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  USART_SendData(USART2, t);
}
unsigned char Serial_Receive(void)
{
  unsigned char data;
  while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
  data = USART_ReceiveData(USART2);
  return data;
}

// FND ���� (TIM2) 
void FND_Init(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 ;
  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
// TIM2 ����
void TIM2_Configuration(int interval_ms)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseStructure.TIM_Prescaler = 16000 - 1;
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
//TIM2 ���� �ڵ� ( �ɹڼ� ��� )
void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    Count_Progress( ((getdata/100)%10), ((getdata/10)%10), (getdata%10));

    // clear interrupt flag
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  }
}


// User switch ���� PC13 ( EXTI 13)
void SW_init(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
}
// 13����  interrupt ���� 
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
// 13������ interrupt ���� �ڵ� ( PC13 user switch ������ RGB_LED ��� ���� )
void EXTI15_10_IRQHandler(void) 
{
/* Make sure that interrupt flag is set */
  if (EXTI_GetITStatus(EXTI_Line13) != RESET) 
  {
   if (intensity > 25600)
  {
    intensity = 10;
  }
   if(getdata <= 60){
       TIM4->CCR1 = 0;
      TIM4->CCR2 = intensity;
      TIM4->CCR3 = 0;
      intensity *= 2;}
   
   
   if(getdata > 60 && getdata <=70) {
      TIM4->CCR1 = 0;
      TIM4->CCR2 = 0;
      TIM4->CCR3 = intensity;
     intensity*=2;
     
   }
   if(getdata >70) 
   {
      TIM4->CCR1 = intensity;
      TIM4->CCR2 = 0;
      TIM4->CCR3 = 0;
     intensity*= 2;}
   
  }
  /* Clear interrupt flag */
  EXTI_ClearITPendingBit(EXTI_Line13);
}


// TIM4 ����
void TIM4_Configuration(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Prescaler = 2837 - 1; 
  TIM_TimeBaseStructure.TIM_Period = 189 - 1; 
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  TIM_Cmd(TIM4, ENABLE);
}
// TIM4  PWM ����
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
  
  
  
  
}
// RGB_LED���� (PB 6, 7, 8) ( TIM4)
void RGB_LED_init(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
 
}
//TIM3 ���� 
void TIM3_Configuration(int interval_ms)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseStructure.TIM_Prescaler = 26880 - 1;
  TIM_TimeBaseStructure.TIM_Period = interval_ms - 1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM3, ENABLE);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
// TIM3 ���� �ڵ� ( G <60 < B< 70 < R <80)
void TIM3_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    //G
    if(getdata <= 80){
       TIM4->CCR1 = 0;
      TIM4->CCR2 = intensity;
      TIM4->CCR3 = 0;
    }
    //B
    else if (getdata > 80 && getdata <=87) {
      TIM4->CCR1 = 0;
      TIM4->CCR2 = 0;
      TIM4->CCR3 = intensity;
    }
    //R
    else {
      TIM4->CCR1 = intensity;
      TIM4->CCR2 = 0;
      TIM4->CCR3 = 0;
    }
    
    
    // clear interrupt flag
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  }
}


//�ɹڼ� 
void TIM5_Configuration(int intervalms)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseStructure.TIM_Prescaler = 16000 - 1;
  TIM_TimeBaseStructure.TIM_Period = intervalms - 1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
  TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM5, ENABLE);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
void TIM5_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
  { 
    
    GetADC_Array[send_count] = (uint8_t) (ADC1_data>>8);
    send_count++;
    GetADC_Array[send_count] = (uint8_t) (ADC1_data);
    send_count++;
    if (send_count == 20)
    { 
     Serial_Send(0xA5);
     for(int i = 0; i < 20; i++)
     {
       Serial_Send(GetADC_Array[i]);
     }
     Serial_Send(0x5A);
     send_count = 0;
    }
   
    // clear interrupt flag
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
   }
}

void USART2_Configuration(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  USART_Cmd(USART2, ENABLE);
}
void ADC_Configuration(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8; // �����ؼ� ���ÿ�
  ADC_CommonInit(&ADC_CommonInitStructure);
  ADC_InitTypeDef ADC_InitStructure;
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_3Cycles);
  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
  ADC_Cmd(ADC1, ENABLE);
}

void ADC_IRQHandler(void)
{
  if (ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET)
  {
     
     
    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
    ADC1_data = ADC_GetConversionValue(ADC1); 
   
  
  }
}



int main()
{
  SW_init();
  FND_Init();
  RGB_LED_init();
  
  

  
  TIM2_Configuration(2);
  TIM4_Configuration();
  PWM_TIM4_Configuration();
  TIM3_Configuration(1000);
   
  TIM5_Configuration(10);
   USART2_Configuration();
  ADC_Configuration();
  ADC_SoftwareStartConv(ADC1);
  
  
  EXTI13_Configuration();
  
  
  while(1)
  {
     getdata = Serial_Receive();
   
  }
  return 0;
}

  