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
�������ܣ����ڳ�ʼ������
****************************************************/
void USART1_Init(uint32_t Baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_USART1, ENABLE);    

	/*�ȳ�ʼ��PA9��TX����PA10��RX�������˿�*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*���ô���1*/ 
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
#pragma import(__use_no_semihosting)   //û��ʵ��fgetcʱ��Ҫ�����ò���          
/* ��׼����Ҫ��֧�ֺ��� ʹ��printf()���Դ�ӡ����Ҫʵ�ָú��� */               
struct __FILE 
{ 
	int handle; 
    /* Whatever you require here. If the only file you are using is */    
    /* standard output using printf() for debugging, no file handling */    
    /* is required. */
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
/* �ض���fputc���� ���ʹ��MicroLIBֻ��Ҫ�ض���fputc�������� */  
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
����ֱ��ʹ��putchar
����Ҫ�ٶ��� int putchar(int ch)����Ϊstdio.h�������¶���
 #define putchar(c) putc(c, stdout)
*/

int ferror(FILE *f) {  
    /* Your implementation of ferror */  
    return EOF;  
} 
#endif 


#ifdef  USE_FULL_ASSERT
// ��Ҫ�ڹ�������Option(��ݼ�ALT+F7)C++����ҳ��define������"USE_FULL_ASSERT"
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









