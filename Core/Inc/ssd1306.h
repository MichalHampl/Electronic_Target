/*
 * ssd1306.h
 *
 *  Created on: Nov 15, 2023
 *      Author: Michal
 */

#ifndef INC_SSD1306_H_
#define INC_SSD1306_H_

void Send_data(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size){
	HAL_GPIO_WritePin(GPIOB, GPIO_Pin_9,GPIO_PIN_SET);
	HAL_SPI_Transmit(hspi, pData, Size, 1);
}
void Send_data(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size){
	HAL_GPIO_WritePin(GPIOB, GPIO_Pin_9,GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, pData, Size, 1);
}

#endif /* INC_SSD1306_H_ */
