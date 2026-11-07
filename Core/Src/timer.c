#include "main.h"
#include "timer.h"
#include "input_reading.h"
#include "led_display.h"

static int timer_counter[MAX_TIMER];
int timer_flag[MAX_TIMER];

void setTimer(int index, int duration_ms) {
    timer_counter[index] = duration_ms / TIMER_CYCLE;
    if (timer_counter[index] <= 0) timer_counter[index] = 1;
    timer_flag[index] = 0;
}

void timer_run(void) {
    for (int i = 0; i < MAX_TIMER; i++) {
        if (timer_counter[i] > 0) {
            timer_counter[i]--;
            if (timer_counter[i] <= 0) timer_flag[i] = 1;
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        static int btn_cnt = 0;
        timer_run();
        scan_display();
        if (++btn_cnt >= BUTTON_TICKS) {
            btn_cnt = 0;
            button_reading();
        }
    }
}
