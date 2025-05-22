#include "mbed.h"
#include "stm32l4xx_hal.h"

// I2S handle
I2S_HandleTypeDef hi2s2;

// Initialize the I2S interface
void init_i2s() {
    __HAL_RCC_SPI2_CLK_ENABLE();  // Enable the I2S clock

    hi2s2.Instance = SPI2;  // SPI2 used as I2S2
    hi2s2.Init.Mode = I2S_MODE_MASTER_RX;  // Master receive mode
    hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;  // Philips I2S standard
    hi2s2.Init.DataFormat = I2S_DATAFORMAT_16B;  // 16-bit data format
    hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;  // No MCLK output
    hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_16K;  // Audio frequency set to 16 kHz
    hi2s2.Init.CPOL = I2S_CPOL_LOW;  // Clock polarity low
    hi2s2.Init.ClockSource = I2S_CLOCK_PLL;  // Clock source
    hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;  // Simplex mode

    if (HAL_I2S_Init(&hi2s2) != HAL_OK) {
        // Initialization Error
        printf("I2S initialization failed!\n");
    } else {
        printf("I2S initialized successfully.\n");
    }
}

// Function to record audio for 10 seconds every 2 minutes
void record_audio_for_10_seconds() {
    uint16_t audioBuffer[512];  // Adjust size as needed
    Timer timer;

    while (true) {
        printf("Recording started...\n");
        timer.reset();
        timer.start();

        while (timer.elapsed_time().count() < 10'000'000) {  // 10 seconds in microseconds
            if (HAL_I2S_Receive(&hi2s2, audioBuffer, sizeof(audioBuffer) / sizeof(uint16_t), HAL_MAX_DELAY) == HAL_OK) {
                // Process or store the audio data here
                printf("Audio data received.\n");
            } else {
                printf("I2S receive error.\n");
            }
            ThisThread::sleep_for(100ms);  // Optional stabilization delay
        }

        timer.stop();
        printf("Recording finished. Sleeping for 2 minutes.\n");
        ThisThread::sleep_for(2min);  // Sleep for 2 minutes
    }
}

int main() {
    printf("Starting audio recording with I2S on STM32L475E...\n");
    init_i2s();
    record_audio_for_10_seconds();  // Start periodic audio recording
}
