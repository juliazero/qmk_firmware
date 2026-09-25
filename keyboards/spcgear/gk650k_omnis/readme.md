# SPC Gear GK650K Omnis

*Full-size RGB keyboard on a EVision VS11K09A, with a volume wheel.*

* Keyboard Maintainer: [juliazero](https://github.com/juliazero)
* Hardware Supported: SPC Gear GK650K Omnis, including the Pudding Edition (US ANSI)
* Hardware Availability: [spcgear.com](https://spcgear.com)

Make for this keyboard (after setting up your build environment):

   qmk compile --clean -kb spcgear/gk650k_omnis -km default

Flashing for this keyboard:

   sonixflasher -v 0c45/7040 -f spcgear_gk650k_omnis_default.bin

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in following way:

* **Physical reset button**: short the `BOOT` pin (pin 3) to ground while plugging the keyboard in

## Fn layer

`Fn` is `MO(1)`: hold it to access layer 1. Keys not explicitly assigned on this
layer are `KC_TRNS` and fall through to the active lower layer.

### Fn layer — top row

| Combo        | Keycode   | Effect                                             |
| ------------ | --------- | -------------------------------------------------- |
| `Fn` + `Esc` | `QK_BOOT` | Put the keyboard into bootloader mode for flashing |
| `Fn` + `F1`  | `KC_MYCM` | Launch My Computer / Home folder                   |
| `Fn` + `F2`  | `KC_WHOM` | Open the web browser home page                     |
| `Fn` + `F3`  | `KC_CALC` | Launch the calculator                              |
| `Fn` + `F4`  | `KC_MSEL` | Launch the media player                            |
| `Fn` + `F5`  | `KC_MPRV` | Previous track                                     |
| `Fn` + `F6`  | `KC_MNXT` | Next track                                         |
| `Fn` + `F7`  | `KC_MPLY` | Play / Pause                                       |
| `Fn` + `F8`  | `KC_MSTP` | Stop media playback                                |
| `Fn` + `F9`  | `KC_VOLU` | Increase volume                                    |
| `Fn` + `F10` | `KC_VOLD` | Decrease volume                                    |
| `Fn` + `F11` | `KC_MUTE` | Mute / unmute audio                                |

### RGB Matrix

| Combo        | Keycode   | Effect                        |
| ------------ | --------- | ----------------------------- |
| `Fn` + `=`   | `RM_TOGG` | Toggle RGB Matrix on or off   |
| `Fn` + `-`   | `RM_NEXT` | Select the next RGB animation |
| `Fn` + `F11` | `KC_MUTE` | Mute / unmute audio           |

### Brightness and speed

| Combo      | Keycode   | Effect                                 |
| ---------- | --------- | -------------------------------------- |
| `Fn` + `↑` | `KC_BRIU` | Increase keyboard backlight brightness |
| `Fn` + `↓` | `KC_BRID` | Decrease keyboard backlight brightness |
| `Fn` + `←` | `RM_SPDD` | Decrease RGB animation speed           |
| `Fn` + `→` | `RM_SPDU` | Increase RGB animation speed           |

### Layer 2 — Fn + Menu

`Fn` + `Menu` activates layer 2 while held. Layer 2 provides additional RGB
Matrix controls; keys not explicitly assigned fall through to lower layers.

| Combo               | Keycode   | Effect                          |
| ------------------- | --------- | ------------------------------- |
| `Fn` + `Menu` + `-` | `RM_HUED` | Decrease hue                    |
| `Fn` + `Menu` + `=` | `RM_HUEU` | Increase hue                    |
| `Fn` + `Menu` + `↑` | `RM_VALU` | Increase RGB brightness |
| `Fn` + `Menu` + `←` | `RM_SATD` | Decrease saturation             |
| `Fn` + `Menu` + `↓` | `RM_VALD` | Decrease RGB brightness |
| `Fn` + `Menu` + `→` | `RM_SATU` | Increase saturation             |

### Other Fn functions

| Combo        | Keycode   | Effect                  |
| ------------ | --------- | ----------------------- |
| `Fn` + `GUI` | `GU_TOGG` | Toggle the GUI modifier |

### Encoder

The keyboard has one rotary encoder.

| Layer     | Counter-clockwise | Clockwise |
| --------- | ----------------- | --------- |
| Base      | `KC_VOLD`         | `KC_VOLU` |
| Fn        | `RM_PREV`         | `RM_NEXT` |
| Fn + Menu | `_______`         | `_______` |

On the base layer, the encoder controls system volume. On the Fn layer, it
selects the previous or next RGB Matrix animation. On layer 2, the encoder
uses `KC_TRNS` (`_______`) and therefore falls through to the lower active
layer.

