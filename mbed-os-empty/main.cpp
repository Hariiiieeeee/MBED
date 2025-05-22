#include "mbed.h"
#include "WiFiInterface.h"
#include "Socket.h"

WiFiInterface *wifi; // Wi-Fi interface

const char *ssid = "YOUR_SSID"; // Your Wi-Fi SSID
const char *password = "YOUR_PASSWORD"; // Your Wi-Fi Password

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

    // Create a Socket
    TCPSocket socket;
    socket.open(wifi);
    
    // Create a SocketAddress to hold the local IP address
    SocketAddress local_address;
    socket.getsockname(&local_address); // Get the local address

    // Print the local IP address
    printf("Connected to Wi-Fi. Local IP Address: %s\n", local_address.get_ip_address());

    // Your main code goes here...

    return 0;
}
