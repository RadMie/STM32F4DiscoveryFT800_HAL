/**
  ******************************************************************************
  * File Name          : main.c
  * Date               : 22/05/2015 23:30:11
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "spi.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
#include "global.h"
#include "ft_gpu.h"
#include "ft800.h"
#include "eve.h"
#include "co_processor.h"

unsigned int cmdBufferRd = 0x0000;																// Used to navigate command ring buffer
unsigned int cmdBufferWr = 0x0000;																// Used to navigate command ring buffer
unsigned int cmdOffset = 0x0000;																// Used to navigate command rung buffer

//static unsigned int dli;
void dl(unsigned long cmd)
{
	ft800memWrite32(RAM_DL + dli, cmd);
	dli += 4;
}
int angle = 10000;
int angle1 = 0;
int sw = 0;
int sw1 = 0;
int sw3 = 0;
int s = 0;
int ss = 0;
int sp = 55;
uint16_t val = 0;
uint16_t val1 = 0;
uint8_t sound = 0;
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();

  /* USER CODE BEGIN 2 */
  ft800_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

	do
		{
			cmdBufferRd = ft800memRead16(REG_CMD_READ);															// Read the graphics processor read pointer
			cmdBufferWr = ft800memRead16(REG_CMD_WRITE); 														// Read the graphics processor write pointer
		} while (cmdBufferWr != cmdBufferRd);

	ft800memWrite32(REG_CMD_WRITE, 0);
	ft800memWrite32(REG_CMD_READ, 0);
	HAL_Delay(100);



	do
		{
			cmdBufferRd = ft800memRead16(REG_CMD_READ);															// Read the graphics processor read pointer
			cmdBufferWr = ft800memRead16(REG_CMD_WRITE); 														// Read the graphics processor write pointer
		} while (cmdBufferWr != cmdBufferRd);
	cli = cmdBufferWr;
	cli = 0;

	ft800memWrite32(REG_CMD_WRITE, 0);
	ft800memWrite32(REG_CMD_READ, 0);

	cmd_dlstart();
	//cmd_coldstart();

	cmd(DL_CLEAR_RGB | 0x000000UL);
	cmd(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);

	cmd(BEGIN(RECTS));
	cmd(VERTEX2II(1, 203, 0, 0));
	cmd(COLOR_RGB(22, 23, 26));
	cmd(VERTEX2II(496, 291, 0, 0));
	cmd(END());


	cmd_fgcolor(0x57a515);
	cmd(COLOR_RGB(255, 255, 255));
	cmd_button(18, 219, 130, 36, 27, OPT_FLAT, "K1");
	cmd_fgcolor(0xff9900);
	cmd_button(175, 220, 130, 36, 27, OPT_FLAT, "K2");
	cmd_fgcolor(0x99aaaa);
	cmd_button(331, 220, 130, 36, 27, OPT_FLAT, "K3");

	cmd_fgcolor(0x57a515);
	cmd_button(307, 78, 70, 70, 31, OPT_FLAT, "I");
	cmd_clock(241, 150, 43, OPT_FLAT, 13, 51, 17, 0);
	cmd_fgcolor(0x6c6f7c);
	cmd_button(339, 18, 120, 36, 27, OPT_FLAT, "Menu");
	cmd_gauge(99, 101, 90, OPT_FLAT, 10, 5, 61, 100);
	cmd_text(197, 22, 30, 0, "Test");
	cmd_fgcolor(0xfe011b);
	cmd_button(389, 78, 70, 70, 31, OPT_FLAT, "O");
	cmd_text(80, 143, 30, 0, "7.2");
	cmd_text(84, 118, 27, 0, "BAR");
	cmd_text(39, 154, 26, 0, "MIN");
	cmd_text(135, 152, 26, 0, "MAX");
	cmd_text(307, 164, 28, 0, "Status:");
	cmd_button(389, 164, 69, 23, 27, OPT_FLAT, "OFF");

	cmd(DL_END);
	cmd(DL_DISPLAY);
	cmd(CMD_SWAP);

	ft800memWrite16(REG_CMD_WRITE, cli);

	HAL_Delay(10);

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
/* User can add his own implementation to report the file name and line number,
ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
