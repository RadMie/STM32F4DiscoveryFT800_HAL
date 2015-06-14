#ifndef __co_processor_H
#define __eve_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "ft_gpu.h"
#include "ft800.h"

extern unsigned int dli, cli;

void write_string(char *s);
void cmd(uint32_t cmd);
void cmd_dlstart(void);
void cmd_swap(void);
void cmd_coldstart(void);
void cmd_interrupt(uint32_t ms);
void cmd_append(uint32_t ptr, uint32_t num);
void cmd_regread(uint32_t ptr, uint32_t result);
void cmd_memwrite(uint32_t ptr, uint32_t num);
void cmd_inflate(uint32_t ptr);
void cmd_loadimage(const uint8_t* img, uint32_t ptr, uint32_t length, uint32_t opt);
void cmd_memcrc(uint32_t ptr, uint32_t num, uint32_t result);
void cmd_memzero(uint32_t ptr, uint32_t num);
void cmd_memset(uint32_t ptr, uint32_t value, uint32_t num);
void cmd_memcpy(uint32_t dest, uint32_t src, uint32_t num);
void cmd_button(int16_t x, int16_t y, int16_t w, int16_t h, int16_t font, int16_t opt, const char*s);
void cmd_clock(int16_t x, int16_t y, int16_t r, uint16_t opt, uint16_t h, uint16_t m, uint16_t s, uint16_t ms);
void cmd_fgcolor(uint32_t color);
void cmd_bgcolor(uint32_t color);
void cmd_gradcolor(uint32_t color);
void cmd_gauge(int16_t x, int16_t y, int16_t r, uint16_t opt, uint16_t major, uint16_t minor, uint16_t val, uint16_t range);
void cmd_gradient(int16_t x0, int16_t y0, uint32_t rgb0, int16_t x1, int16_t y1, uint32_t rgb1);
void cmd_keys(int16_t x, int16_t y, int16_t w, int16_t h, int16_t font, uint16_t opt, const char* s);
void cmd_progress(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t opt, uint16_t val, uint16_t range);
void cmd_scrollbar(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t opt, uint16_t val, uint16_t size, uint16_t range);
void cmd_slider(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t opt, uint16_t val, uint16_t range);
void cmd_dial(int16_t x, int16_t y, int16_t r, uint16_t opt, uint16_t val);
void cmd_toggle(int16_t x, int16_t y, int16_t w, int16_t font, uint16_t opt, uint16_t state, const char* s);
void cmd_text(int16_t x, int16_t y, int16_t font, uint16_t opt, const char* s);
void cmd_number(int16_t x, int16_t y, int16_t font, uint16_t opt, int32_t n);
void cmd_loadidentity(void);
void cmd_setmatrix(void);
void cmd_getmatrix(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f);
void cmd_getptr(uint32_t result);
void cmd_getprops(uint32_t ptr, uint32_t width, uint32_t height);
void cmd_scale(int32_t sx, int32_t sy);
void cmd_rotate(int32_t a);
void cmd_translate(int32_t tx, int32_t ty);
void cmd_calibrate(void);
void cmd_spinner(int16_t x, int16_t y, uint16_t style, uint16_t scale);
void cmd_screensaver(void);
void cmd_sketch(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t ptr, uint16_t format);
void cmd_stop(void);
void cmd_setfont(uint32_t font, uint32_t ptr);
void cmd_track(int16_t x, int16_t y, int16_t w, int16_t h, int16_t tag);
void cmd_snapshot(uint32_t ptr);
void cmd_logo(void);



#endif /*__co_processor_H */
