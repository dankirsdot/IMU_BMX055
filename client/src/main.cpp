#include <mbed.h>
#include <rtos.h>
#include <EthernetInterface.h>

#include <error.h>

#define IP "192.168.1.177"
#define GATEWAY "192.168.1.1"
#define MASK "255.255.255.0"

#define SERVER_IP "192.168.1.72"
#define SERVER_PORT 50000

EthernetInterface eth;

Thread thread;

// Thread function
void callback()
{
    nsapi_size_or_error_t SocketStatus;

    TCPSocket socket2;
    SocketStatus = socket2.open(&eth);
    check_nsapi_error("Socket 2 initialization status", SocketStatus, true);

    // Connect
    SocketStatus = socket2.connect(SERVER_IP, SERVER_PORT);
    check_nsapi_error("Socket 2 connection status", SocketStatus, true);
    printf("Socket 2 connection is established!\n\n");
}


void establish_ethernet_connection(EthernetInterface eth)
{
    nsapi_size_or_error_t EthernetStatus;

    eth.disconnect();
    EthernetStatus = eth.set_network(IP, MASK, GATEWAY);
    check_nsapi_error("Set IP status", EthernetStatus, true);

    EthernetStatus = eth.connect();
    check_nsapi_error("Ethernet connection status", EthernetStatus, true);

    const char *ip = eth.get_ip_address();
    const char *mac = eth.get_mac_address();
    printf("IP address is: %s\n", ip ? ip : "No IP");
    printf("MAC address is: %s\n", mac ? mac : "No MAC");
    printf("The Ethernet connection is established!\n\n");

    // Should we return something?
}


enum operation_modes
{
    SET_SETTINGS        =     0, /*!< Settings mode */
    ESTIMATE_PARAMETERS =     1, /*!< Estimate parameters */
    INITIALIZE_POSITION =     2, /*!< Move to the zero position */
    RUN_EXPERIMENT      =     3, /*!< Run the experiment */
};


int main(int, char **)
{
    // Initialize network interface
    establish_ethernet_connection(eth);

    nsapi_size_or_error_t SocketStatus;

    // Create a socket
    TCPSocket socket;
    SocketStatus = socket.open(&eth);
    check_nsapi_error("Socket initialization status", SocketStatus, true);

    // Connect
    SocketStatus = socket.connect(SERVER_IP, SERVER_PORT);
    check_nsapi_error("Socket connection status", SocketStatus, true);
    printf("Socket connection is established!\n\n");

    // Start the second socket
    thread.start(callback);

    ThisThread::sleep_for(5e+3); // wait 5 seconds

    int mode = 0;
    const char *result = "OK!";

    while (true)
    {
        // Receive data from the server
        SocketStatus = socket.recv(&mode, sizeof(mode));
        check_nsapi_error("Bytes recieved", SocketStatus, false);

        printf("Mode is %d.\n", mode);
        switch (mode)
        {
            case SET_SETTINGS:
                printf("The settings function!\n");
                break;
            case ESTIMATE_PARAMETERS:
                printf("The estimation function!\n");
                break;
            case INITIALIZE_POSITION:
                printf("The initialization function!\n");
                break;
            case RUN_EXPERIMENT:
                printf("The experiment thread!\n");
                break;
            default:
                printf("Wrong mode %d.\n", mode);
        }

        ThisThread::sleep_for(5e+3); // wait 5 seconds

        // Send the result to the server
        SocketStatus = socket.send(result, sizeof(result));
        check_nsapi_error("Bytes sended", SocketStatus, false);
    }
}