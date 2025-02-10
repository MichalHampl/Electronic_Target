/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define ADC4_SAMPLINGTIME_COMMON_1 LL_ADC_SAMPLINGTIME_COMMON_1
#define ADC4_SAMPLINGTIME_COMMON_2 LL_ADC_SAMPLINGTIME_COMMON_2
#define ADC4_SAMPLINGTIME_COMMON_1 LL_ADC_SAMPLINGTIME_COMMON_1
#define ADC4_SAMPLINGTIME_COMMON_2 LL_ADC_SAMPLINGTIME_COMMON_2
//#include "ssd1306.h"
#include "usbd_cdc_if.h"
#include "string.h"
#include "envelope_xcorr_gen.h"
#include "envelope_xcorr_gen_terminate.h"
#include "rt_nonfinite.h"
#include "ui.h"
#include "st7789.h"
#include <math.h>
#include <complex.h>								//it has to be done...
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//#define TEST_VER

#define THRESHOLD 400
#define SAMPLE_N 1024
#define SAMPLE_PERIOD 60
#define RESULT_BUFFER_LEN 100
#define RESULT_BUFFER_SEGMENT 10

#define B_UP 1
#define B_DOWN 2
#define B_LEFT 3
#define B_RIGHT 4
#define B_MIDDLE 5
//#define H_RANGE 2200
//#define L_RANGE 1800
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim5;

/* USER CODE BEGIN PV */
uint32_t status=0,pos,pos_debug,tik,tok,startup=0;
uint32_t state = 0,tmp = 0;
uint32_t debug = 0, debug1 = 0;
char text_buffer[64];
float s1[SAMPLE_N/4],s2[SAMPLE_N/4];
float hit[2] = {0};
double delta1 = 0, delta2 = 0;
uint16_t data[8];
uint16_t data1[SAMPLE_N];		// ADC output buffer
//result variables
uint8_t score_res[RESULT_BUFFER_LEN+10] = {0},res_pos = 0,part_res = 0;
float results_x[RESULT_BUFFER_LEN+10] = {0},results_y[RESULT_BUFFER_LEN+10] = {0};


struct settings
{
	float offsetx, offsety, speedx, speedy;
	uint8_t debug, start, normalize;

};


struct settings set = {-0.0024,-0.00225,1005,994,0,0,0};
;
target_def tg1 = {20,20,10};

extern float ts1[];
extern float ts2[];


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM5_Init(void);
/* USER CODE BEGIN PFP */
void print_array(uint16_t *arr1,uint16_t size,uint32_t start_pos, uint8_t type);
void Reset_ADC();
void AWD_Interrupt();
void TIM3_interrupt();
void Read_channel(uint16_t *source, float *target, uint32_t source_size, uint32_t target_size, uint32_t start_pos, uint32_t channel);
uint8_t ConvertCoordinates(float* coords, float diffx, float diffy, float speed, float xoffset, float yoffset, float target_size);
void Abs_maxScale(float* target, uint32_t size, uint32_t new_max);
void Offset_remove(float* target, uint32_t size);
uint8_t Check_buttons();
void Debug_screen();
uint8_t Convert_to_score(float hitx, float hity, float proj_diammeter, target_def tg);
void Draw_All();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USB_DEVICE_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_SPI1_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
  ST7789_Init();
  ST7789_InvertColors(0);
  HAL_ADC_Start_DMA(&hadc1, &data1, SAMPLE_N);
  HAL_TIM_Base_Start(&htim5);
  ADC1->CR1 &= ~ADC_CR1_AWDEN;
  ADC1->CR1 &= ~ADC_CR1_AWDIE;


  HAL_TIM_Base_Start(&htim2);
  HAL_Delay(500);
  ADC1->SR &= ~ADC_SR_AWD;
  ADC1->CR1 |= ADC_CR1_AWDEN;
  ADC1->CR1 |= ADC_CR1_AWDIE;
  TIM3->CR1 &= ~TIM_CR1_CEN;

  TIM3->DIER |= TIM_DIER_UIE;
  GPIOC->ODR |= GPIO_ODR_OD13;

