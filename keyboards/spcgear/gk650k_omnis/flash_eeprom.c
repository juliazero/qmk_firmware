// Copyright 2026 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
//
// EEPROM for this board: the stock wear leveling on the embedded flash, with two
// board specific workarounds layered on top, so drivers/ and platforms/ can stay
// untouched.
//
// 1. Park the LED matrix around flash writes.
//    The SN32F240B flash is single bank: during a page erase or word program the
//    CPU cannot fetch, so the RGB interrupt that walks the 18 anode rows stops.
//    The PWM timer keeps running, though, and whichever row was selected stays on
//    at full duty instead of its 1/18 share - a bright stripe for as long as the
//    write takes, which for a wear leveling consolidation (erasing the whole
//    backing store) is a noticeable moment. The driver's own guard only skips its
//    update while EFLD1 is in FLASH_PGM, which an interrupt almost never gets to
//    see: the first flash operation starts microseconds after the unlock, well
//    before the next PWM period. So the backend's unlock/lock are wrapped here,
//    synchronously: stop the RGB interrupt, drop every anode, then write. A dark
//    blip of well under a millisecond is invisible where the stripe was not.
//
// 2. Hold back RGB matrix settings until they stop changing.
//    rgb_matrix writes its config on the frame after every change, and VIA's
//    lighting "save" arrives after every slider step. Each of those is a flash
//    write and a few entries in the write log, and a full log means the next
//    consolidation - so an encoder spin could cost a freeze. Writes that fall
//    inside the rgb_matrix config are kept in RAM here and committed once the
//    value has been stable for GK650K_RGB_EECONFIG_FLUSH_DELAY, or right away on
//    shutdown/suspend. Reads see the pending bytes, so nothing upstream notices.

#include <string.h>

// The stock EFL backend, with lock/unlock renamed so they can be wrapped below.
#define backing_store_unlock efl_backing_store_unlock
#define backing_store_lock efl_backing_store_lock
#include "wear_leveling_efl.c"
#undef backing_store_unlock
#undef backing_store_lock

#include "quantum.h"
#include "eeprom_driver.h"
#include "nvm_eeprom_eeconfig_internal.h"

bool backing_store_unlock(void);
bool backing_store_lock(void);

/* ---- LED parking ---------------------------------------------------------- */

static const pin_t led_anodes[] = SN32F2XX_RGB_MATRIX_ROW_PINS;
static bool        leds_parked  = false;

static void leds_park(void) {
    // EEPROM comes up before the RGB matrix; nothing is being driven until then.
    if (PWMD1.state != PWM_READY) return;

    chSysLock();
    // The driver's callback re-arms this itself at the end of every run, so it
    // cannot run again after this - and drop a match that already latched, or it
    // would fire on unlock and re-arm the notification behind our back.
    pwmDisablePeriodicNotificationI(&PWMD1);
    nvicClearPending(SN32_CT16B1_NUMBER);
    chSysUnlock();

    // Anodes are active high (SN32F2XX_RGB_OUTPUT_ACTIVE_HIGH): low is off. With
    // every row off the column PWM has nothing to light.
    for (uint8_t i = 0; i < ARRAY_SIZE(led_anodes); i++) {
        gpio_write_pin_low(led_anodes[i]);
    }
    leds_parked = true;
}

static void leds_resume(void) {
    if (!leds_parked) return;
    leds_parked = false;
    // The next period's callback selects its row and re-enables the channels.
    pwmEnablePeriodicNotification(&PWMD1);
}

bool backing_store_unlock(void) {
    leds_park();
    if (!efl_backing_store_unlock()) {
        leds_resume();
        return false;
    }
    return true;
}

bool backing_store_lock(void) {
    bool ok = efl_backing_store_lock();
    leds_resume();
    return ok;
}

/* ---- EEPROM driver, deferring the rgb_matrix config ----------------------- */

#define RGB_EE_ADDR ((uint32_t)EECONFIG_RGB_MATRIX)
#define RGB_EE_LEN (sizeof(rgb_config_t))

static uint8_t  rgb_ee_pending[RGB_EE_LEN];
static bool     rgb_ee_dirty = false;
static bool     rgb_ee_defer = false; // off until init is done, so first-boot defaults land at once
static uint32_t rgb_ee_timer = 0;

static void rgb_ee_flush(void) {
    if (!rgb_ee_dirty) return;
    rgb_ee_dirty = false;
    wear_leveling_write(RGB_EE_ADDR, rgb_ee_pending, RGB_EE_LEN);
}

void eeprom_driver_init(void) {
    wear_leveling_init();
}

void eeprom_driver_format(bool erase) {
    // Wear leveling needs its log structures erased before use either way.
    (void)erase;
    eeprom_driver_erase();
}

void eeprom_driver_erase(void) {
    rgb_ee_dirty = false;
    wear_leveling_erase();
}

void eeprom_read_block(void *buf, const void *addr, size_t len) {
    uint32_t a = (uint32_t)addr;
    wear_leveling_read(a, buf, len);
    if (!rgb_ee_dirty) return;
    for (size_t i = 0; i < len; i++) {
        if (a + i >= RGB_EE_ADDR && a + i < RGB_EE_ADDR + RGB_EE_LEN) {
            ((uint8_t *)buf)[i] = rgb_ee_pending[a + i - RGB_EE_ADDR];
        }
    }
}

void eeprom_write_block(const void *buf, void *addr, size_t len) {
    uint32_t a = (uint32_t)addr;
    if (rgb_ee_defer && a >= RGB_EE_ADDR && a + len <= RGB_EE_ADDR + RGB_EE_LEN) {
        if (!rgb_ee_dirty) {
            wear_leveling_read(RGB_EE_ADDR, rgb_ee_pending, RGB_EE_LEN);
        }
        memcpy(&rgb_ee_pending[a - RGB_EE_ADDR], buf, len);
        rgb_ee_dirty = true;
        rgb_ee_timer = timer_read32();
        return;
    }
    // Anything else touching the held range goes out after it, in order.
    if (rgb_ee_dirty && a < RGB_EE_ADDR + RGB_EE_LEN && a + len > RGB_EE_ADDR) {
        rgb_ee_flush();
    }
    wear_leveling_write(a, buf, len);
}

void keyboard_post_init_kb(void) {
    rgb_ee_defer = true;
    keyboard_post_init_user();
}

void housekeeping_task_kb(void) {
    if (rgb_ee_dirty && timer_elapsed32(rgb_ee_timer) >= GK650K_RGB_EECONFIG_FLUSH_DELAY) {
        rgb_ee_flush();
    }
    housekeeping_task_user();
}

void suspend_power_down_kb(void) {
    rgb_ee_flush(); // the host may cut power while asleep
    suspend_power_down_user();
}

bool shutdown_kb(bool jump_to_bootloader) {
    rgb_ee_flush(); // QK_BOOT, soft reset, EE_CLR
    return shutdown_user(jump_to_bootloader);
}
