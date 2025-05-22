#include "mbed.h"
#include "WiFiInterface.h"
#include "socket.h"

DigitalOut valvePin(PA_5); // Replace with your valve control pin
WiFiInterface *wifi; // Wi-Fi interface

const char *ssid = "chittuKuruvi"; // Your Wi-Fi SSID
const char *password = "1234hari"; // Your Wi-Fi Password

int main() {
    wifi = WiFiInterface::get_default_instance(); // Get the default Wi-Fi interface
    if (!wifi) {
        printf("No Wi-Fi interface found.\n");
        return -1;
    }

    // Connect to Wi-Fi
    printf("Connecting to Wi-Fi...\n");
    int connection_status = wifi->connect(ssid, password, NSAPI_SECURITY_WPA2);
    if (connection_status != 0) {
        printf("Connection error: %d\n", connection_status);
        return -1;
    }
    
    printf("Connected to Wi-Fi. IP Address: %s\n", wifi->get_ip_address());

    // Create a TCP socket
    TCPSocket socket;
    socket.open(wifi);
    socket.bind(80); // Bind to port 80 (HTTP)
    socket.listen();

    while (true) {
        // Accept incoming connection
        TCPSocket *client = socket.accept();
        printf("New client connected!\n");

        // Read the HTTP request
        char buffer[256];
        int bytes_received = client->recv(buffer, sizeof(buffer) - 1);
        buffer[bytes_received] = '\0'; // Null-terminate the string

        // Log the received request
        printf("Received request: %s\n", buffer);

        // Handle the request
        if (strstr(buffer, "GET /valve/on") != nullptr) {
            valvePin = 1; // Turn the valve ON
            client->send("HTTP/1.1 200 OK\r\n\r\nValve is ON", 1000);
        } else if (strstr(buffer, "GET /valve/off") != nullptr) {
            valvePin = 0; // Turn the valve OFF
            client->send("HTTP/1.1 200 OK\r\n\r\nValve is OFF", 1000);
        } else {
            client->send("HTTP/1.1 404 Not Found\r\n\r\n", 1000);
        }

        // Close the client connection
        client->close();
        delete client; // Clean up the client socket
    }
}
