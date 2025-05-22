#include "mbed.h"

InterruptIn flowSensor(D2);             // Connect the signal pin (Yellow) of YF-S201 to pin D2
BufferedSerial pc(USBTX, USBRX, 9600);  // BufferedSerial connection to the PC for monitoring at 9600 baud

// Constants
volatile int pulseCount = 0;            // Number of pulses counted
const float calibrationFactor = 7.5;     // 7.5 pulses per second per liter/minute

// Timer
Ticker flowTicker;

// Interrupt handler for counting pulses
void pulseCounter() {
    pulseCount++;
}

// Function to calculate and print flow rate
void calculateFlowRate() {
    // Calculate the flow rate in liters per minute and liters per second
    float flowRate = (pulseCount / calibrationFactor);  // Liters per minute
    float litersPerSec = flowRate / 60;                 // Liters per second
    
    // Convert to integers (x100 for two decimal precision)
    int flowRateInt = static_cast<int>(flowRate * 100);  // E.g., 5.25 L/min becomes 525
    int litersPerSecInt = static_cast<int>(litersPerSec * 100);

    // Prepare the data to send to serial monitor
    char buffer[64];
    int length = snprintf(buffer, sizeof(buffer), "Flow rate: %d.%02d L/min | %d.%02d L/sec\r\n", 
                          flowRateInt / 100, flowRateInt % 100, litersPerSecInt / 100, litersPerSecInt % 100);
    
    // Write the formatted data to the serial monitor
    pc.write(buffer, length);

    // Reset the pulse count
    pulseCount = 0;
}

int main() {
    // Attach the interrupt handler to the flow sensor
    flowSensor.rise(&pulseCounter);

    // Call calculateFlowRate every second to output the flow data
    flowTicker.attach(&calculateFlowRate, 1s);  // Use chrono 1 second interval

    while (true) {
        // Main loop does nothing, all work done in interrupt and Ticker functions
        ThisThread::sleep_for(1s);  // Use chrono for sleep duration
    }
}
