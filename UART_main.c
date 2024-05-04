#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>


 void uart_writechar(char);
 char uart_readchar();
 void uart_sendstring(char*);
 char * uart_readstring(char *,char);
 void UART_init(void);


unsigned int *PAHB1ENR       = (unsigned int*)0x40023830;
unsigned int *APB1ENR        = (unsigned int*)0x40023840;
unsigned int *GPIOMODER      = (unsigned int*)0x40020000;
unsigned int *GPIOOODR       = (unsigned int*)0x40020014;
unsigned int *GPIOAFRL       = (unsigned int*)0x40020020;
unsigned int *USART_STA      = (unsigned int*)0x40004400;
unsigned int *USART_DATA     = (unsigned int*)0x40004404;
unsigned int *USART_BRR      = (unsigned int*)0x40004408;
unsigned int *USART_CONTROL  = (unsigned int*)0x4000440C;



void uart_sendstring(char *string)
{
	while(*string)
	{
		uart_writechar(*string);
		string++;
	}
}


char * uart_readstring(char *string,char length)
{
	char i;
	for(i=0;i<length;i++)
	{
		while (!(*USART_STA & (1<<5)));  // Wait for RXNE to SET.. This indicates that the data has been Received
		string[i]=*USART_DATA;  // Read the data.
		if(string[i]=='\n')
			break;
	}
	string[i]='\0';  //putting Null character at the end
}


 void uart_writechar(char ch)
{

		/*1. Write the data to send in the USART_DR register (this clears the TXE bit). Repeat this
			 for each data to be transmitted in case of single buffer.
		2. After writing the last data into the USART_DR register, wait until TC=1. This indicates
			 that the transmission of the last frame is complete. This is required for instance when
			 the USART is disabled or enters the Halt mode to avoid corrupting the last transmission.*/

			 *USART_DATA = ch;   // LOad the Data
		     while(!(*USART_STA&0x0040));  // Wait for TC to SET.. This indicates that the data has been transmitted


}

char uart_readchar()
{
			/*********** STEPS FOLLOWED *************

		1. Wait for the RXNE bit to set. It indicates that the data has been received and can be read.
		2. Read the data from USART_DR  Register. This also clears the RXNE bit

		****************************************/
		char Temp;

		while (!(*USART_STA & (1<<5)));  // Wait for RXNE to SET.. This indicates that the data has been Received
		Temp =*USART_DATA;  // Read the data.
		return Temp;

}


void UART_init()
{
	//Enable clock access for GPIOA
	*PAHB1ENR |= (1<<0);
	//Enable clock access for USART2
	*APB1ENR |= (1<<17);
	//Configure the GPIO Mode register to alt function
	//set PA2 to ALT FUNC
	*GPIOMODER &=~(1<<4);
    *GPIOMODER |=(1<<5);

     //set PA3 to ALT FUNC
     *GPIOMODER &=~(1<<6);
     *GPIOMODER |=(1<<7);

	//Configure the GPIO to o/p function
     //set PA5 as output
     *GPIOMODER |=(1<<10);
     *GPIOMODER &=~(1<<11);

	//Configure the ALT Function to AFRL For PA2 and PA3
     //PA2
     *GPIOAFRL|=(1<<8);
     *GPIOAFRL |=(1<<9);
     *GPIOAFRL |=(1<<10);
     *GPIOAFRL &=~(1<<11);
     //PA3
     *GPIOAFRL|=(1<<12);
     *GPIOAFRL |=(1<<13);
     *GPIOAFRL |=(1<<14);
     *GPIOAFRL &=~(1<<15);


	//Configure the BRR TO 9600

	 *USART_BRR |=((9<<0)|(20<<4));


	//Configure the UART for tx and rx
	 //Tx Enable bit
	 *USART_CONTROL =0x0008;
	 //Rx enable bit
	 *USART_CONTROL =0x0004;
	 //USART enable bit
	 *USART_CONTROL =0x2000;
}

void delay_ms(int v)
{
	int i=39;
	while(v)
	{
		while(--i);
	}
}


int main(void)
{

UART_init();

while(1)
{
	char ch='a';
	char *string="hello world";
	char receive[15];

	//sending char
	uart_writechar(ch);
	delay_ms(1000);
	ch=uart_readchar();

	delay_ms(1000);

	//sending string
	uart_sendstring(string);
	delay_ms(1000);
	string=uart_readstring(string,15);
}
return 0;
}
