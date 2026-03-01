# STM32F407 Bare-Metal Firmware Architecture

A professional-grade, bare-metal driver development portfolio for the STM32F407VG (ARM Cortex-M4). 

This repository systematically strips away hardware abstraction layers (HAL) to interact directly with the silicon, demonstrating deep mastery of memory mapping, bus matrices, and peripheral register configuration using the ST RM0090 Reference Manual.

## ⚙️ Core Architecture
* **Microcontroller:** STM32F407VGT6
* **Core:** ARM 32-bit Cortex-M4 with FPU
* **Toolchain:** Bare-metal C (No HAL), Custom Linker Script, CMSIS Device Headers
* **Hardware Verification:** Logic Analyzer Waveform Decoding

## 🛠️ Completed Bare-Metal Drivers

### 1. USART (Universal Synchronous Asynchronous Receiver Transmitter)
* **Status:** IN PROGRESS
* **Bus:** APB1
* **Pins:** PA2 (TX), PA3 (RX) - Alternate Function 7
* **Logic Analyzer Proof:** [Pending...]