#ifndef _gui_H
#define _gui_H

#include "public.h"
#include "tftlcd.h"
#include "string.h"

u16 gui_alpha_blend565(u16 src,u16 dst,u8 alpha);						//alpha blending
u16 gui_color_chg(u32 rgb);												//颜色转换.将32位色转为16位色
u16 gui_rgb332torgb565(u16 rgb332);										//将rgb332转为rgb565
long gui_pow(u8 m,u8 n);											//得到m^n
									 
u32 gui_disabs(u32 x1,u32 x2);											//差的绝对值
void gui_alphablend_area(u16 x,u16 y,u16 width,u16 height,u16 color,u8 aval);
void gui_draw_bigpoint(u16 x0,u16 y0,u16 color);						//画大点
void gui_draw_line(u16 x0,u16 y0,u16 x1,u16 y1,u16 color);				//画任意线
void gui_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color);	//画一条粗线
void gui_draw_bline1(u16 x0,u16 y0,u16 x1,u16 y1,u8 size,u16 color);	//画一条粗线,方法1
void gui_draw_rectangle(u16 x0,u16 y0,u16 width,u16 height,u16 color);	//画矩形
void gui_draw_arcrectangle(u16 x,u16 y,u16 width,u16 height,u8 r,u8 mode,u16 upcolor,u16 downcolor);//画圆角矩形
void gui_draw_vline(u16 x0,u16 y0,u16 len,u16 color);								//画垂直线
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color);								//画水平线
void gui_fill_colorblock(u16 x0,u16 y0,u16 width,u16 height,u16* ctbl,u8 mode);		//填充颜色块
void gui_smooth_color(u32 srgb,u32 ergb,u16*cbuf,u16 len);							//颜色平滑过渡
void gui_draw_smooth_rectangle(u16 x,u16 y,u16 width,u16 height,u32 srgb,u32 ergb);	//画颜色平滑过渡的矩形/线条
void gui_fill_rectangle(u16 x0,u16 y0,u16 width,u16 height,u16 color);				//填充区域
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color);
void gui_draw_ellipse(u16 x0, u16 y0, u16 rx, u16 ry,u16 color);
void gui_fill_ellipse(u16 x0,u16 y0,u16 rx,u16 ry,u16 color);
void gui_draw_argrec(u16 x0,u16 y0,u16 width,u16 height,u16 color);
void gui_show_strmid(u16 x,u16 y,u16 width,u16 height,u16 color,u8 size,u8 *str);
void gui_show_ptchar(u16 x,u16 y,u16 xend,u16 yend,u16 offset,u16 color,u16 size,u8 chr,u8 mode);
void gui_show_ptfont(u16 x,u16 y,u16 xend,u16 yend,u16 offset,u16 color,u16 size,u8* chr,u8 mode);
void gui_show_ptstr(u16 x,u16 y,u16 xend,u16 yend,u16 offset,u16 color,u8 size,u8 *str,u8 mode);
void gui_show_ptstrwhiterim(u16 x,u16 y,u16 xend,u16 yend,u16 offset,u16 color,u16 rimcolor,u8 size,u8 *str);//特效显示字符串
void gui_draw_icos(u16 x,u16 y,u8 size,u8 index);
void gui_draw_icosalpha(u16 x,u16 y,u8 size,u8 index);
void gui_show_num(u16 x,u16 y,u8 len,u16 color,u8 size,long num,u8 mode);
u8* gui_num2str(u8*str,u32 num);//将数字转为字符串
void gui_draw_arc(u16 sx,u16 sy,u16 ex,u16 ey,u16 rx,u16 ry,u16 r,u16 color,u8 mode);

u32 gui_get_stringline(u8*str,u16 linelenth,u8 font);
void gui_show_string(u8*str,u16 x,u16 y,u16 width,u16 height,u8 font,u16 fcolor);
u16 gui_string_forwardgbk_count(u8 *str,u16 pos);



#endif
