/*************************************************************************************
实验现象：可以分别使用按键和手机发送数据这两种方式来控制led小灯，舵机和步进电机。
并且有一块TFT屏幕显示三种元件的状态。每按一下按键或发送一个数据，元件的状态就改变一次。

实验说明：实验中小灯的亮灭表示家电中“电灯”的亮灭，舵机旋转90度代表家具“门”的开关
步进电机的顺时针旋转代表窗帘的拉开，逆时针旋转代表窗帘拉上。

接线说明：
hex文件在output文件夹中
k8,k7,k6分别接P24,P23,P22。（按键部分）
P07与交通灯模块的D6连接（LED小灯部分）
舵机的VCC接开发板上5V,GND接GND。PWM线接P03（舵机部分）
步进电机的A+,A-,B+,B-分别接在J81的OA,OB,OC,OD。
IA,IB,IC,ID分别接P00,P01,P02,P03(步进电机部分)
蓝牙模块的VCC,GND分别与开发板上的5V和GND相连
TXD接P30,RXD接P31（蓝牙部分）

注意事项：
1，一次只能按下一个按键，不能2个或3个按键同时按下。
2，通过蓝牙，手机发送“01”时电灯状态改变，发送“02”时门的状态改变
发送“03”时，窗帘状态改变。如果发送别的，蓝牙小程序接收数据区会收到
“Error!”意为发送的数据有错误，需要重新发送。
*************************************************************************************/
#include "tftlcd.h"
#include "reg52.h"
#define SPEED 1900 //修改此值可改变电机旋转速度
sbit led1=P0^7;
sbit pwm=P0^6;
sbit MOTOA = P0^0;
sbit MOTOB = P0^1;
sbit MOTOC = P0^2;
sbit MOTOD = P0^3;
sbit key1=P2^4;//k8，电灯开关
sbit key2=P2^3;//k7，门开关
sbit key3=P2^2;//k6，窗帘开关
bit g1=1,g2=1,g3=1;//1代表电器状态未改变
bit flag1=1,flag2=1,flag3=1;/*设置3个变量存储电器当前状态，为1表示电器状态为关，为0为开*/
bit f=1;/*默认为1。它为1表示接收到了手机改变电器状态的数据。只有它为1或者有按键按下时
程序才会跳出keyout函数中的循环.否则程序停在那里等待按键按下或f变为1*/
char ascii[7]={"Error!"};//若手机发送的数据错误，就向手机发送这个数组里的
unsigned char i=0;//该变量读取手机发送的数据
unsigned char jd=1;//角度标识,默认门为关
unsigned char count=0;//0.5ms次数标识，最初次数为0
void FontShow_Init();//函数声明
void Time0_Init();
void initUART();
void keyout();
void tftFontChange();
void doorChange();
void clockwise();
void antiClockwise();
void curtainChange();
void main()
{
	TFTLCD_Init();//初始化屏幕
	FontShow_Init();//初始化屏幕显示
	Time0_Init();//打开定时器0
	initUART();//初始化串口	
  //以下几行为电器状态初始化
	led1=flag1;
	//jd=1;//这一步本来要的，因为在定义变量时完成，故可省略
	MOTOA=0;
	MOTOB=0;
	MOTOC=0;
	MOTOD=0;
	while(1)//不断检测，从而改变电器状态
	{		
		keyout();	
		tftFontChange();
		if(g1==0){led1=flag1;g1=1;}		
		if(g2==0){doorChange();	g2=1;}
		if(g3==0){curtainChange();g3=1;}
	}
}
void FontShow_Init()
{
	FRONT_COLOR=RED;
	LCD_ShowFontHZ(10,10,"电灯状态：关");
	LCD_ShowFontHZ(10,50,"门状态：关");
	LCD_ShowFontHZ(10,90,"窗帘状态：关");	
}
void keyout()
{
	unsigned int a;
	key1=1;
	key2=1;
	key3=1;
	while(key1==1&&key2==1&&key3==1&&f==1);//未按下则停在这
	if(f==0)f=1;
	if(key1==0||key2==0||key3==0)//key=0说明按键被按下了
	{
		for(a=0;a<1000;a++);//延时约10ms
		if(key1==0||key2==0||key3==0)//再次检测是否真的有按键按下
		{
			if(key1==0){flag1=~flag1;g1=0;}
			if(key2==0){flag2=~flag2;g2=0;}
			if(key3==0){flag3=~flag3;g3=0;}
		}
		while(!(key1==1&&key2==1&&key3==1));//只要全部按键电平都为1才可通过该循环
		for(a=0;a<1000;a++);//延时约10ms
		while(!(key1==1&&key2==1&&key3==1));//再次检测
	}
}
void tftFontChange()
{
	if(g1==0)
	{
	  if(flag1==1)LCD_ShowFontHZ(170,10,"关");//开关这2个字的横坐标在170
	  else LCD_ShowFontHZ(170,10,"开");//一个字宽32，32*5+10=170
	}
	if(g2==0)
	{
	  if(flag2==1)LCD_ShowFontHZ(138,50,"关");//门状态的在138，与上同理
	  else LCD_ShowFontHZ(138,50,"开");
	}
	if(g3==0)
	{
	  if(flag3==1)LCD_ShowFontHZ(170,90,"关");	
	  else LCD_ShowFontHZ(170,90,"开");		
	}
}
void Time0_Init()          //定时器初始化
{
	TMOD=0X21;//定时器0为0001，定时器1为0010。TMOD不能位寻址，所以在这里一次性赋值。   
	EA=1;//打开总中断
	ET0=1;//打开定时器0的中断
	TH0  = 0xfe;
	TL0  = 0x33; //11.0592MZ晶振，0.5ms
	TR0=1; //定时器开始
}

