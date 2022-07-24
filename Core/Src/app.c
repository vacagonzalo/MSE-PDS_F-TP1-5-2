#include "main.h"
#include "arm_math.h"
#include "arm_const_structs.h"

typedef struct
{
	uint32_t wait_time;
	uint32_t systick_limit;
} systick_delay_t;

typedef struct
{
	char head;
	uint16_t val;
} data_frame_t;

static data_frame_t data_frame = {
		.head = '<',
		.val = 0
};

#define MAX_ID 10000

void systick_delay_init(systick_delay_t * delay, uint32_t milliseconds);
void systick_delay(systick_delay_t * delay);

extern UART_HandleTypeDef huart3;
extern ADC_HandleTypeDef hadc1;

void app_launch()
{
	systick_delay_t dsp_delay;
	systick_delay_init(&dsp_delay, 1);
	while(1)
	{

		/* ADC read */
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		data_frame.val = HAL_ADC_GetValue(&hadc1);
		data_frame.head = '<';

		/* UART Transmit */
		HAL_UART_Transmit(&huart3, (uint8_t *)&data_frame, sizeof(data_frame_t), HAL_MAX_DELAY);

		/* Consume time to keep the fs stable */
		systick_delay(&dsp_delay);
	}
}

void systick_delay_init(systick_delay_t * delay, uint32_t milliseconds)
{
	delay->wait_time = milliseconds;
	delay->systick_limit = HAL_GetTick() + delay->wait_time;
}

void systick_delay(systick_delay_t * delay)
{
	while(HAL_GetTick() < delay->systick_limit);
	systick_delay_init(delay, delay->wait_time);
}
