Here i mentioning How to calculate baudrate for STM32 boards

Formula For Baudrate:

The baud rate for the receiver and transmitter (Rx and Tx) are both set to the same value as 
programmed in the Mantissa and Fraction values of USARTDIV.

Equation 1: Baud rate for standard USART

  Tx/Rx Baudrate=frequency / (8*2-OVER8*USARTDIV);

Examples:

Example 1:

If DIV_Mantissa = 0x27 and DIV_Fraction[2:0]= 0d6 (USART_BRR = 0x1B6), then
Mantissa (USARTDIV) = 0d27
Fraction (USARTDIV) = 6/8 = 0d0.75
Therefore USARTDIV = 0d27.75
Example 2: 

To program USARTDIV = 0d25.62
This leads to:
DIV_Fraction = 8*0d0.62 = 0d4.96
The nearest real number is 0d5 = 0x5
DIV_Mantissa = mantissa (0d25.620) = 0d25 = 0x19
Then, USART_BRR = 0x195 => USARTDIV = 0d25.625


Now iam calculating baudrate for 9600 on my stm32F411RE Board

Actualyy USART2 in my board that is getting Clock from APB! bus thai is 50 MHZ

According my formula=  5000000 / ((8 * 2 - 1)* 9600

                    =32.552

         fraction part=16*0.552
                      =8.332 = Nearest real number is 9

         mantissa=32

 so final inserting value for BRR ragister is 0x209.
