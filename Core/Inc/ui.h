/*
 * ui.h
 *
 */

#ifndef INC_UI_H_
#define INC_UI_H_

#include "st7789.h"
#include "string.h"

typedef struct Target_def
{
	float inner_circle_mm;
	float circle_distance_mm;
	uint8_t circle_count;
} target_def;


void Draw_targetElement(uint16_t x, uint16_t y, uint16_t size, uint16_t color, uint16_t hitcolor, uint16_t bgcolor, uint16_t hit, float hitx, float hity);
void Draw_def_targetElement(uint16_t x, uint16_t y, uint16_t size, uint16_t color, uint16_t hitcolor, uint16_t bgcolor, target_def target);
void Draw_hit(uint16_t x, uint16_t y, uint16_t size, uint16_t color, uint16_t hitcolor, float hitx, float hity, target_def target);
void Draw_resultTable(uint16_t x, uint16_t y, uint16_t rows, uint8_t* score, uint16_t index, uint16_t color, uint16_t bgcolor);
#endif /* INC_UI_H_ */
