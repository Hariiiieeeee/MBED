#include "mbed.h"

// Create a UART serial object (TX, RX pins)
UnbufferedSerial serial_port(D1, D0, 115200); // Change pins according to your board configuration

// Buffer for received data
char recv_buffer[100];

// Function to send data to NodeMCU
void sendDataToESP(const char *message) {
    serial_port.write(message, strlen(message));
    serial_port.write("\n", 1);  // Send newline character for NodeMCU to read as complete message
}

// Function to receive data from NodeMCU
void receiveDataFromESP() {
    if (serial_port.readable()) {
        // Read available data (non-blocking)
        int bytes_read = serial_port.read(recv_buffer, sizeof(recv_buffer) - 1);
        if (bytes_read > 0) {
            recv_buffer[bytes_read] = '\0';  // Null-terminate the string
            printf("Received from NodeMCU: %s\n", recv_buffer);
        }
    }
}

int main() {
    printf("Starting communication with NodeMCU...\n");

    while (true) {
        // Send initial data to NodeMCU
        sendDataToESP("Hello from STM32!");
        // Continuously check for data from NodeMCU
        receiveDataFromESP();
        ThisThread::sleep_for(500ms);  // Add delay to avoid excessive CPU usage
    }
}
