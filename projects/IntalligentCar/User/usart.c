#include "usart.h"

uint8_t rxbuf[64];
uint16_t rxptr = 0;

void USART1_NVIC_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*****************************************************
函数功能：串口初始化函数
****************************************************/
void USART1_Init(uint32_t Baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_USART1, ENABLE);    

	/*先初始化PA9（TX）和PA10（RX）两个端口*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*配置串口1*/ 
    USART_InitStructure.USART_BaudRate = Baud; 
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 
	USART_InitStructure.USART_Parity = USART_Parity_No; 
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None ; 
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
	
	
	USART1_NVIC_Init();		
	USART_ClearFlag(USART1, USART_IT_RXNE); 
	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);	
}


#ifndef MicroLIB
#pragma import(__use_no_semihosting)   //没有实现fgetc时需要声明该参数          
/* 标准库需要的支持函数 使用printf()调试打印不需要实现该函数 */               
struct __FILE 
{ 
	int handle; 
    /* Whatever you require here. If the only file you are using is */    
    /* standard output using printf() for debugging, no file handling */    
    /* is required. */
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
/* 重定义fputc函数 如果使用MicroLIB只需要重定义fputc函数即可 */  
int fputc(int ch, FILE *f)
{
    /* Place your implementation of fputc here */
    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {}

    /* e.g. write a character to the USART */
    USART_SendData(USART1, (uint8_t) ch);

    return ch;
}
/*
可以直接使用putchar
不需要再定义 int putchar(int ch)，因为stdio.h中有如下定义
 #define putchar(c) putc(c, stdout)
*/

int ferror(FILE *f) {  
    /* Your implementation of ferror */  
    return EOF;  
} 
#endif 


#ifdef  USE_FULL_ASSERT
// 需要在工程设置Option(快捷键ALT+F7)C++属性页的define栏输入"USE_FULL_ASSERT"
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
     
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_TC) == SET)
	{

	}
  /* USART in Receiver mode */
  if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
  {
    if (rxptr < 64-1)
    {
      /* Receive Transaction data */
			rxbuf[rxptr++] = USART_ReceiveData(USART1);
    }
    else
    {
      /* Disable the Rx buffer not empty interrupt */
    }
  }
}









