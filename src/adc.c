#define RCC_APB2ENR       (*(volatile unsigned int*)(0x40023844))
#define ADC1_SR           (*(volatile unsigned int*)(0x40012000))
#define ADC1_CR1          (*(volatile unsigned int*)(0x40012004))
#define ADC1_CR2          (*(volatile unsigned int*)(0x40012008))
#define ADC1_SQR3         (*(volatile unsigned int*)(0x40012034))
#define ADC1_DR           (*(volatile unsigned int*)(0x4001204C))
#define ADC_CCR           (*(volatile unsigned int*)(0x40012304))

// System Control Block (SCB) - Coprocessor Access Control Register (CPACR)
#define SCB_CPACR (*((volatile unsigned int*)0xE000ED88))

void enable_fpu(void) {
    // Enable CP10 and CP11 full access (bits 20-23)
    SCB_CPACR |= (0xF << 20);
}

float read_internal_temp(void) {
    // 1. Enable ADC1 Clock
    RCC_APB2ENR |= (1 << 8);

    // 2. Wake up the Temperature Sensor (TSVREFE bit in ADC_CCR)
    ADC_CCR |= (1 << 23);

    // 3. Set Sample Time for Channel 18
    // Temperature sensor requires a minimum sample time (typically > 10us)
    // ADC1_SMPR1 (offset 0x0C). Ch 18 is bits 26:24. Set to '111' (480 cycles)
    (*(volatile unsigned int*)(0x4001200C)) |= (7 << 24);

    // 4. Set Channel 18 as the 1st conversion in regular sequence
    ADC1_SQR3 = 18;

    // 5. Turn on ADC
    ADC1_CR2 |= (1 << 0); // ADON bit

    // 6. Start Conversion (SWSTART bit)
    ADC1_CR2 |= (1 << 30);

    // 7. Wait for End of Conversion (EOC bit in SR)
    while (!(ADC1_SR & (1 << 1)));

    // 8. Read result
    unsigned int raw_value = ADC1_DR;

    // 9. Calculate Temperature
    // Formula from Datasheet: T(°C) = ((V_sense - V_25) / Avg_Slope) + 25
    // For STM32F411: V_25 ≈ 0.76V, Avg_Slope ≈ 2.5mV/°C
    float voltage = (float)raw_value * (3.3f / 4095.0f);
    float temperature = ((voltage - 0.76f) / 0.0025f) + 25.0f;

    return temperature;
}
