/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "bwos.h"
#include "nrf_gpio.h"

static void Board_LED_init()
{
	nrf_gpio_cfg(2, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
	nrf_gpio_cfg(3, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
	nrf_gpio_cfg(4, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
	nrf_gpio_cfg(5, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
	nrf_gpio_pin_clear(2);
	nrf_gpio_pin_clear(3);
	nrf_gpio_pin_clear(4);
	nrf_gpio_pin_clear(5);
}

static void Board_LED_Set(int num, int f)
{
	uint32_t led_num = num + 2;
	if ( f ) {
		nrf_gpio_pin_set(led_num);
	}
	else {
		nrf_gpio_pin_clear(led_num);
	}
}

/* configuration task */
static int		task_struct[16];

void tmout_handler(void* param, bool is_first)
{
static bool flag = true;
	Board_LED_Set(0, flag ? 1 : 0);
	flag = flag ? false : true;
}

void timer_start()
{
	TimerInfo info;
	int timer = timer_create();
	info.cyclic = SEC(2);
	info.tmout = SEC(5);
	info.kind = TIMER_CYCLIC;
	info.param = 0;
	info.handler = tmout_handler;

	timer_set(timer, &info);
	timer_enable(timer, true);
}

void task1(void* arg0, void* arg1)
{
	lprintf("task1\n");
	for (;;) {
		Board_LED_Set(1, 0);
		task_tsleep(MSEC(100));
		Board_LED_Set(1, 1);
		task_tsleep(MSEC(100));
	}
}

volatile uint32_t s_cpuid;

void task2(void* arg0, void* arg1)
{
	lprintf("task2\n");
	s_cpuid = SCB->CPUID;
	for (;;) {
		Board_LED_Set(2, 0);
		task_tsleep(SEC(1));
		Board_LED_Set(2, 1);
		task_tsleep(SEC(1));
	}
}

float a = 10.0f;
float b = 20.0f;

void task3(void* arg0, void* arg1)
{
	lprintf("task3\n");
	for (;;) {
		a += 1.1f;
		lprintf("task3:%d\n", (uint32_t)a);
		task_tsleep(MSEC(100));
	}
}

void task4(void* arg0, void* arg1)
{
	lprintf("task4\n");
	for (;;) {
		b += 2.2f;
		lprintf("task4:%d\n", (uint32_t)b);
		task_tsleep(MSEC(500));
		nrf_gpio_pin_toggle(5);
	}
}

TaskCreateInfo	task_info[] = {
		{"TASK1", TASK_ACT, task1, 1024, 1024, 5, (void*)128},
		{"TASK2", TASK_ACT, task2, 1024, 1024, 6, (void*)128},
		{"TASK3", TASK_ACT, task3, 1024, 1024, 7, (void*)128},
		{"TASK4", TASK_ACT, task4, 1024, 1024, 8, (void*)128},
};

void main_task(void)
{
	Board_LED_init();
	int ix;
	for ( int ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}
	timer_start();

	task_sleep();
}
