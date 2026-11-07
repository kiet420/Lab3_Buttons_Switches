#include "main.h"
#include "timer.h"
#include "input_reading.h"

// Schematic: +3.3V -- nút -- chân MCU -- R 1k -- GND  => nhấn = 1 (active-high)
#define BUTTON_IS_PRESSED   GPIO_PIN_SET
#define BUTTON_IS_RELEASED  GPIO_PIN_RESET
#define LONG_PRESS_COUNT    (1000 / BUTTON_READ_PERIOD)   // số lần đọc ứng với 1 giây

static GPIO_PinState buttonBuffer[NO_OF_BUTTONS] = {GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET};
static GPIO_PinState debounce1[NO_OF_BUTTONS] = {GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET};
static GPIO_PinState debounce2[NO_OF_BUTTONS] = {GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET};
static uint8_t  flagLong[NO_OF_BUTTONS];
static uint16_t counterLong[NO_OF_BUTTONS];

static GPIO_TypeDef *btnPort[NO_OF_BUTTONS] = {Button1_GPIO_Port, Button2_GPIO_Port, Button3_GPIO_Port};
static uint16_t      btnPin [NO_OF_BUTTONS] = {Button1_Pin, Button2_Pin, Button3_Pin};

void button_reading(void) {
    for (int i = 0; i < NO_OF_BUTTONS; i++) {
        debounce2[i] = debounce1[i];
        debounce1[i] = HAL_GPIO_ReadPin(btnPort[i], btnPin[i]);
        if (debounce1[i] == debounce2[i]) {
            buttonBuffer[i] = debounce1[i];
        }

        if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
            if (counterLong[i] < LONG_PRESS_COUNT) counterLong[i]++;
            else flagLong[i] = 1;
        } else {
            counterLong[i] = 0;
            flagLong[i] = 0;
        }
    }
}

unsigned char is_button_pressed(unsigned char index) {
    if (index >= NO_OF_BUTTONS) return 0;
    return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}

unsigned char is_button_pressed_1s(unsigned char index) {
    if (index >= NO_OF_BUTTONS) return 0;
    return (flagLong[index] == 1);
}
