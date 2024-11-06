#include "stm32f4xx.h"
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

//led ���� 
  // c����Ʈ , out mode , 0~ 7�� , out type = push-pull , no pull
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

// user ��ư ���� 
  // c�� 13�� ��Ʈ , �Է� mode , no pull 
void SW_init(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
}

//// user switch ������ �� ������ ���α׷�
//int main()
//{
//  LED_init();
//  SW_init();
//  
//  while(1)
//  {
//    // ����ġ ������(0) : �� 
//    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 1)
//      GPIO_Write(GPIOC, 0x00FF);
//    else
//      GPIO_Write(GPIOC, 0x0000);
//  }
//
//  return 0;
//} 

//// user switch ������ �ϳ��� ������ �ڵ� 
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
//    // ���ȴ� �������� action
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

//// �ܺ� ����ġ ��� (pc_12�� ��Ʈ ���) + �������� led ��ȭ 
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
    // ���°� ���������� ������ ? 
    if (sw_old != sw_new) 
    { 
      Delay(10000) ;
      sw_new = GPIO_ReadInputDataBit(GPIOC , GPIO_Pin_12);
    }
    
    // ���ȴ� �������� action
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

