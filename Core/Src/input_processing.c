#include "main.h"
#include "timer.h"
#include "input_reading.h"
#include "input_processing.h"

typedef enum { BTN_RELEASED, BTN_PRESSED, BTN_PRESSED_1S } BtnState;

static BtnState state[3] = {BTN_RELEASED, BTN_RELEASED, BTN_RELEASED};
static uint8_t armed[3] = {0, 0, 0};   // chỉ nhận nhấn sau khi đã thấy nút thả
int button_event[3] = {0, 0, 0};

void fsm_for_input_processing(void) {
    for (int i = 0; i < 3; i++) {
        switch (state[i]) {
        case BTN_RELEASED:
            if (is_button_pressed(i)) {
                state[i] = BTN_PRESSED;
                if (armed[i]) button_event[i] = 1;   // bỏ qua mức "nhấn" lúc mới boot
            } else {
                armed[i] = 1;
            }
            break;

        case BTN_PRESSED:
            if (!is_button_pressed(i)) {
                state[i] = BTN_RELEASED;
            } else if (is_button_pressed_1s(i)) {
                state[i] = BTN_PRESSED_1S;
                setTimer(TIMER_REPEAT, 200);         // giữ lâu: lặp mỗi 200ms
            }
            break;

        case BTN_PRESSED_1S:
            if (!is_button_pressed(i)) {
                state[i] = BTN_RELEASED;
            } else if (i == 1 && timer_flag[TIMER_REPEAT]) {   // chỉ nút 2 tự lặp
                setTimer(TIMER_REPEAT, 200);
                button_event[1] = 1;
            }
            break;
        }
    }
}