/*
  for(int i = 0; i < 25; i++)
  {
	  res_pos++;
	  score_res[i] = i;
  }
*/
  if(set.debug == 0)
  {
	  Draw_All();
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  //sprintf(text_buffer,"channel 1: %04d",data1[0]);
	  //ST7789_WriteString(10, 50, text_buffer, Font_7x10, RED, BLACK);
	  //ST7789_Test();
#ifdef TEST_VER

	  tik = TIM5->CNT;
	  float delta = envelope_xcorr_gen(ts1, ts2, SAMPLE_N/4, 400000, 4, 200, 400);
	  tok = TIM5->CNT;
	  ConvertCoordinates(hit, delta/2, delta/2, 994, -0.003749, 0, 0.385);
	  //Draw_targetElement(10, 20, 200, WHITE,RED, BLACK,1,hit[0]/0.15,hit[1]/0.15);
	  Draw_def_targetElement(10, 20, 200,  WHITE,RED, BLACK, tg1);
	  Draw_hit(10, 20, 200, WHITE ,RED, 100, 50, tg1);
	  sprintf(text_buffer,"delta: %.2f us",delta*1000000);
	  ST7789_WriteString(200, 20, text_buffer, Font_7x10, WHITE, BLACK);
	  sprintf(text_buffer,"time: %04ld us",tok-tik);
	  ST7789_WriteString(200, 30, text_buffer, Font_7x10, WHITE, BLACK);
	  sprintf(text_buffer,"x=: %3.1f mm",hit[0]*1000);
	  ST7789_WriteString(200, 40, text_buffer, Font_7x10, WHITE, BLACK);
	  sprintf(text_buffer,"y=: %3.1f mm",hit[1]*100);
	  ST7789_WriteString(200, 50, text_buffer, Font_7x10, WHITE, BLACK);
	  HAL_Delay(2000);

#endif

#ifndef TEST_VER
	  switch(state)
	  {
	  	  case 1:							//aktivace TIM3 interruptu
	  		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	  		HAL_Delay(500);
	  		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	  		HAL_Delay(500);
	  		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	  		tik = TIM5->CNT;
	  		Read_channel((uint16_t*) data1, s1, SAMPLE_N, SAMPLE_N/4, ((pos-pos%4)-SAMPLE_N/8)%SAMPLE_N, 0);
	  		Read_channel((uint16_t*) data1, s2, SAMPLE_N, SAMPLE_N/4, ((pos-pos%4)-SAMPLE_N/8)%SAMPLE_N, 1);
	  		if(set.normalize)
	  		{
	  			Offset_remove(s1, SAMPLE_N/4);
	  			Offset_remove(s2, SAMPLE_N/4);
	  			Abs_maxScale(s1, SAMPLE_N/4, 2000);
	  			Abs_maxScale(s2, SAMPLE_N/4, 2000);
	  		}
	  		delta1 = envelope_xcorr_gen(s1, s2, SAMPLE_N/4, 400000, 4, 200, 400);
	  		Read_channel((uint16_t*) data1, s1, SAMPLE_N, SAMPLE_N/4, ((pos-pos%4)-SAMPLE_N/8)%SAMPLE_N, 2);
	  		Read_channel((uint16_t*) data1, s2, SAMPLE_N, SAMPLE_N/4, ((pos-pos%4)-SAMPLE_N/8)%SAMPLE_N, 3);
	  		if(set.normalize)
	  		{
	  			Offset_remove(s1, SAMPLE_N/4);
	  			Offset_remove(s2, SAMPLE_N/4);
	  			Abs_maxScale(s1, SAMPLE_N/4, 2000);
	  			Abs_maxScale(s2, SAMPLE_N/4, 2000);
	  		}
	  		delta2 = envelope_xcorr_gen(s1, s2, SAMPLE_N/4, 400000, 4, 200, 400);
	  		tok = TIM5->CNT;
	  		tmp = ConvertCoordinates(hit, delta1, delta2, set.speedx, set.offsetx,set. offsety, 0.385);
	  		if(tmp == 1)
	  		{
	  			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	  			HAL_Delay(2000);
	  			Reset_ADC();
	  			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	  			TIM2->CR1 |= TIM_CR1_CEN;
	  			state = 0;
	  			break;
	  		}
	  		results_x[res_pos] = hit[0];
	  		results_y[res_pos] = hit[1];
	  		score_res[res_pos] = Convert_to_score(hit[0], hit[1], 3, tg1);

	  		if(res_pos < part_res*RESULT_BUFFER_SEGMENT || res_pos > (part_res+1)*RESULT_BUFFER_SEGMENT)
	  		{
	  			part_res = res_pos/RESULT_BUFFER_SEGMENT;
	  		}
	  		if(res_pos >= RESULT_BUFFER_LEN)
	  		{
	  			for(int i = 0; i<RESULT_BUFFER_LEN;i++)
	  			{
	  				results_x[i] = 0;
	  				results_x[i] = 0;
	  				score_res[i] = 0;
	  				part_res = 0;

	  			}
	  		}
	  		//Draw_targetElement(10, 20, 200, WHITE,RED, BLACK,1,hit[0]/0.15,hit[1]/0.15);
	  		Draw_def_targetElement(10, 20, 200,  WHITE,RED, BLACK, tg1);
	  		res_pos++;
	  		if(set.debug)
	  		{
	  			Debug_screen();
			}
	  		else
			{
	  			Draw_All();
			}

	  		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	  		HAL_Delay(1000);
	  		Reset_ADC();
	  		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	  		TIM2->CR1 |= TIM_CR1_CEN;												//Timer 2 enabled, sampling continues
	  		state = 0;
	  	  default:
	  		  break;
	  }
	  if(Check_buttons()==B_UP)
	  {
		  part_res=(part_res+1);
		  if(part_res > 10){part_res = 0;}
		  Draw_All();
	  }
	  if(Check_buttons()==B_DOWN)
	  {
		  part_res=(part_res-1);
		  if(part_res > 10){part_res = 10;}
		  Draw_All();
	  }
#endif


	  /*
	  if(GPIOB->IDR & 0b1)
	  {
		  tmp = 1;
		  TIM2->CR1 &= !TIM_CR1_CEN;				//Timer2 enable bit set to zero.Timer 2 is disabled.
		  print_array(&data1, SAMPLE_N,0);
		  HAL_Delay(10000);
	  }else{
		  tmp = 0;
		  TIM2->CR1 |= TIM_CR1_CEN;					//Timer 2 enable bit set to one.Timer 2 is enabled.
	  }

	  debug[pos_debug%50] = DMA2_Stream0->NDTR;
	  pos_debug++;
	  */

	  /*alternative if statement
	  data1[DMA2_Stream0->NDTR] > H_RANGE || data1[DMA2_Stream0->NDTR] < L_RANGE || GPIOB->IDR & 0b1


	   */

	  // following code should be handled mostly in interrupts and is unusable for higher ADC speeds
	  /*
	  }
		*/

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 384;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_AnalogWDGConfTypeDef AnalogWDGConfig = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = ENABLE;
  hadc1.Init.NbrOfDiscConversion = 1;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 4;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the analog watchdog
  */
  AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_ALL_REG;
  AnalogWDGConfig.HighThreshold = 2048+THRESHOLD;
  AnalogWDGConfig.LowThreshold = 2048-THRESHOLD;
  AnalogWDGConfig.ITMode = ENABLE;
  if (HAL_ADC_AnalogWDGConfig(&hadc1, &AnalogWDGConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = SAMPLE_PERIOD-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = SAMPLE_PERIOD-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = SAMPLE_N/2;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 95;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 4294967295;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_SET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB5 PB6 PB7 PB8
                           PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void print_array(uint16_t *arr1,uint16_t size,uint32_t start_pos, uint8_t type){

	uint8_t xd;
	uint16_t i;
	char print_buffer[50];
	uint32_t buffer[4];

	if(type == 0)
	{
	xd = sprintf(print_buffer,"start:\n\n\r");
	CDC_Transmit_FS((uint8_t*)print_buffer,xd);

	for(i = 0;i < size;i+=4){
		xd = sprintf(print_buffer,"%04d %04d %04d %04d; ",arr1[(start_pos+i)%size],arr1[(start_pos+i+1)%size],arr1[(start_pos+i+2)%size],arr1[(start_pos+i+3)%size]);     	//saving data to buffer in format: 0123 0123 0123 0123; 0123 ...
		CDC_Transmit_FS((uint8_t*)print_buffer,xd);																															//sending buffer
		HAL_Delay(1);
	}
	/*
	xd = sprintf(uart_buffer,"; ");
	CDC_Transmit_FS((uint8_t*)uart_buffer,xd);
	for(i=0;i<size;i++){
			xd = sprintf(uart_buffer,"%d ",arr2[(i+var)%size]);
			CDC_Transmit_FS((uint8_t*)uart_buffer,xd);
			HAL_Delay(1);
		}
	*/
	xd = sprintf(print_buffer,"\n end\n\r");
	CDC_Transmit_FS((uint8_t*)print_buffer,xd);
	}
	if(type == 1)
	{
	for(i = 0;i < size;i+=4)
		{

		buffer[0] = arr1[(start_pos+i)%size];
		buffer[1] = arr1[(start_pos+i+1)%size];
		buffer[2] = arr1[(start_pos+i+2)%size];
		buffer[3] = arr1[(start_pos+i+3)%size];
		CDC_Transmit_FS((uint8_t*)buffer,16);																															//sending buffer
		HAL_Delay(1);
		}
	}
	return;
}
void Read_channel(uint16_t *source, float *target, uint32_t source_size, uint32_t target_size, uint32_t start_pos, uint32_t channel)
{
	uint32_t chnl_cnt = source_size/target_size;
	for(int i = 0; i<target_size; i++)
	{
		target[i] = source[(start_pos+i*chnl_cnt+channel)%source_size];
	}
}

uint8_t ConvertCoordinates(float* coords, float diffx, float diffy, float speed, float xoffset, float yoffset, float target_size)
{
	//normalization to -2 to 2 scale and conversion to complex float
	double _Complex dx = ((diffx*speed+xoffset)*4)/target_size;
	double _Complex dy = ((diffy*speed+yoffset)*4)/target_size;
	if(fabsf(diffx*speed+xoffset) > target_size || fabsf(diffy*speed+yoffset) > target_size)
	{
		return 1;
	}

	double _Complex x = -1*_Complex_I*(dx*csqrt(dy*dy-4)*csqrt(dx*dx-dy*dy-4))/(4*csqrt(dx*dx+dy*dy-4));
	double _Complex y = -1*_Complex_I*(dy*csqrt(dx*dx-4)*csqrt(-dx*dx+dy*dy-4))/(4*csqrt(dx*dx+dy*dy-4));

	coords[0] = (float)creal(x)*target_size/2;
	coords[1] = (float)creal(y)*target_size/2;
	return 0;

}
void Abs_maxScale(float* target,uint32_t size, uint32_t new_max)
{
	uint32_t i;
	float max = 0;
	float scale_factor;
	for(i = 0; i < size; i++)
	{
		if(max < fabsf(target[i]))
		{
			max = fabsf(target[i]);
		}
	}
	if(max < new_max)
	{
		scale_factor = (float)new_max/max;
		for(i = 0; i < size; i++)
		{
			target[i] = target[i]*scale_factor;
		}
	}

}

void Offset_remove(float* target,uint32_t size)
{
	uint32_t i;
	float sum;
	float mean;
	for(i = 0; i < size; i++)
	{
		sum += target[i];
	}
	mean = sum/(float)size;
	for(i = 0; i < size; i++)
	{
		target[i] = target[i]-mean;
	}
}
void Debug_screen()
{
	Draw_def_targetElement(10, 20, 200,  WHITE,RED, BLACK, tg1);
	Draw_hit(10, 20, 200, WHITE ,RED, hit[0]*1000, hit[1]*1000, tg1);
	sprintf(text_buffer,"d1: %.2f us",delta1*1000000);
	ST7789_WriteString(220, 20, text_buffer, Font_7x10, WHITE, BLACK);
	sprintf(text_buffer,"d2: %.2f us",delta2*1000000);
	ST7789_WriteString(220, 30, text_buffer, Font_7x10, WHITE, BLACK);
	sprintf(text_buffer,"time: %03ld ms",(tok-tik)/1000);
	ST7789_WriteString(220, 40, text_buffer, Font_7x10, WHITE, BLACK);
	sprintf(text_buffer,"x=: %.1f mm",hit[0]*1000);
	ST7789_WriteString(220, 50, text_buffer, Font_7x10, WHITE, BLACK);
	sprintf(text_buffer,"y=: %.1f mm",hit[1]*1000);
	ST7789_WriteString(220, 60, text_buffer, Font_7x10, WHITE, BLACK);
	print_array((uint16_t*) data1,SAMPLE_N,((pos-pos%4)-SAMPLE_N/8)%SAMPLE_N,1);						//Print out whole ADC buffer to console pos - pos%4
}

void Draw_All()
{
	Draw_def_targetElement(10, 20, 200,  WHITE,RED, BLACK, tg1);
	Draw_resultTable(220, 10, 10, score_res, part_res, WHITE, BLACK);

	if(startup == 1){
		startup++;
	}else{
		if(res_pos < RESULT_BUFFER_SEGMENT*(part_res+1))
		{
			if(part_res == 0)
			{
				for(int i = RESULT_BUFFER_SEGMENT*(part_res);i<res_pos;i++)
				{
					Draw_hit(10, 20, 200, WHITE ,RED, results_x[i]*1000, results_y[i]*1000, tg1);
				}
			}
			else
			{
				for(int i = RESULT_BUFFER_SEGMENT*(part_res);i<=res_pos;i++)
				{
					Draw_hit(10, 20, 200, WHITE ,RED, results_x[i]*1000, results_y[i]*1000, tg1);
				}
			}
		}
		else
		{
			for(int i = RESULT_BUFFER_SEGMENT*part_res;i<RESULT_BUFFER_SEGMENT*(part_res+1);i++)
			{
				Draw_hit(10, 20, 200, WHITE ,RED, results_x[i]*1000, results_y[i]*1000, tg1);
			}
		}
	}
}
uint8_t Check_buttons()
{
	uint8_t on = GPIO_PIN_RESET;
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == on){return 1;}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == on){return 2;}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == on){return 3;}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == on){return 4;}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == on){return 5;}
	return 0;

}

