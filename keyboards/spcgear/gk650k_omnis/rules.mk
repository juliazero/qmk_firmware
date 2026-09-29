LTO_ENABLE = yes

# EEPROM: stock wear leveling on the embedded flash, with the backend supplied by
# flash_eeprom.c so flash writes can be kept away from the LED matrix.
EEPROM_DRIVER = custom
WEAR_LEVELING_DRIVER = custom
OPT_DEFS += -DHAL_USE_EFL
SRC += flash_eeprom.c
