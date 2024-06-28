#ifndef _touch_H
#define _touch_H


#include "public.h"


//---定义使用的IO口---//
sbit TOUCH_DOUT = P2^0;	  //输出
sbit TOUCH_CLK  = P2^1;	  //时钟
sbit TOUCH_DIN  = P2^2;	  //输入
sbit TOUCH_CS   = P2^3;	  //片选
sbit TOUCH_PEN  = P2^4;	  //检测触摸屏响应信号


//---定义芯片命令字节---//
#define XPT_CMD_X	0xD0    //读取X轴的命令
#define XPT_CMD_Y	0x90	//读取Y轴的命令

//---定义数据类型---//
typedef struct 
{
	u16 x;	 //对应XPT2046读取的AD值
	u16 y;

	u32 lcdx;//对应TFTLCD实际坐标值
	u32 lcdy;

	u8 sta;//触摸状态

} XPT_XY;

extern XPT_XY xpt_xy;

//---声明全局函数---//
void TOUCH_SPI_Start(void);
void TOUCH_SPI_Write(u8 dat);
u16 TOUCH_SPI_Read(void);
u16 TOUCH_XPT_ReadData(u8 cmd);
u8 TOUCH_XPT_ReadXY(void);
u8 TOUCH_Scan(void);

#endif
