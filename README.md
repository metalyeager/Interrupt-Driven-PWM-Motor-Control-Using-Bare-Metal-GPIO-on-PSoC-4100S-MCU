# Interrupt-Driven PWM Motor Control Using Bare-Metal GPIO on PSoC 4100S MCU

Interrupt-driven PWM motor control on a PSoC 4100S Plus using a bare-metal, register-level approach. TCPWM is configured for PWM generation, GPIO interrupts are used to toggle motor direction via an ISR, and all peripherals are initialized through direct register access.

## Overview

This project demonstrates PWM-based motor control on the PSoC 4100S Plus microcontroller using direct memory-mapped register configuration. The implementation focuses on low-level peripheral control.

An on-board switch connected to GPIO P3.7 generates an interrupt. The interrupt service routine (ISR) safely switches the motor direction by updating GPIO outputs and temporarily disabling PWM during transitions.

## Features

- PWM generation using TCPWM
- Register-level GPIO configuration
- On-board switch interrupt handling (GPIO P3.7)
- NVIC interrupt enable and clear
- ISR-based motor direction control
- Peripheral clock configuration using PERI dividers

## Hardware Platform

- PSoC 4100S Plus MCU
- On-board switch (GPIO P3.7)
- DC motor with driver circuitry
- External power supply (as required)

## Software Details

- Language: C  
- Toolchain: ARM GCC  
- Programming style: Bare-metal (direct register access)

## Project Structure

    BAREMETAL_MOTOR_PWM_INTR/
    ├── Application_Source/
    │   ├── main.c
    │   ├── user_cfg.c
    │   └── user_cfg.h
    ├── BSP/
    ├── Device_Drivers/
    ├── Libc_Device_Support/
    ├── Special_Libraries/
    ├── Startup/
    ├── linker_script.ld
    └── README.md

## Working Principle

1. Peripheral clocks are configured using PERI divider registers.
2. TCPWM is initialized in PWM mode with a fixed period and duty cycle.
3. GPIO pins are configured for PWM output and motor direction control.
4. The on-board switch at GPIO P3.7 is configured to generate an interrupt.
5. The ISR toggles motor direction and restores PWM operation.
6. The main loop remains empty, with all control handled through interrupts.

## Workspace Setup Instructions

To make this workspace work on your system, update the following paths according to your local installation.

### Update OpenOCD path in Makefile
- File: `Debug/Makefile`
- Line 18:
OCD = <YOUR_PATH>

### Update VS Code settings
- File: `.vscode/settings.json`
- Update:
- `OpenOCD_path`
- `local_gdbPath`

### Environment Variables (Windows)
Add the OpenOCD installation directory to the user PATH and restart the terminal.

## Notes

- Vendor-provided device files and startup code are included.
- Build artifacts are excluded from version control.

## License

This project is licensed under the MIT License. Vendor-provided files retain their original licenses.


