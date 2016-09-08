#include "stm32f10x.h"
#include "systick.h"
#include "usart.h"
#include "pwm.h"
#include "control.h"
#include "CSB.h"
#include "NRF24l01.h"

void NrfRecv(void);
void UsartPlay(void);

int main(void)
{	
	SysTick_init();
	USART1_Init(115200);
	MotoPwm_Init();
	MotoEnablePin_Init();
	MotoCtrl(0,0);
	CSB_Init();
	NRF24L01_Init();
	NRF24L01_Check();
	RX_Mode();
	while(1)
	{
		Measure();
		if(Data_New)
		{
			Data_New=0;
			//printf("%.2f\n",Distance);
			//AutoRun(Distance);
		}
		UsartPlay();
		//NrfRecv();
	}
}

void NrfRecv(void)
{
	static uint32_t lastTm = 0;
	char buf[32];
	float pitch,roll,yaw;
	if(0 == NRF24L01_RxPacket((uint8_t*)buf))
	{
		if(sscanf(buf,"7Mt%6f%6f%6f",&pitch,&roll,&yaw) == 3)
		{
			CtrlRun(pitch, roll, yaw);
			lastTm = get_timer();
		}
	}
	if(lastTm + 600 < get_timer())
		MotoCtrl(0,0);
}

void UsartPlay(void)
{
	int newPack=0;
	unsigned char *pbuf;
	unsigned char cmd;
	int len;
	if (rxptr > 3) //数据接收
	{
			int i;
			for(i=0; i<rxptr-3; i++)
			{
					if (rxbuf[i] == 0xa5 && rxbuf[i + 1] == 0x5a)
					{
							len = rxbuf[i + 2];
							cmd = rxbuf[i + 3];
							pbuf = &rxbuf[i+4];
							newPack = 1;
							break;
					}
			}
			if(newPack==0)
			{
					rxptr = 0;
			}
	}
	if(newPack && (rxbuf+rxptr >= pbuf-4+len)) //数据接收完成
	{
		switch(cmd)
		{
			case 0x10:
				Stop();
				break;
			case 0x11:
				GoAhead();
				break;
			case 0x12:
				GoBack();
				break;
			case 0x13:
			{
				TurnLeft();
			}break;
			case 0x14:
			{
				TurnRight();
			}break;
			case 0x21:
				break;
			case 0x22:
			{
			}break;
		}
		rxptr=0;
	}
}