void initUART()//波特率9600
{
	SCON=0x50;
	TH1=0XFA;
	TL1=TH1;
	PCON=0X80;//通过这个控制SMOD位为1
	ES=1;
	//EA=1;总中断在之前的函数Time0_Init()中已打开，所以这行注释掉
	ET1=0;
	TR1=1;
}
void doorChange()
{
	  if(flag2==1){jd=1;count=0;}//flag2只有0，1两个值
	  else {jd=3;count=0;}
}
void clockwise()//步进电机顺时针旋转
{
	unsigned char j;
	for(j=0;j<20;j++)
	{
		MOTOA = 1;
		MOTOB = 1;
		MOTOC = 0;
		MOTOD = 1;//-B
		delay_10us(SPEED);
		
		MOTOA = 0;
		MOTOB = 1;//-A
		MOTOC = 1;
		MOTOD = 1;
		delay_10us(SPEED);
		
		MOTOA = 1;
		MOTOB = 1;
		MOTOC = 1;//B
		MOTOD = 0;
		delay_10us(SPEED);
		
		MOTOA = 1;//A
		MOTOB = 0;
		MOTOC = 1;
		MOTOD = 1;
		delay_10us(SPEED);
	}
}
void antiClockwise()
{
	unsigned char j;
	for(j=0;j<20;j++)//步进电机逆时针旋转
	{	
		MOTOA = 1;//A
		MOTOB = 0;
		MOTOC = 1;
		MOTOD = 1;
		delay_10us(SPEED);

		MOTOA = 1;
		MOTOB = 1;
		MOTOC = 1;//B
		MOTOD = 0;
		delay_10us(SPEED);

		MOTOA = 0;
		MOTOB = 1;//-A
		MOTOC = 1;
		MOTOD = 1;
		delay_10us(SPEED);

		MOTOA = 1;
		MOTOB = 1;
		MOTOC = 0;
		MOTOD = 1;//-B
		delay_10us(SPEED);												
	}
}
void curtainChange()
{
	  if(flag3==0)clockwise();
	  else antiClockwise();
}
void Time0_Int() interrupt 1     //定时器0中断,TF0=1时触发，优先级比串口中断高
{
	TH0  = 0xfe;                 //重新赋值
	TL0  = 0x33;
	if(count< jd)              	 //判断0.5ms次数是否小于角度标识
	pwm=1;                       //确实小于，PWM输出高电平
	else
	pwm=0;                       //大于则输出低电平
	count=(count+1);             //0.5ms次数加1	             
	if(count==40){count=0;}//次数始终保持为40 即保持周期为20ms
}
void uart() interrupt 4//RI=1时触发
{
	unsigned char j=0;
	if(RI==1)
	{
		RI=0;
		i=SBUF;
	}
	switch(i)
	{
		case 0x01:{flag1=~flag1;g1=0;break;}
		case 0x02:{flag2=~flag2;g2=0;break;}
		case 0x03:{flag3=~flag3;g3=0;break;}
		default://如果手机发送的数据有错的话，就通过蓝牙向手机发送"error!".
			{
				for(j=0;j<6;j++)
				{
					SBUF=ascii[j];
			    while(!TI);
				  TI=0;
				}
			}
	}
	f=0;	
}