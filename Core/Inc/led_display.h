#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

enum { C_OFF, C_RED, C_GREEN, C_YELLOW };

void setRoad1(int color);
void setRoad2(int color);
void scan_display(void);

#endif /* INC_LED_DISPLAY_H_ */
