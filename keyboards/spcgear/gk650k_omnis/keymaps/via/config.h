// Copyright 2026 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* This part has 8 KB of RAM in total and VIA costs roughly half a kilobyte of it
 o ver the plain keymap - its* own USB endpoint plus the dynamic keymap
 machinery - which is enough to overflow the region. The emulated EEPROM is the
 largest single consumer left: wear_leveling keeps a RAM cache exactly as big
 as its logical size, 1 KB by default. Three quarters of that is enough here.
 A dynamic layer costs MATRIX_ROWS * MATRIX_COLS * 2 = 252 bytes plus the
 encoder map, and dynamic_keymap asserts that at least 100 bytes are left over
 for macros: two layers plus the header need about 570 bytes, so 768 holds them.
     The backing store must be a whole multiple of the logical size, hence 1536.
     That alone still left the region 32 bytes short, hence the shorter key hit
     history too - the buffer costs 1 + 5 * LED_HITS_TO_REMEMBER bytes, so going
     from eight to two frees 30 of them, and the reactive effects still look right
     because only the newest hits are visible anyway. */
#define WEAR_LEVELING_LOGICAL_SIZE 768
#define WEAR_LEVELING_BACKING_SIZE 1536
#define DYNAMIC_KEYMAP_LAYER_COUNT 2
#define LED_HITS_TO_REMEMBER 2
