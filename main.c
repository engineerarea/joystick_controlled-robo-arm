#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dma.h"              // Keil::Device:StdPeriph Drivers:DMA

uint32_t adcValue=0;
uint32_t MapValue1=0;
uint32_t MapValue2=0;


void GpioConfig(){

GPIO_InitTypeDef  gpioInits;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	gpioInits.GPIO_Mode=GPIO_Mode_AIN;
	gpioInits.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOA,&gpioInits);
	
	gpioInits.GPIO_Mode=GPIO_Mode_AIN;
	gpioInits.GPIO_Pin=GPIO_Pin_1;
	GPIO_Init(GPIOA,&gpioInits);
	
	gpioInits.GPIO_Mode=GPIO_Mode_AF_PP;
	gpioInits.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	gpioInits.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC,&gpioInits);

}

void AdcConfig(){
	
ADC_InitTypeDef  AdcInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	AdcInitStructure.ADC_ContinuousConvMode=ENABLE;
	AdcInitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	AdcInitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	AdcInitStructure.ADC_Mode=ADC_Mode_Independent;
	AdcInitStructure.ADC_NbrOfChannel=2;
	AdcInitStructure.ADC_ScanConvMode=ENABLE;
	
	ADC_Init(ADC1,&AdcInitStructure);
	ADC_DMACmd(ADC1,ENABLE);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_41Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);

}
void DmaConfig(){
	DMA_InitTypeDef  DmaInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	DMA_Cmd(DMA1_Channel1,DISABLE);
  DMA_DeInit(DMA1_Channel1);
	
	DmaInitStructure.DMA_BufferSize=2;
	DmaInitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	DmaInitStructure.DMA_M2M=DISABLE;
	DmaInitStructure.DMA_MemoryBaseAddr=(uint32_t)adcValue;
	DmaInitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	DmaInitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DmaInitStructure.DMA_Mode=DMA_Mode_Circular;
	DmaInitStructure.DMA_PeripheralBaseAddr=(uint32_t) & ADC1->DR;
	DmaInitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	DmaInitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DmaInitStructure.DMA_Priority=DMA_Priority_High;
	
	DMA_Init(DMA1_Channel1,&DmaInitStructure);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	

}

void TimeConfig(){
	
	
	TIM_TimeBaseInitTypeDef TimeInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TimeInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TimeInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TimeInitStructure.TIM_Period=4799;
	TimeInitStructure.TIM_Prescaler=100;
	TimeInitStructure.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(TIM3,&TimeInitStructure);
	TIM_Cmd(TIM3,ENABLE);

}

void PwmConfig1(uint32_t TimePulse){
	
TIM_OCInitTypeDef TimeStructure;
	
TimeStructure.TIM_OCMode=TIM_OCMode_PWM1;
TimeStructure.TIM_OCPolarity=TIM_OCPolarity_High;
TimeStructure.TIM_OutputState=TIM_OutputState_Enable;
TimeStructure.TIM_Pulse=TimePulse;
	
TIM_OC1Init(TIM3,&TimeStructure);
TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
	
	
}
void PwmConfig2(uint32_t TimePulse){
	
TIM_OCInitTypeDef TimeStructure;
	
TimeStructure.TIM_OCMode=TIM_OCMode_PWM1;
TimeStructure.TIM_OCPolarity=TIM_OCPolarity_High;
TimeStructure.TIM_OutputState=TIM_OutputState_Enable;
TimeStructure.TIM_Pulse=TimePulse;
	
TIM_OC2Init(TIM3,&TimeStructure);
TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	
}
float map(float AdcValue,float ConMax,float ConMin,float max,float min){
	
	
return adcValue*((ConMax-ConMin)/(max-min));

}
void delayUs(uint32_t time){

	uint32_t NewTime=24*time;
	while(NewTime--);

}

int main(){
	
GpioConfig();
AdcConfig();
	DmaConfig();
	TimeConfig();

while(1){

	/////Servo1////
	MapValue1=map(adcValue,480,240,4030,0);
	PwmConfig1(MapValue1);
	delayUs(36000);

////////////////////////////////////////////////
	
	/////Servo2////
	MapValue2=map(adcValue,480,240,4030,0);
	PwmConfig2(MapValue2);
	delayUs(36000);
	
////////////////////////////////////////////////
	
}



}