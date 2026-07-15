/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

-*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

const int8_t cols = 16;
const int8_t rows = 8 ;

   int8_t frames[8][16] = {                                \
                          {0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0},    \
                          {0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0},    \
                          {0,0,1,1,1,0,0,0,0,0,0,1,0,0,0,0},    \
                          {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},    \
                          {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},    \
                          {0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0},    \
                          {0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,0},    \
                          {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0},    \
                                                                \
                        };





int8_t vecini[8][16]; // tabela in care stocam numarul de vecini vii pentru fiecare celula

// subprogram de afisare al datelor.

void display(int8_t frame[rows][cols])
{
	uint16_t steps[8] =
    {
	    GPIO_PIN_7,  GPIO_PIN_6,  GPIO_PIN_5,  GPIO_PIN_4,
	    GPIO_PIN_3,  GPIO_PIN_2,  GPIO_PIN_1,  GPIO_PIN_0
	};

    int8_t i; //contor cu care parcurgem display-ul.
    int8_t repetitii; // contor de repetitii la afisare (asta ca sa persiste un pic imaginea

    for(repetitii = 0; repetitii < 5; repetitii ++)
        for(i = 0; i < rows; i++)
        {
        	HAL_GPIO_WritePin(GPIOA, (frame[i][0 ] << 15) | (frame[i][1 ] << 14) | (frame[i][2 ] << 13) | (frame[i][3 ] << 12) |
                                   (frame[i][4 ] << 11) | (frame[i][5 ] << 10) | (frame[i][6 ] <<  9) | (frame[i][7 ] <<  8) |
                                   (frame[i][8 ] <<  7) | (frame[i][9 ] <<  6) | (frame[i][10] <<  5) | (frame[i][11] <<  4) |
                                   (frame[i][12] <<  3) | (frame[i][13] <<  2) | (frame[i][14] <<  1) | (frame[i][15] <<  0) , GPIO_PIN_SET);

            HAL_GPIO_WritePin(GPIOB, steps[rows-i-1] , GPIO_PIN_SET);
            HAL_Delay(1);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_All, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_All, GPIO_PIN_RESET);
        }
}

void numara_vecini()
{   int8_t i,j;  // rezultatul este stocat in tabela vecini[8][8]. Parcurgem fiecare celula si ii numaram vecinii

    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            vecini[i][j] = 0;  // initializam matricea de vecinatati cu zero

    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
    {

        if((i > 0) && (j > 0))
        {
            if(frames[i-1][j-1] == 1)
                vecini[i][j]++;
        }
        else
        {
            if((i == 0)&&(j == 0))
               if(frames[rows - 1][cols - 1] == 1)
                    vecini[i][j]++;

            if((i > 0 )&&(j == 0))
               if(frames[i-1][ cols - 1] == 1)
                    vecini[i][j]++;

            if((i == 0)&&(j > 0))
               if(frames[rows - 1][j-1] == 1)
                    vecini[i][j]++;
        }



        if(i > 0)
        {
            if(frames[i-1][j] == 1)
                vecini[i][j]++;
        }
        else
        {
            if(frames[rows -1][j] == 1)
                vecini[i][j]++;
        }


        if((i > 0) && (j < cols - 1))
        {
            if(frames[i-1][j+1] == 1)
                vecini[i][j]++;
        }
        else
        {
            if((i == 0)&&(j == cols - 1))
               if(frames[rows - 1][0] == 1)
                    vecini[i][j]++;

            if((i > 0)&&(j == cols - 1))
               if(frames[i-1][0] == 1)
                    vecini[i][j]++;

            if((i == 0 )&&(j < cols - 1))
               if(frames[rows-1][j+1] == 1)
                    vecini[i][j]++;
        }


        if(j > 0)
        {
            if(frames[i][j-1] == 1)
                vecini[i][j]++;
        }
        else
        {
            if(frames[i][cols - 1] == 1)
                vecini[i][j]++;
        }


        if(j < cols - 1)
        {
            if(frames[i][j+1] == 1)
                vecini[i][j]++;
        }
        else
        {
            if(frames[i][0] == 1)
                vecini[i][j]++;
        }


        if((i < rows - 1) && (j > 0))
        {
            if(frames[i+1][j-1] == 1)
                vecini[i][j]++;
        }
        else
        {
            if((i == rows - 1)&&(j == 0))
               if(frames[0][cols - 1] == 1)
                    vecini[i][j]++;

            if((i == rows - 1)&&(j > 0))
               if(frames[0][j-1] == 1)
                    vecini[i][j]++;

            if((i < rows - 1)&&(j == 0))
               if(frames[i+1][cols - 1] == 1)
                    vecini[i][j]++;
        }


        if(i < rows - 1)
        {
            if(frames[i+1][j] == 1)
                vecini[i][j]++;
        }
        else
        {
            if(frames[0][j] == 1)
                vecini[i][j]++;
        }


        if((i < rows - 1) && (j < cols - 1))
        {
            if(frames[i+1][j+1] == 1)
                vecini[i][j]++;
        }
        else
        {
            if((i == rows - 1)&&(j == cols - 1))
               if(frames[0][0] == 1)
                    vecini[i][j]++;

            if((i == rows - 1)&&(j < cols - 1))
               if(frames[0][j+1] == 1)
                    vecini[i][j]++;

            if((i < rows - 1)&&(j == cols - 1))
               if(frames[i+1][0] == 1)
                    vecini[i][j]++;
        }


    }
}

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
  /* USER CODE BEGIN 2 */

  int8_t i,j;

  for (i = 0; i < rows; i++)
	  for (j = 0; j < cols; j++)
		  vecini[i][j] = 0;  // initializam matricea de vecinatati cu zero

  //  PORTA e port de iesire



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  while(1)
  {
      display(frames);

      numara_vecini();
      for (i = 0; i < rows; i++)
          for (j = 0; j < cols; j++)
          {
              if ((vecini[i][j] == 3)&&(frames[i][j]==0))
              {
                  frames[i][j] = 1;
              }

              if ((vecini[i][j] < 2)||(vecini[i][j] > 3))
              {
                  frames[i][j] = 0;
              }

              if (((vecini[i][j] == 2)||(vecini[i][j] == 3))&&(frames[i][j] == 1))
                  frames[i][j] = 1;

          }

  }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 PA6 PA7
                           PA8 PA9 PA10 PA11
                           PA12 PA13 PA14 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB3
                           PB4 PB5 PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
    