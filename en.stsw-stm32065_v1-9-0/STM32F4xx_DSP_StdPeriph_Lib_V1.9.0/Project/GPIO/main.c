
#include "stm32f4xx.h"

void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

// user_LED(PA_5) 제어 프로그램 
//int main()
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//  
//  while(1)
//  {
//    GPIO_SetBits(GPIOA, GPIO_Pin_5);
//    Delay(5000000);
//    GPIO_ResetBits(GPIOA, GPIO_Pin_5);
//    Delay(5000000);
//  }
//  return 0;
//}
//----------------------------------------------------------


// 8개 LED 제어프로그램
int main()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  int led =0; 
  
  while(1)
  {
    // 8개 다 on-off 됨 
//    GPIO_Write(GPIOC, 0x0000);
//    Delay(5000000);
//    GPIO_Write(GPIOC, 0x00FF);
//    Delay(5000000);
    
    
    //8개가 순차적으로 켜지게 하기
//    GPIO_Write(GPIOC, GPIO_Pin_0);
//    Delay(5000000);
//    GPIO_Write(GPIOC, GPIO_Pin_1);
//    Delay(5000000);
//    GPIO_Write(GPIOC, GPIO_Pin_2);
//    Delay(5000000);
//    GPIO_Write(GPIOC, GPIO_Pin_3);
//    Delay(5000000);
//    GPIO_Write(GPIOC, GPIO_Pin_4);
//    Delay(5000000);
//    GPIO_Write(GPIOC, GPIO_Pin_5);
//    Delay(5000000);
//    GPIO_Write(GPIOC, GPIO_Pin_6);
//    Delay(5000000);
//    GPIO_Write(GPIOC, GPIO_Pin_7);
//    Delay(5000000);
     
    
    // 변수 사용해서 순차적으로 하나씩 켜지게 하기
    // 실행시키면 이상함  
//    GPIO_Write(GPIOC, led);
//    Delay(500000);
//    led++;
//    if(led > 256)
//      led = 0;
    
    
    //4개씩 왔다 갔다 켜지게 하기 
//    GPIO_Write(GPIOC, 0x000F);
//    Delay(5000000);
//    GPIO_Write(GPIOC, 0x00F0);
//    Delay(5000000);
    
    
    // 뭐 그림 대로 켜기 
//    GPIO_Write(GPIOC, 0x0000);
//    Delay(5000000);
//    GPIO_Write(GPIOC, 0x0081);
//    Delay(5000000);
//    GPIO_Write(GPIOC, 0x00C3);
//    Delay(5000000);
//    GPIO_Write(GPIOC, 0x00E7);
//    Delay(5000000);
//    GPIO_Write(GPIOC, 0x00FF);
//    Delay(5000000);
  }
  return 0;
}

