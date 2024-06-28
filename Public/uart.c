//不会用到这个文件的代码，把它注释掉了
/*#include "uart.h"

#define RELOAD_COUNT 0xFA //宏定义波特率发生器的载入值 9600

void UART_Init(void)
{
	SCON=0X50;			//设置为工作方式1
	TMOD=0X20;			//设置计数器工作方式2
	PCON=0X80;			//波特率加倍
	TH1=RELOAD_COUNT;	//计数器初始值设置
	TL1=TH1;
	//ES=1;				//关闭接收中断
	EA=1;				//打开总中断
	TR1=1;				//打开计数器
	TI=1;          //发送中断标记位，如果使用printf函数的必须设置	
}

void UART_Cmd(FunctionalState ensta)
{
	ES=ensta;	
}

void UART_SendData(u8 dat)
{
	ES=0; //关闭串口中断
	TI=0; //清发送完毕中断请求标志位
	SBUF=dat; //发送
	while(TI==0); //等待发送完毕
	TI=0; //清发送完毕中断请求标志位
	ES=1; //允许串口中断
}

void UART_SendString(u8 *pbuf)
{
	while(*pbuf!='\0') //遇到空格跳出循环	
	{
		UART_SendData(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
}

u8 UART_RX_BUF[UART_REC_LEN];     //接收缓冲,最大UART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 UART_RX_STA=0;       //接收状态标记	


void UART_IRQn() interrupt 4
{
	u8 r;
	
	if(RI)
	{
		RI=0;
		r=SBUF;//读取接收到的数据
		if((UART_RX_STA&0x8000)==0)//接收未完成
		{
			if(UART_RX_STA&0x4000)//接收到了0x0d
			{
				if(r!=0x0a)UART_RX_STA=0;//接收错误,重新开始
				else UART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(r==0x0d)UART_RX_STA|=0x4000;
				else
				{
					UART_RX_BUF[UART_RX_STA&0X3FFF]=r ;
					UART_RX_STA++;
					if(UART_RX_STA>(UART_REC_LEN-1))UART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		
	}	
}

*/