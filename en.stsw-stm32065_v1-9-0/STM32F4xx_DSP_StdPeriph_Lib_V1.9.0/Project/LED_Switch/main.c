#include "stm32f4xx.h"
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

//led 설정 
  // c번포트 , out mode , 0~ 7번 , out type = push-pull , no pull
void LED_init(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
}

// user 버튼 설정 
  // c의 13번 포트 , 입력 mode , no pull 
void SW_init(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
}

//// user switch 누르면 다 켜지는 프로그램
//int main()
//{
//  LED_init();
//  SW_init();
//  
//  while(1)
//  {
//    // 스위치 누르면(0) : 켬 
//    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 1)
//      GPIO_Write(GPIOC, 0x00FF);
//    else
//      GPIO_Write(GPIOC, 0x0000);
//  }
//
//  return 0;
//} 

//// user switch 누르면 하나씩 켜지는 코드 
//int main()
//{
//  LED_init();
//  SW_init();
//  
//  int led = 0;
//  int sw_new = 1;
//  int sw_old = 1;
//  
//  while(1)
//  { 
//    sw_new = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
//    
//    // 눌렸다 떨어질때 action
//    if( (sw_old == 0) & (sw_new == 1) ) 
//    {
//      if(led == 0)
//      {
//        led++;
//      }
//      else if(led == 128)
//      {
//        led = 0;
//      }
//      else
//      {
//        led = led << 1;
//      }
//    }
//    
//    sw_old = sw_new;
//    GPIO_Write(GPIOC, led);
//  }

//// 외부 스위치 사용 (pc_12번 포트 사용) + 눌렀을때 led 변화 
  int main()
{
  LED_init();
  SW_init();
  
  int led = 0;
  int sw_new = 1;
  int sw_old = 1;
  
  while(1)
  { 
    sw_new = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12);
    // 상태가 같지않으면 딜레이 ? 
    if (sw_old != sw_new) 
    { 
      Delay(10000) ;
      sw_new = GPIO_ReadInputDataBit(GPIOC , GPIO_Pin_12);
    }
    
    // 눌렸다 떨어질때 action
    if( (sw_old == 1) & (sw_new == 0) ) 
    {
      if(led == 0)
      {
        led++;
      }
      else if(led == 128)
      {
        led = 0;
      }
      else
      {
        led = led << 1;
      }
    }
    
    sw_old = sw_new;
    GPIO_Write(GPIOC, led);
  }

  return 0;
}

