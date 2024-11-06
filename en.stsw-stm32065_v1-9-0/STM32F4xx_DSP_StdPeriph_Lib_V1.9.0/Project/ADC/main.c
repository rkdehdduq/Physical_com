#include "stm32f4xx.h"
unsigned char Font[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xD8, 0x80, 0x90};
uint8_t ADC1_flag = 0;
uint32_t ADC1_data = 0;
uint32_t ADC1_voltage = 0;
uint32_t sum_adc_data = 0;
uint32_t mean_adc_data = 0;
uint32_t adc_cnt = 0;
#define WIN 1000
void Delay(__IO uint32_t nCount)
{
for(; nCount != 0; nCount--);
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

void FND_Init(void)
{
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |
GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
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

void Count_Progress(int d_3, int d_2, int d_1, int d_0)
{
GPIO_Write(GPIOB, 0x0008);
GPIO_Write(GPIOC, Font[d_0]<<1);
Delay(10000);
GPIO_Write(GPIOB, 0x0004);
GPIO_Write(GPIOC, Font[d_1]<<1);
Delay(10000);
GPIO_Write(GPIOB, 0x0002);
GPIO_Write(GPIOC, Font[d_2]<<1);
Delay(10000);
GPIO_Write(GPIOB, 0x0001);
GPIO_Write(GPIOC, Font[d_3]<<1);
Delay(10000);
}

uint32_t ADC_to_Voltage(uint16_t data)
{
ADC1_voltage = data *3300/4095;
return ADC1_voltage;
}

void ADC_Configuration(void)
{
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_Init(GPIOC, &GPIO_InitStructure);
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
ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;
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
ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_3Cycles);
ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
ADC_Cmd(ADC1, ENABLE);
}

void ADC_IRQHandler(void)
{
if (ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET)
{
ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
ADC1_data = ADC_GetConversionValue(ADC1);
sum_adc_data += ADC1_data;
adc_cnt++;
if (adc_cnt >= WIN)
{
GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
ADC1_flag = 1;
adc_cnt = 0;
mean_adc_data = sum_adc_data / WIN;
sum_adc_data = 0;
// ADC1_voltage = ADC_to_Voltage(mean_adc_data);
// Count_Progress(ADC1_voltage/1000, ((ADC1_voltage/100)%10), ((ADC1_voltage/10)%10), (ADC1_voltage%10));
}
}
}

int main()
{
ADC1_flag = 0;
ADC1_data = 0;
adc_cnt = 0;
mean_adc_data = 0;
sum_adc_data = 0;
LED_init();
FND_Init();
ADC_Configuration();
ADC_SoftwareStartConv(ADC1);
while(1)
{
if(ADC1_flag == 1)
{
ADC1_voltage = ADC_to_Voltage(mean_adc_data);
Count_Progress(ADC1_voltage/1000, ((ADC1_voltage/100)%10), ((ADC1_voltage/10)%10), (ADC1_voltage%10));
ADC1_flag = 0;
}
}
return 0;
}
