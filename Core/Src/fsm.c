#include "main.h"
#include "global.h"
#include "timer.h"
#include "input_processing.h"
#include "led_display.h"
#include "fsm.h"

typedef enum { R1_G2, R1_Y2, G1_R2, Y1_R2 } TrafficState;

static TrafficState ts;
static int t1, t2, tState;   // đếm ngược đường 1, đường 2, thời gian còn lại của trạng thái
static int blink = 0;

static void enter_state(TrafficState s) {
    ts = s;
    switch (s) {
    case R1_G2:
        setRoad1(C_RED);    setRoad2(C_GREEN);
        t1 = dur_red;       t2 = dur_green;    tState = dur_green;
        break;
    case R1_Y2:
        setRoad1(C_RED);    setRoad2(C_YELLOW);
        t1 = dur_yellow;    t2 = dur_yellow;   tState = dur_yellow;
        break;
    case G1_R2:
        setRoad1(C_GREEN);  setRoad2(C_RED);
        t1 = dur_green;     t2 = dur_red;      tState = dur_green;
        break;
    case Y1_R2:
        setRoad1(C_YELLOW); setRoad2(C_RED);
        t1 = dur_yellow;    t2 = dur_yellow;   tState = dur_yellow;
        break;
    }
}

static void enter_mode(int m) {
    mode = m;
    button_event[1] = 0;
    button_event[2] = 0;
    if (m == MODE_1) {
        enter_state(R1_G2);
        setTimer(TIMER_SEC, 1000);
    } else {
        setRoad1(C_OFF);
        setRoad2(C_OFF);
        blink = 0;
        setTimer(TIMER_BLINK, 250);              // đổi trạng thái mỗi 250ms => 2Hz
        if (m == MODE_2) temp_val = dur_red;
        if (m == MODE_3) temp_val = dur_yellow;
        if (m == MODE_4) temp_val = dur_green;
    }
}

static void update_buffer(void) {
    if (mode == MODE_1) {
        led_buffer[0] = t1 / 10;  led_buffer[1] = t1 % 10;
        led_buffer[2] = t2 / 10;  led_buffer[3] = t2 % 10;
    } else {
        led_buffer[0] = mode / 10;      led_buffer[1] = mode % 10;       // đường 1: số mode
        led_buffer[2] = temp_val / 10;  led_buffer[3] = temp_val % 10;   // đường 2: giá trị
    }
}

// Nút 3: SET. Luôn giữ ràng buộc red = green + yellow
static void apply_value(void) {
    switch (mode) {
    case MODE_2:                                  // chỉnh đỏ
        if (temp_val > dur_yellow) {
            dur_red   = temp_val;
            dur_green = dur_red - dur_yellow;
        }
        break;
    case MODE_3:                                  // chỉnh vàng
        if (dur_green + temp_val <= 99) {
            dur_yellow = temp_val;
            dur_red    = dur_green + dur_yellow;
        }
        break;
    case MODE_4:                                  // chỉnh xanh
        if (temp_val + dur_yellow <= 99) {
            dur_green = temp_val;
            dur_red   = dur_green + dur_yellow;
        }
        break;
    }
}

void fsm_init(void) {
    setTimer(TIMER_SCAN, 10);
    enter_mode(MODE_1);
}

void fsm_run(void) {
    if (button_event[0]) {                        // nút 1: đổi mode 1->2->3->4->1
        button_event[0] = 0;
        enter_mode(mode % 4 + 1);
    }

    if (mode == MODE_1) {
        button_event[1] = 0;                      // bỏ qua nút 2, 3 ở chế độ thường
        button_event[2] = 0;
        if (timer_flag[TIMER_SEC]) {
            setTimer(TIMER_SEC, 1000);
            t1--; t2--; tState--;
            if (tState <= 0) enter_state((TrafficState)((ts + 1) % 4));
        }
    } else {
        if (button_event[1]) {                    // nút 2: tăng giá trị 1..99
            button_event[1] = 0;
            temp_val = (temp_val >= 99) ? 1 : temp_val + 1;
        }
        if (button_event[2]) {                    // nút 3: set giá trị
            button_event[2] = 0;
            apply_value();
        }
        if (timer_flag[TIMER_BLINK]) {            // nháy 2Hz
            setTimer(TIMER_BLINK, 250);
            blink = !blink;
            int c = (mode == MODE_2) ? C_RED : (mode == MODE_3) ? C_YELLOW : C_GREEN;
            setRoad1(blink ? c : C_OFF);
            setRoad2(blink ? c : C_OFF);
        }
    }
    update_buffer();
}
