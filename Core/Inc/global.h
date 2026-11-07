#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#define MODE_1 1
#define MODE_2 2
#define MODE_3 3
#define MODE_4 4

extern int mode;
extern int dur_red, dur_yellow, dur_green;   // đơn vị: giây
extern int temp_val;                         // giá trị đang chỉnh ở mode 2-4
extern int led_buffer[4];                    // 4 chữ số cần hiển thị

#endif /* INC_GLOBAL_H_ */
