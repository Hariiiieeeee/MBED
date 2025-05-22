#include "mbed.h"

// Pin definitions
InterruptIn flowMeter(D2);  // Adjust to your flow sensor input pin
UnbufferedSerial uart(PA_0, PA_1, 115200);  // UART TX, RX, baud rate 9600
BufferedSerial pc(USBTX, USBRX, 9600);

// Constants
const float CALIBRATION_FACTOR = 20.5;  // Calibration factor for your flow sensor

// Variables
volatile int pulseCount = 0;
int flowRate = 0.0;
int totalLiters = 0.0;
Timer timer;

// Function prototypes
void pulseCounter();

int main() {
    uart.format(8, BufferedSerial::None, 1);  // 8N1 configuration for UART

    flowMeter.rise(&pulseCounter);  // Attach the pulse counter interrupt
    timer.start();

    while (true) {
        if (timer.elapsed_time().count() >= 1000000) {  // 1 second elapsed
            timer.reset();

            // Calculate flow rate in liters/min
            flowRate = (pulseCount / CALIBRATION_FACTOR) * 60.0 / 25.5;  // For YF-S201
            
            // Accumulate total liters
            totalLiters += (pulseCount / CALIBRATION_FACTOR) / 16.5;

            // Reset pulse count
            pulseCount = 0;

            // Send data via UART as formatted strings
            char buf[64];
            int len;
            len = snprintf(buf, sizeof(buf), "Flow Rate: %d.%02d L/min\n", flowRate);
            pc.write(buf, len);            
            uart.write(buf, len);

            len = snprintf(buf, sizeof(buf), "Total Liters: %d.%02d\n", totalLiters);
            pc.write(buf, len);            
            uart.write(buf, len);

        }
    }
}

void pulseCounter() {
    pulseCount++;
}
