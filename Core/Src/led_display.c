#include "main.h"
#include "global.h"
#include "timer.h"
#include "led_display.h"

// 7-seg common anode: đoạn sáng khi chân MCU = 0
#define SEG_ACTIVE_LOW 1

static const uint8_t SEG_CODE[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

static void write_seg_bit(GPIO_TypeDef *port, uint16_t pin, uint8_t on) {
#if SEG_ACTIVE_LOW
    HAL_GPIO_WritePin(port, pin, on ? GPIO_PIN_RESET : GPIO_PIN_SET);
#else
    HAL_GPIO_WritePin(port, pin, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
#endif
}

static void display7SEG1(int n) {
    uint8_t v = SEG_CODE[n % 10];
    write_seg_bit(SEG1_0_GPIO_Port, SEG1_0_Pin, v & 0x01);
    write_seg_bit(SEG1_1_GPIO_Port, SEG1_1_Pin, v & 0x02);
    write_seg_bit(SEG1_2_GPIO_Port, SEG1_2_Pin, v & 0x04);
    write_seg_bit(SEG1_3_GPIO_Port, SEG1_3_Pin, v & 0x08);
    write_seg_bit(SEG1_4_GPIO_Port, SEG1_4_Pin, v & 0x10);
    write_seg_bit(SEG1_5_GPIO_Port, SEG1_5_Pin, v & 0x20);
    write_seg_bit(SEG1_6_GPIO_Port, SEG1_6_Pin, v & 0x40);
}

static void display7SEG2(int n) {
    uint8_t v = SEG_CODE[n % 10];
    write_seg_bit(SEG2_0_GPIO_Port, SEG2_0_Pin, v & 0x01);
    write_seg_bit(SEG2_1_GPIO_Port, SEG2_1_Pin, v & 0x02);
    write_seg_bit(SEG2_2_GPIO_Port, SEG2_2_Pin, v & 0x04);
    write_seg_bit(SEG2_3_GPIO_Port, SEG2_3_Pin, v & 0x08);
    write_seg_bit(SEG2_4_GPIO_Port, SEG2_4_Pin, v & 0x10);
    write_seg_bit(SEG2_5_GPIO_Port, SEG2_5_Pin, v & 0x20);
    write_seg_bit(SEG2_6_GPIO_Port, SEG2_6_Pin, v & 0x40);
}

void setRoad1(int color) {
    HAL_GPIO_WritePin(GPIOA, RED1_Pin | GREEN1_Pin | YELLOW1_Pin, GPIO_PIN_RESET);
    if (color == C_RED)    HAL_GPIO_WritePin(GPIOA, RED1_Pin,    GPIO_PIN_SET);
    if (color == C_GREEN)  HAL_GPIO_WritePin(GPIOA, GREEN1_Pin,  GPIO_PIN_SET);
    if (color == C_YELLOW) HAL_GPIO_WritePin(GPIOA, YELLOW1_Pin, GPIO_PIN_SET);
}

void setRoad2(int color) {
    HAL_GPIO_WritePin(GPIOA, RED2_Pin | GREEN2_Pin | YELLOW2_Pin, GPIO_PIN_RESET);
    if (color == C_RED)    HAL_GPIO_WritePin(GPIOA, RED2_Pin,    GPIO_PIN_SET);
    if (color == C_GREEN)  HAL_GPIO_WritePin(GPIOA, GREEN2_Pin,  GPIO_PIN_SET);
    if (color == C_YELLOW) HAL_GPIO_WritePin(GPIOA, YELLOW2_Pin, GPIO_PIN_SET);
}

static void update7SEG(int index) {
    HAL_GPIO_WritePin(GPIOA, EN0_Pin | EN1_Pin | EN2_Pin | EN3_Pin, GPIO_PIN_SET);
    switch (index) {
    case 0: display7SEG1(led_buffer[0]); HAL_GPIO_WritePin(GPIOA, EN0_Pin, GPIO_PIN_RESET); break;
    case 1: display7SEG1(led_buffer[1]); HAL_GPIO_WritePin(GPIOA, EN1_Pin, GPIO_PIN_RESET); break;
    case 2: display7SEG2(led_buffer[2]); HAL_GPIO_WritePin(GPIOA, EN2_Pin, GPIO_PIN_RESET); break;
    case 3: display7SEG2(led_buffer[3]); HAL_GPIO_WritePin(GPIOA, EN3_Pin, GPIO_PIN_RESET); break;
    }
}

void scan_display(void) {
    static int idx = 0;
    if (timer_flag[TIMER_SCAN]) {
        setTimer(TIMER_SCAN, 250);
        update7SEG(idx);
        idx = (idx + 1) % 4;
    }
}
