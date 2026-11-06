# SPC Gear GK650K Omnis

*Full-size RGB keyboard on a EVision VS11K09A, with a volume wheel.*

* Keyboard Maintainer: [juliazero](https://github.com/juliazero)
* Hardware Supported: SPC Gear GK650K Omnis, including the Pudding Edition (US ANSI)
* Hardware Availability: [spcgear.com](https://spcgear.com)

## Keymap

#### Layer 1 — Fn:

<details>
<summary>Details</summary>

| Combination | Keycode | Function |
|---|---|---|
| `Fn + Esc` | `QK_BOOT` | Enter bootloader mode |
| `Fn + F1` | `KC_MYCM` | Open My Computer / Home folder |
| `Fn + F2` | `KC_WHOM` | Open the browser home page |
| `Fn + F3` | `KC_CALC` | Launch the calculator |
| `Fn + F4` | `KC_MSEL` | Launch the media player |
| `Fn + F5` | `KC_MPRV` | Previous track |
| `Fn + F6` | `KC_MNXT` | Next track |
| `Fn + F7` | `KC_MPLY` | Play / Pause |
| `Fn + F8` | `KC_MSTP` | Stop media playback |
| `Fn + F9` | `KC_MUTE` | Mute / Unmute |
| `Fn + F10` | `KC_VOLD` | Decrease volume |
| `Fn + F11` | `KC_VOLU` | Increase volume |
| `Fn + -` | `RM_NEXT` | Select the next RGB effect |
| `Fn + =` | `RM_TOGG` | Toggle RGB on/off |
| `Fn + [` | `RM_HUED` | Decrease hue |
| `Fn + ]` | `RM_HUEU` | Increase hue |
| `Fn + ↑` | `RM_VALU` | Increase keyboard brightness |
| `Fn + ↓` | `RM_VALD` | Decrease keyboard brightness |
| `Fn + ←` | `RM_SPDD` | Decrease animation speed |
| `Fn + →` | `RM_SPDU` | Increase animation speed |
| `Fn + <` | `RM_SATD` | Decrease saturation |
| `Fn + >` | `RM_SATU` | Increase saturation |
| `Fn + GUI` | `GU_TOGG` | Toggle the GUI modifier |

</details>

#### Rotary encoder:

<details>
<summary>Details</summary>

| Active layer | Counter-clockwise | Clockwise |
|---|---|---|
| 0: Base | Decrease volume | Increase volume |
| 1: Fn | Previous effect | Next effect |

</details>

## Instructions

#### Compiling from source:

<details>
<summary>Linux:</summary>

1. Install [arm-gnu-toolchain-15.x](https://learn.arm.com/install-guides/gcc/arm-gnu/)
2. Install [qmk cli](https://docs.qmk.fm/cli)
3. Clone repository:
   `git clone https://github.com/juliazero/qmk_firmware -b gk650k_omnis --recurse-submodules`
4. Compile firmware
   `qmk compile --clean -kb spcgear/gk650k_omnis -km <variant>`

</details>

<details>
<summary>Windows:</summary>

1. Install [qmk-msys](https://github.com/qmk/qmk_distro_msys/releases/tag/1.12.0) and run it
3. Clone repository:
   `git clone https://github.com/juliazero/qmk_firmware -b gk650k_omnis --recurse-submodules`
4. Compile firmware
   `qmk compile --clean -kb spcgear/gk650k_omnis -km <variant>`

</details>

#### Rebooting to bootloader:

<details>
<summary>HID Recovery method (Windows)</summary>

1. Install Python: `winget install Python.Python.3.14`
4. Open PowerShell/Windows Terminal as admin
5. Run: `python.exe ./spcgear_gk650k_hid_recovery-windows.py`
6. Keyboard should be booted in bootloader mode.
	
</details>

<details>
<summary>Dissasembly keyboard method</summary>

1. Remove keycaps and screws:
    <img width="50%" src="https://github.com/user-attachments/assets/f9afb3ff-127e-4302-ae91-8b893f816f78" />
3. Open keyboard from front
4. Wire the two pins (BOOT + GND) on the PCB before connecting the keyboard to USB.
    <img width="50%" alt="obraz" src="https://github.com/user-attachments/assets/c1d6e264-bbea-4a9b-bf10-d577e8f44706" />
	
5. Remove wire after keyboard is connected without LEDs.
	
</details>

<details>
<summary>When QMK firmware is installed</summary>
	
`Fn + Esc` is mapped to `QK_BOOT` and can be used to enter the bootloader.
	
</details>

#### Flashing:

<details>
<summary>Detalis</summary>

[SonixFlasherC](https://github.com/SonixQMK/SonixFlasherC/releases) is required

1. Check if keyboard is detected in bootloader mode: `sonixflasher -c`
2. Flash: `sonixflasher -v 0c45/7040 -f spcgear_gk650k_omnis_<variant>.bin`

</details>

#### Configuring:

<details>
<summary>VIA</summary>

1. Open https://usevia.app/
2. Go to Settings
3. Enable `Show design tab`
4. Go to DESIGN tab
7. Upload `spcgear_gk650k_omnis_via_definition.json`.

</details>

<details>
<summary>ColorHoster and OpenRGB</summary>

1. Download & extract [ColorHoster](https://github.com/Azarattum/ColorHoster/releases) and move it to eg. C:\ProgramFiles\
2. Copy your VIA keyboard configuration file to the configs folder (create it)
3. Run PowerShell or Windows Terminal as Administrator and execute:
`.\ColorHoster.exe -p 6743 -b -d "C:\Program Files\ColorHoster\configs" -s create`
4. Then start the service:
`.\ColorHoster.exe -s start`
6. In OpenRGB, go to Client SDK tab, set the port to 6743, click Connect, then click Save Connection.

</details>
