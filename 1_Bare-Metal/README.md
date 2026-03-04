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
* **Status:** IN PROGRESS (Hardware Validation Pending)
* **Bus:** APB1 (`RCC_APB1ENR`)
* **Pins:** PA2 (TX), PA3 (RX) - Alternate Function 7
* **Engineered Applications:**
  * **Fixed-Point Baud Rate Generation:** Computed and injected precise fractional dividers (`USARTDIV`) into the `BRR` register to synthesize a 115200 baud rate from the 16MHz HSI clock.
  * **Standard Library Retargeting:** Intercepted the newlib C library `_write` system call to hijack `printf`, routing formatted character arrays directly to the bare-metal transmission engine.
  * **Asynchronous Polling Engine:** Architected a temporal roadblock utilizing the `TXE` (Transmit Data Register Empty) status flag to synchronize the 16MHz CPU with the 115.2kHz serial wire, preventing data collision.
* **Logic Analyzer Proof:** [Pending FT232 Validation...]