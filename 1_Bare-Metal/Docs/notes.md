# STM32 Bare-Metal Programming — Master Revision Notes  
Target MCU: STM32F411 (Black Pill)  
Goal: Register-level mastery of GPIO, Timers, Interrupts, PWM  

---

# 1. MEMORY + POINTER FOUNDATION (CORE CONCEPT)

## Memory Map (Hardware Reality)

Microcontroller memory is divided logically:

| Region | Base Address | Purpose |
-------|-------------|---------
FLASH | 0x08000000 | Program code storage
SRAM  | 0x20000000 | Variables, stack
PERIPHERALS | 0x40000000 | Hardware registers

Peripheral registers behave like RAM locations — but they control real hardware.

---

## Register Access Pattern

Always use:

```c
#define REG ((volatile uint32_t *)(ADDRESS))
````

### Why `volatile`?

Because:

• Hardware can change register values anytime
• Compiler must NOT cache values
• Every read/write must hit the actual address

Without volatile → compiler may optimize and break hardware control.

---

### Why `uint32_t`?

STM32 registers are 32-bit wide.
This guarantees correct access width.

---

## Bit Manipulation (Safe Register Control)

### Set Bit (Turn ON feature)

```c
REG |= (1 << n);
```

Effect:

• Keeps other bits unchanged
• Only sets bit `n`

---

### Clear Bit (Turn OFF feature)

```c
REG &= ~(1 << n);
```

Effect:

• Clears only selected bit
• Leaves others untouched

---

### Toggle Bit

```c
REG ^= (1 << n);
```

---

### Read Bit (IMPORTANT)

Used to check flags or pin states.

```c
(REG >> n) & 1
```

Example:

```c
if((TIM2_SR >> 0) & 1)
```

Meaning:

• Shift bit 0 to LSB position
• Mask everything except that bit
• Result = 1 → flag set
• Result = 0 → flag clear

Used heavily for:

• Status flags
• Button reads
• Polling loops

---

# 2. UNIVERSAL CONFIGURATION ORDER (MUST MEMORIZE)

Every peripheral follows this exact flow:

```
1. RCC Clock Enable
2. GPIO Mode Setup
3. Peripheral Register Configuration
4. Interrupt Enable (Optional)
5. Peripheral Enable (CR1 / Start bit)
```

### Why This Order?

Because:

• No clock = peripheral logic OFF
• GPIO must route signal to pin
• Configuration must be ready before running
• Enable LAST prevents glitches

---

# 3. RCC — CLOCK SYSTEM (POWER GATE)

All peripherals are OFF by default to save power.

---

## GPIO Clock Enable

```c
RCC_AHB1ENR |= (1<<0);   // GPIOA ON
```

---

## Timer Clock Enable (TIM2)

```c
RCC_APB1ENR |= (1<<0);   // TIM2 ON
```

---

### IMPORTANT RULE

Clock must be enabled BEFORE touching peripheral registers.

Otherwise:

• Writes silently fail
• Hardware ignores data

---

# 4. GPIO — PIN CONTROL

---

## MODER Register (2 bits per pin)

| Bits | Mode               |
| ---- | ------------------ |
| 00   | Input              |
| 01   | Output             |
| 10   | Alternate Function |
| 11   | Analog             |

---

### Example: PA0 as Alternate Function

```c
GPIOA_MODER &= ~(3<<0);
GPIOA_MODER |=  (2<<0);
```

Meaning:

• Disconnect normal GPIO logic
• Connect alternate hardware (timer, UART, etc)

---

## AFRL / AFRH — Alternate Function Selection

Each peripheral channel requires AF mapping.

Example:

TIM2_CH1 → PA0 → AF1

```c
GPIOA_AFRL |= (0x01 << 0);
```

This physically routes TIM2 output to pin.

---

# 5. TIMER FUNDAMENTALS (TIME ENGINE)

Timer = hardware counter

```
CNT increments → reaches ARR → overflow → reset → repeat
```

---

## Timer Mathematics (MOST IMPORTANT)

### Timer Tick Frequency

```text
Timer Tick = Timer Clock / (PSC + 1)
```

---

### Overflow Period

```text
Overflow Time = (ARR + 1) / Timer Tick Frequency
```

---

## Example Calculation (1 ms interrupt)

System Clock = 16 MHz

```
PSC = 15
ARR = 1000
```

Timer Tick:

```
16 MHz / 16 = 1 MHz
```

Tick Period:

```
1 µs
```

Overflow:

```
1000 × 1 µs = 1 ms
```

Interrupt rate:

```
1000 Hz
```

---

## Important Timer Registers

| Register | Purpose                 |
| -------- | ----------------------- |
| PSC      | Prescaler (slows clock) |
| ARR      | Auto Reload (limit)     |
| CNT      | Current counter value   |
| SR       | Status flags            |
| UIF      | Update flag (bit 0)     |
| EGR      | Event generator         |

---

# 6. FORCE UPDATE EVENT (VERY IMPORTANT)

## What is Update Event (UEV)?

Update event happens when:

• Timer overflows
• OR software forces it

It is responsible for:

• Transferring preload registers
• Reloading PSC
• Synchronizing shadow registers

---

## Why Force Update Is Needed?

PSC, ARR, CCR use **shadow registers**.

When you write:

```c
TIM2_PSC = value;
```

The value does NOT immediately affect hardware.

It waits until:

```
Update Event occurs
```

---

## What Problem Does This Prevent?

Without preload:

• ARR or CCR may change mid-cycle
• PWM waveform glitches
• Output timing becomes unstable

Shadow buffering ensures:

• Updates happen safely at period boundary
• PWM edges remain clean

---

## How To Force Update Manually

```c
TIM2_EGR |= (1<<0);   // UG bit
```

This forces:

• PSC reload
• ARR reload
• CCR shadow transfer
• Counter reset synchronization

Use after configuring timer.

---

# 7. INTERRUPTS — EVENT DRIVEN CONTROL

Polling wastes CPU.

Interrupt = hardware notifies CPU automatically.

---

## Interrupt Flow

```
Timer sets UIF
→ NVIC detects interrupt
→ CPU jumps to ISR
→ ISR clears flag
→ CPU returns
```

---

## Enable Timer Interrupt

### Peripheral side:

```c
TIM2_DIER |= (1<<0);
```

---

### NVIC side:

```c
NVIC_ISER0 |= (1<<28);
```

---

## ISR Naming Rule

Must match vector table exactly:

```c
void TIM2_IRQHandler(void)
```

Wrong name → Default_Handler → Infinite loop.

---

## Clearing Flag (MANDATORY)

Inside ISR:

```c
TIM2_SR &= ~(1<<0);
```

If not cleared:

• Interrupt keeps retriggering
• CPU stuck in ISR

---

# 8. PWM — HARDWARE SIGNAL ENGINE

PWM is created using:

```
Counter + Compare Logic + Output Pin
```

---

## PWM Mode 1 Logic

```
CNT < CCR → Output HIGH
CNT ≥ CCR → Output LOW
```

---

## PWM Frequency Formula

```text
PWM Frequency = Timer Clock / ((PSC+1) × (ARR+1))
```

---

## Duty Cycle Formula

```text
Duty % = (CCR / ARR) × 100
```

---

# 9. PWM CONFIGURATION SEQUENCE (WITH CODE)

### STEP 1 — Enable Timer Clock

```c
RCC_APB1ENR |= (1<<0);
```

---

### STEP 2 — Enable GPIO Clock

```c
RCC_AHB1ENR |= (1<<0);
```

---

### STEP 3 — Set Pin to Alternate Function

```c
GPIOA_MODER |= (2<<0);
```

---

### STEP 4 — Select Timer AF Mapping

```c
GPIOA_AFRL |= (0x01<<0);
```

---

### STEP 5 — Configure Prescaler

```c
TIM2_PSC = 15;
```

---

### STEP 6 — Configure ARR

```c
TIM2_ARR = 1000;
```

---

### STEP 7 — Enable ARR Preload

```c
TIM2_CR1 |= (1<<7);
```

---

### STEP 8 — Select PWM Mode

```c
TIM2_CCMR1 |= (6<<4);
```

---

### STEP 9 — Enable CCR Preload

```c
TIM2_CCMR1 |= (1<<3);
```

---

### STEP 10 — Enable Output Channel

```c
TIM2_CCER |= (1<<0);
```

---

### STEP 11 — Force Update Event

```c
TIM2_EGR |= (1<<0);
```

---

### STEP 12 — Start Timer (LAST)

```c
TIM2_CR1 |= (1<<0);
```

---

# 10. INTERRUPT BASED BREATHING LED (NON BLOCKING)

---

## Timer Interrupt Period

Example:

```
Interrupt every 1 ms
```

---

## Brightness Logic

Inside ISR:

```c
brightness += direction;
```

---

## Total Breathing Time

Steps:

```
0 → ARR → 0 = 2 × ARR steps
```

Time:

```
2 × ARR × interrupt_period
```

Example:

```
2 × 1000 × 1ms = 2 seconds
```

---

# 11. ISR TIMING RULE (REAL-TIME RULE)

ISR execution time must be:

```
<< Interrupt Period
```

Safe rule:

```
ISR < 10% of period
```

Your case:

ISR ≈ 1 µs
Period = 1000 µs

CPU Load = 0.1% (Excellent)

---

# 12. BUSY DELAY VS INTERRUPT

Busy Delay:

❌ CPU wasted
❌ inaccurate
❌ blocks tasks

Interrupt:

✅ precise
✅ scalable
✅ professional design

---

# 13. FREQUENCY VS RESOLUTION TRADEOFF

Higher ARR:

✔ Better brightness resolution
❌ Lower PWM frequency

Lower ARR:

✔ Higher PWM frequency
❌ Less resolution

Engineering tradeoff.

---

# 14. DEBUGGING CHECKLIST

If PWM or timer fails:

1. RCC enabled?
2. GPIO AF correct?
3. Timer running?
4. PWM mode set?
5. Output enabled?
6. Update event forced?
7. Flag cleared?
8. ISR name correct?

