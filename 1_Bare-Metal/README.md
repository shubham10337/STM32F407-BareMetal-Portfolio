# STM32F407 Bare-Metal Firmware Architecture

A professional-grade, bare-metal driver development portfolio for the STM32F407VG (ARM Cortex-M4). 

This repository systematically strips away hardware abstraction layers (HAL) to interact directly with the silicon, demonstrating deep mastery of memory mapping, bus matrices, and peripheral register configuration using the ST RM0090 Reference Manual.

## ⚙️ Core Architecture
* **Microcontroller:** STM32F407VGT6
* **Core:** ARM 32-bit Cortex-M4 with FPU
* **Toolchain:** Bare-metal C (No HAL), Custom Linker Script, CMSIS Device Headers
* **Hardware Verification:** Logic Analyzer Waveform Decoding

## 🛠️ Completed Bare-Metal Drivers

  ### 1. Bare-Metal GPIO & Safety-Critical Systems
* **Status:** COMPLETE
* **Bus:** AHB1 (`RCC_AHB1ENR`)
* **Engineered Applications:**
  * **Robotic Arm Emergency Interlock:** Designed a safety-critical polling loop utilizing `GPIOE_IDR` (Input) to instantly halt hardware execution on `GPIOB_ODR` (Output) upon emergency trigger detection.
  * **HMI Debounce Controller:** Architected a bare-metal switch debouncing algorithm for Port A (PA0) without relying on RTOS or HAL blocking delays.
  * **Direct Register Manipulation:** Handled all `MODER`, `ODR`, and `IDR` bitwise masking natively using custom `typedef struct` memory boundaries.

### 2. USART (Universal Synchronous Asynchronous Receiver Transmitter)
* **Status:** IN PROGRESS
* **Bus:** APB1
* **Pins:** PA2 (TX), PA3 (RX) - Alternate Function 7
* **Logic Analyzer Proof:** [Pending...]