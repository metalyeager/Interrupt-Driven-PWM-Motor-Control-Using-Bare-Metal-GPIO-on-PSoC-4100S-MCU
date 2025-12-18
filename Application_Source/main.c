#include "user_cfg.h"

/* Timer period */
#define timer_period   10000U
uint8_t app_heap[512] __attribute__((section (".heap")));
uint8_t app_stack[4096] __attribute__((section (".stack")));
uint8_t invertFLAG = 0;
void Delay1(uint32_t delayNumber);
int main()
{  
    /* HF CLOCK divider init*/
    Cy_SysClk_ClkHfSetDivider(0u); //0 - No Divider, 1 - DIV by 2, 2 = DIV by 4, 3 = DIV by 8
     
    enable_irq();
    /*GPIO pin init*/
    *((uint32_t *)0x40040100) = (1 << 2); // Set default output value of P1.2 to 1 GPIO_PRT1_DR
    *((uint32_t *)0x40040108) = (6 << 6); // Set drive mode of P1.2 to Digital OP Push Pull GPIO_PRT1_PC
    *((uint32_t *)0x40020100) = (8 << 8); // Assign GPIO to PWM output by selecting the MUX in HSIOM_PORT_SEL1 Register
    *((uint32_t *)0x40040500) = (1 << 3)|(1 << 1);// default values for 5.3 and 5.1
    *((uint32_t *)0x40040508) = (6 << 3)|(6 << 9);// config teh 5.1 and 5.3 as digital op push pull

    //interrupt enabling
    *((uint32_t *) 0x40040308) = *((uint32_t *)0x40040308)|(2 << 21);//PC config for 3.7 as dig ip
    *((uint32_t *) 0x40040300) |= (1 << 7);//setting the DR for 3.7
    *((uint32_t *)0x4004030C) |= (2 << 14);
    *((uint32_t *)0xE000E400) |= (3 << 30);

    //nvic clearing and enabling
    *((uint32_t *)0xE000E280) = 0xFFFFFFFF;//icpr
    *((uint32_t *)0xE000E100) |= (1 << 3);//iser

    /* Peripheral clock initializatio*/
    init_peri_Clock_Config();

    /*TIMER 3 - PWM - INIT*/
    
    *((uint32_t *)0x40200000) &= ~(1<< 3); //Disable Timer 3  in TCPWM_CTRL Register

    *((uint32_t *)0x402001C8) = 0; //Clear the counter register of  TCPWM3 TCPWM_CNT3_COUNTER Register

    *((uint32_t *)0x402001E8) =0x31 ; //Setting Line out behaviour in CC_MATCH_MODE and OVERFLOW_MODE in  TCPWM3 TCPWM_CNT3_TR_CTRL2 Register

    *((uint32_t *)0x402001D4)  = (timer_period-1); //Set the Period Register of TCPWM3 TCPWM_CNT3_PERIOD Register

    *((uint32_t *)0x402001CC) =  (25000-1) ; //Compare register TCPWM3, TCPWM_CNT3_CC Regsiter 

    *((uint32_t *)0x402001C0) |= (4 << 24) | (1 << 3); //Mode (PWM Mode) configuration and PWM_STOP_ON_KILL of for TCPWM3, TCPWM_CNT3_CTRL Regsiter 
    
    *((uint32_t *)0x40200000) |=  (1<< 3); //Enable Timer 3  in TCPWM_CTRL Register

	*((uint32_t *)0x40200008) |= (1 << 11); //Reload trigger in TCPWM_CMD Register

    enable_irq();
    for(;;)
    {
        
    }

    return 0;
}
void Delay1(uint32_t delayNumber)
{
    for(volatile uint32_t i=0; i<delayNumber; i++);
    for( volatile uint32_t i=0; i<delayNumber; i++);
}
/*Peripheral clock initilizations*/
void init_peri_Clock_Config()
{
    //TIMER 3 PWM- CLOCK
    *((uint32_t *)0x40010000) = (1<<30); // Disable the Divider using PERI_DIV_CMD 

    *((uint32_t *)0x40010300) = (240 - 1) << 8 ; //Set the divider value in PERI_DIV_16_CTL0, We are configuring Divider 0 

    *((uint32_t *)0x40010000) |= (1<<31) |(3<<14) |(63<<8) |(1<<6) ; //PERI_DIV_CMD 
    //Enable the divder 31:bit, Keep 3 at 15:14 and 63 13:8 this selects the HFCLK as reference , Select 16 bit divider 7:6, and Select the divider no 0 using 5:0 so not writing 0 bit

    *((uint32_t *)0x40010124) = (1<<6); // Specify Divider type 7:6 and Selected Divider 3:0 (Divider 0) in register PERI_PCLK_CTL9 TCPWM2 is PERIPHERAL 9

}
void ioss_interrupts_gpio_3_IRQHandler(void)
{
 uint32_t readval;
 *((uint32_t *)0x40040310) = (1 << 7);
 readval=*((uint32_t *) 0x40040304);
 if(((readval >> 7) &1) == 0u)
 { 
    *((uint32_t *)0x402001CC) = 0u; // setting the  duty cycle to 0%
    Delay1(1000000);

    if(invertFLAG == 0u)
    {
        *((uint32_t *)0x40040500) = (0 << 3)|(1 << 1);
        invertFLAG=1u;
        
    }
    else if(invertFLAG == 1u)
    {
        *((uint32_t *)0x40040500) = (1 << 3)|(0 << 1);
        invertFLAG=0u;
        
    }
    Delay1(1000000);
    *((uint32_t*)0x402001CC)=5000u;// 50% duty cycle
  
}
}
