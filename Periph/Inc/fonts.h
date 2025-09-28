/*
 * Filename: fonts.h
 * Description: The file contains for fonts code.
 *
 * Project: Simple Multitasking Logic
 * Platform: MicroChip ATTiny85
 * Created: 10.08.2025 07:35:51 PM
 * Author: Dmitry Slobodchikov
 */
#ifndef __FONTS_H_
#define __FONTS_H_


#include "main.h"

typedef uint8_t font_dot_5x7_t[6];
typedef uint8_t font_dot_10x14_t[24];

extern const font_dot_5x7_t font_dot_5x7[96];
extern const font_dot_10x14_t font_dot_10x14[96];


#endif /* __FONTS_H_ */