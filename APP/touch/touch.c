//不会用到这个文件的代码，把它注释掉了
/*#include "touch.h"
#include "tftlcd.h"
#include "uart.h"

XPT_XY xpt_xy;	 //定义一个全局变量保存X、Y的值



void TOUCH_SPI_Start(void)
{
	TOUCH_CLK = 0;
	TOUCH_CS  = 1;
	TOUCH_DIN = 1;
	TOUCH_CLK = 1;
	TOUCH_CS  = 0;		
}

void TOUCH_SPI_Write(u8 dat)
{
	u8 i;
	TOUCH_CLK = 0;
	for(i=0; i<8; i++)
	{
		TOUCH_DIN = dat >>7;  	//放置最高位
		dat <<= 1;
		TOUCH_CLK = 0;			//上升沿放置数据

		TOUCH_CLK = 1;

	}
}

u16 TOUCH_SPI_Read(void)
{
	u16 i, dat=0;
	TOUCH_CLK = 0;
	for(i=0; i<12; i++)		//接收12位数据
	{
		dat <<= 1;

		TOUCH_CLK = 1;
		TOUCH_CLK = 0;

		dat |= TOUCH_DOUT;

	}
	return dat;	
}

#define XY_READ_TIMS  10		   //读取的次数

u16 TOUCH_XPT_ReadData(u8 cmd)
{
	u8 i, j;
	u16 readValue[XY_READ_TIMS];
	long endValue;

	TOUCH_CLK = 0;    //先拉低时间
	TOUCH_CS  = 0;    //选中芯片
	
	for(i=0; i<XY_READ_TIMS; i++)	 	//读取XY_READ_TIMS次结果
	{	
		TOUCH_SPI_Write(cmd);  //发送转换命令
		//delay_10us();
		for(j=6; j>0; j--); 	//延时等待转换结果
		TOUCH_CLK = 1;	  //发送一个时钟周期，清除BUSY
		_nop_();
		_nop_();
		TOUCH_CLK = 0;
		_nop_();
		_nop_();
	
		readValue[i] = TOUCH_SPI_Read();
	}
	TOUCH_CS = 1; //释放片选

	//---软件滤波---//
	//---先大到小排序，除去最高值，除去最低值，求其平均值---//
   	for(i=0; i<XY_READ_TIMS - 1; i++)	//从大到小排序
	{
		for(j= i+1; j<XY_READ_TIMS; j++)
		{
			if(readValue[i] < readValue[j])
			{
				endValue = readValue[i];
				readValue[i] = readValue[j];
				readValue[j] = endValue;
			}  
		}
	}
//	if((readValue[2] - readValue[3]) > 5)
//	{
//		return 0;
//	}
	endValue = 0;
	for(i=2; i<XY_READ_TIMS-2; i++)		   
	{
		endValue += readValue[i]; 
	}
	endValue = endValue/ (XY_READ_TIMS - 4);//求平均值

	return endValue;
}



u8 TOUCH_XPT_ReadXY(void)
{	
	u16 x1, x2, x, y1, y2, y;

	TOUCH_SPI_Start();
	//---分别读两次X值和Y值, 交叉着读可以提高一些读取精度---//
	x1 = TOUCH_XPT_ReadData(XPT_CMD_X);
	y1 = TOUCH_XPT_ReadData(XPT_CMD_Y);
	x2 = TOUCH_XPT_ReadData(XPT_CMD_X);
	y2 = TOUCH_XPT_ReadData(XPT_CMD_Y);	


	//---求取X值的差值---//
	if (x1 > x2)
	{
		x = x1 - x2;
	}
	else
	{
		x = x2 - x1;
	}

	//---求取Y值的差值---//
	if (y1 > y2)
	{
		y = y1 - y2;
	}
	else
	{
		y = y2 - y1;
	}

	//---判断差值是否大于50，大于就返回0，表示读取失败---//
	if((x > 50) || (y > 50))  
	{
		return 0;
	}

	//---求取两次读取值的平均数作为读取到的XY值---//
	xpt_xy.x = (x1 + x2) / 2;
	xpt_xy.y = (y1 + y2) / 2;

	xpt_xy.x &= 0xFFF0;	//去掉低四位
	xpt_xy.y &= 0xFFF0;
	 
	//---确定XY值的范围，用在触摸屏大于TFT时---//
	if((xpt_xy.x < 100) || (xpt_xy.y > 3800))
	{
		return 0;
	}

	return 1;  // 返回1，表示读取成功
}

//返回1：触摸按下
//0:无触摸
u8 TOUCH_Scan(void)
{
	u8 res=0;
	u32 temp;

	if(TOUCH_XPT_ReadXY())
	{
		//--如果触摸跟显示发生偏移，可以根据显示AD值--//
		//--调整下面公式里面的数值--//
		if(tftlcd_data.dir==0)
		{
#ifdef TFT20_HX8309
			xpt_xy.lcdx = xpt_xy.x;
			xpt_xy.lcdx = (xpt_xy.lcdx - 250) * 180 / 3700; 
			xpt_xy.lcdy =	xpt_xy.y;
			xpt_xy.lcdy = (xpt_xy.lcdy - 350) * 250 / 3500;
#endif

#ifdef TFT22_ILI9225B
			xpt_xy.lcdx = xpt_xy.x;
			xpt_xy.lcdx = (xpt_xy.lcdx - 250) * 180 / 3700; 
			xpt_xy.lcdy =	xpt_xy.y;
			xpt_xy.lcdy = (xpt_xy.lcdy - 350) * 250 / 3500;
#endif

#ifdef TFT22_R61503B
			xpt_xy.lcdx = xpt_xy.x;
			xpt_xy.lcdx = (xpt_xy.lcdx - 250) * 180 / 3700; 
			xpt_xy.lcdy =	xpt_xy.y;
			xpt_xy.lcdy = (xpt_xy.lcdy - 350) * 250 / 3500;
#endif

#ifdef TFT24_ST7781R
			xpt_xy.lcdx=xpt_xy.x;
			xpt_xy.lcdx=((xpt_xy.lcdx - 336)*240)/3328;
			xpt_xy.lcdy=xpt_xy.y; 
			xpt_xy.lcdy =((xpt_xy.lcdy - 240) *320)/3412;
#endif	
	
#ifdef TFT26_R61509V
			xpt_xy.lcdx=xpt_xy.x;
			xpt_xy.lcdx=((xpt_xy.lcdx - 240)*260)/3850;
			xpt_xy.lcdy=xpt_xy.y; 
			xpt_xy.lcdy =((xpt_xy.lcdy - 200)*420)/3950;
#endif

#ifdef TFT26_ILI9325D
			xpt_xy.lcdx=xpt_xy.x;
			xpt_xy.lcdx=((xpt_xy.lcdx - 600)*297)/3550;
			xpt_xy.lcdy=xpt_xy.y; 
			xpt_xy.lcdy =((xpt_xy.lcdy - 250)*362)/3850;
#endif

		}
		else
		{
#ifdef TFT20_HX8309
		xpt_xy.lcdx = 4096-xpt_xy.y;
		xpt_xy.lcdx = (xpt_xy.lcdx - 350) * 250 / 3500;
		xpt_xy.lcdy = xpt_xy.x;
		xpt_xy.lcdy = (xpt_xy.lcdy - 250) * 180 / 3700;
#endif

#ifdef TFT22_ILI9225B
		xpt_xy.lcdx = 4096-xpt_xy.y;
		xpt_xy.lcdx = (xpt_xy.lcdx - 350) * 250 / 3500;
		xpt_xy.lcdy = xpt_xy.x;
		xpt_xy.lcdy = (xpt_xy.lcdy - 250) * 180 / 3700;
#endif

#ifdef TFT22_R61503B
		xpt_xy.lcdx = 4096-xpt_xy.y;
		xpt_xy.lcdx = (xpt_xy.lcdx - 350) * 250 / 3500;
		xpt_xy.lcdy = xpt_xy.x;
		xpt_xy.lcdy = (xpt_xy.lcdy - 250) * 180 / 3700;
#endif

#ifdef TFT24_ST7781R
			xpt_xy.lcdx=xpt_xy.y;
			xpt_xy.lcdx =((xpt_xy.lcdx - 240) *320)/3412;
			xpt_xy.lcdy=4096-xpt_xy.x; 
			xpt_xy.lcdy=((xpt_xy.lcdy - 336)*240)/3328;
#endif

#ifdef TFT26_R61509V
			xpt_xy.lcdx=xpt_xy.y;
			xpt_xy.lcdx=((xpt_xy.lcdx - 200)*420)/3950;
			xpt_xy.lcdy=4096 - xpt_xy.x; 
			xpt_xy.lcdy =((xpt_xy.lcdy - 240)*260)/3850;
#endif	

#ifdef TFT26_ILI9325D
			xpt_xy.lcdx=xpt_xy.y;
			xpt_xy.lcdx=((xpt_xy.lcdx - 250)*362)/3850;
			xpt_xy.lcdy=4096 - xpt_xy.x; 
			xpt_xy.lcdy =((xpt_xy.lcdy - 600)*297)/3550;
#endif	
					
		}
 

//		if(tftlcd_data.dir==0)
//		{
//			xpt_xy.lcdx=xpt_xy.lcdx;
//			xpt_xy.lcdy=xpt_xy.lcdy;	
//		}
//		else
//		{		
//			temp=xpt_xy.lcdx;
//			xpt_xy.lcdx=xpt_xy.lcdy;
//			xpt_xy.lcdy=tftlcd_data.height-temp;
//		}

//		printf("ad_x=%d   ad_y=%d\r\n",xpt_xy.x,xpt_xy.y);
//		printf("lcdx=%d   lcdy=%d\r\n",xpt_xy.lcdx,xpt_xy.lcdy);
	   res=1;
	   xpt_xy.sta=1;
	}
	else
		xpt_xy.sta=0;	
	return res;	
}
*/
