/*
 * ui.c
 *
 */
#include "ui.h"


void Draw_targetElement(uint16_t x, uint16_t y, uint16_t size, uint16_t color, uint16_t hitcolor, uint16_t bgcolor, uint16_t hit, float hitx, float hity)
{
	ST7789_Fill(x, y, x+size, y+size, bgcolor);
	ST7789_DrawLine(x, y+size/2, x+size, y+size/2, color);
	ST7789_DrawLine(x+size/2, y, x+size/2, y+size, color);
	ST7789_DrawPixel(x+size/2, y+size/2, color);
	for(uint16_t i = 1; i<5 ;i++)
	{
		ST7789_DrawCircle(x+size/2, y+size/2, i*(size/10), color);
	}
	if(hit)
	{
		ST7789_DrawFilledCircle(x+(int)(size*(hitx+1)/2), y+(int)(size*(hity+1)/2), 2, hitcolor);
	}
}

void Draw_def_targetElement(uint16_t x, uint16_t y, uint16_t size, uint16_t color, uint16_t hitcolor, uint16_t bgcolor, target_def target)
{
	float outer_diameter = target.inner_circle_mm + target.circle_distance_mm*(target.circle_count-1);
	float scale = size/outer_diameter/2;
	ST7789_Fill(x, y, x+size, y+size, bgcolor);
	ST7789_DrawLine(x, y+size/2, x+size, y+size/2, color);
	ST7789_DrawLine(x+size/2, y, x+size/2, y+size, color);
	ST7789_DrawPixel(x+size/2, y+size/2, color);
	ST7789_DrawCircle(x+size/2, y+size/2, scale*target.inner_circle_mm, color);
	for(uint16_t i = 1; i<target.circle_count ;i++)
		{
			ST7789_DrawCircle(x+size/2, y+size/2, scale*(target.inner_circle_mm+target.circle_distance_mm*i), color);
		}
}

void Draw_hit(uint16_t x, uint16_t y, uint16_t size, uint16_t color, uint16_t hitcolor, float hitx, float hity, target_def target)
{
	float outer_diameter = target.inner_circle_mm + target.circle_distance_mm*(target.circle_count-1);
	float scale = size/outer_diameter/2;

	ST7789_DrawFilledCircle(x+size/2+(int)(scale*hitx*2), y+size/2-(int)(scale*hity*2), 3, hitcolor);
}

void Draw_resultTable(uint16_t x, uint16_t y, uint16_t rows, uint8_t* score, uint16_t index, uint16_t color, uint16_t bgcolor)
{
	uint16_t sum = 0;
	uint8_t row_size = 20, width = 80;
	char text_buffer[3];
	index = index*rows;
	ST7789_WriteString(x, y, "SCORE:", Font_16x26, color, bgcolor);
	y = y+28;
	ST7789_Fill(x, y, x+width, y+row_size*(rows+1), bgcolor);
	ST7789_DrawRectangle(x, y, x+width, y+row_size*(rows/2), color);
	ST7789_DrawLine(x+width/2, y, x+width/2, y+rows/2*row_size, color);
	for(uint16_t i = 0; i<rows ;i++)
	{
		if(i < rows/2)
		{
			sum = sum + score[index+i];
			sprintf(text_buffer,"%2d",score[index+i]);
			ST7789_WriteString(x+2, y+row_size*(i)+2, text_buffer, Font_11x18, color, bgcolor);
		}
		else
		{
			sum = sum + score[index+i];
			sprintf(text_buffer,"%2d",score[index+i]);
			ST7789_WriteString(x+2+width/2, y+row_size*(i-rows/2)+2, text_buffer, Font_11x18, color, bgcolor);
			ST7789_DrawLine(x, y+(i+1-rows/2)*row_size, x+width, y+(i+1-rows/2)*row_size, color);
		}
	}
	sprintf(text_buffer,"%2d",sum);
	ST7789_WriteString(x, y+row_size*(rows/2)+4, "TOTAL:", Font_16x26, color, bgcolor);
	ST7789_WriteString(x+2, y+row_size*(rows/2)+35, "   ", Font_16x26, color, bgcolor);
	ST7789_WriteString(x+2, y+row_size*(rows/2)+35, text_buffer, Font_16x26, color, bgcolor);
	ST7789_WriteString(x, y+row_size*(rows/2)+62, "Page:", Font_11x18, color, bgcolor);
	sprintf(text_buffer,"%d",index/rows);
	ST7789_WriteString(x+2+50, y+row_size*(rows/2)+62, "   ", Font_11x18, color, bgcolor);
	ST7789_WriteString(x+2+50, y+row_size*(rows/2)+62, text_buffer, Font_11x18, color, bgcolor);
}
