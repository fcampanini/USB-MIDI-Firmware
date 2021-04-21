/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

static struct timer_task TIMER_Service_task1, TIMER_Service_task2;

/**
 * Example of using TIMER_Service.
 */
static void TIMER_Service_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_Service_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_Service_example(void)
{
	TIMER_Service_task1.interval = 100;
	TIMER_Service_task1.cb       = TIMER_Service_task1_cb;
	TIMER_Service_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_Service_task2.interval = 200;
	TIMER_Service_task2.cb       = TIMER_Service_task2_cb;
	TIMER_Service_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_Service, &TIMER_Service_task1);
	timer_add_task(&TIMER_Service, &TIMER_Service_task2);
	timer_start(&TIMER_Service);
}