uint8_t Convert_to_score(float hitx, float hity, float proj_diammeter, target_def tg)
{
	float r = sqrtf(hitx*hitx+hity*hity)*1000;
	if(r*2<tg.inner_circle_mm+proj_diammeter){return 10;}
	for(uint16_t i = 1;i < tg.circle_count-1;i++)
	{
		if(r*2<tg.inner_circle_mm+tg.circle_distance_mm*i+proj_diammeter)
		{
			return 10-i;
		}
	}
	return 0;
}

void AWD_Interrupt()
{

	pos = SAMPLE_N - DMA2_Stream0->NDTR;									//saving current ADC buffer position
	TIM3->CR1 |= TIM_CR1_CEN;												//enabling Timer 3, which will wait for SAMPLE_N/2 of samples to be measured before calling interrupt
	ADC1->CR1 &= ~ADC_CR1_AWDEN;
	ADC1->CR1 &= ~ADC_CR1_AWDIE;											//disable AWD interrupt mode !!!enable later!!!
	return;
}
void Reset_ADC()
{

	TIM2->CR1 &= ~TIM_CR1_CEN;												//Timer 2 is disabled.
	TIM3->CR1 &= ~TIM_CR1_CEN;												//Timer 3 is disabled.
	TIM3->CNT = 0;															//Resetting Timer 3 counter
	//TIM2->CR1 |= TIM_CR1_CEN;												//Timer 2 is enabled.
	//HAL_Delay(100);
	//TIM2->CR1 &= ~TIM_CR1_CEN;												//Timer 2 is disabled.
	ADC1->SR &= ~ADC_SR_AWD;
	ADC1->CR1 |= ADC_CR1_AWDEN;
	ADC1->CR1 |= ADC_CR1_AWDIE;											//enabling AWD interrupt mode !! MUST be done twice !!
	ADC1->CR1 |= ADC_CR1_AWDIE;
	return;
}

void TIM3_interrupt()
{
	TIM2->CR1 &= ~TIM_CR1_CEN;												//Timer 2 is disabled sampling stops
	TIM3->CR1 &= ~TIM_CR1_CEN;
	state = 1;
	return;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
