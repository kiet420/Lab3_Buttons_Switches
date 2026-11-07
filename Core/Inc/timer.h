#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#define TIMER_CYCLE    10     // ms, phải khớp với ngắt TIM2 (Counter Period = TIMER_CYCLE - 1)
#define BUTTON_PERIOD  10     // ms, chu kỳ đọc nút (>= 10ms để chống rung)
#define BUTTON_TICKS   ((BUTTON_PERIOD > TIMER_CYCLE) ? (BUTTON_PERIOD / TIMER_CYCLE) : 1)
#define BUTTON_READ_PERIOD (BUTTON_TICKS * TIMER_CYCLE)

#define MAX_TIMER      4
#define TIMER_SEC      0      // đếm 1 giây cho đèn giao thông
#define TIMER_SCAN     1      // quét LED 7 đoạn
#define TIMER_BLINK    2      // nháy 2Hz (đổi trạng thái mỗi 250ms)
#define TIMER_REPEAT   3      // tự tăng khi giữ nút

extern int timer_flag[MAX_TIMER];
void setTimer(int index, int duration_ms);
void timer_run(void);

#endif /* INC_TIMER_H_ */
