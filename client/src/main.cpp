#include <mbed.h>
#include <EthernetInterface.h>
#include <Timer.h>

// Initialize network interface
EthernetInterface eth;

#define IP "192.168.1.177"
#define GATEWAY "192.168.1.1"
#define MASK "255.255.255.0"

// Initialize timer
Timer t;

float main_function(int16_t *result, float *data)
{
    for(int i = 0; i < 10; i++)
    {
        data[i] = i;
    }
    wait(10);

    
    for(int i = 0; i < 10; i++)
    {
        result[i] = 9-i;
    }

    return 0;
}

int main(int, char **)
{
    nsapi_size_or_error_t  status;

    // Set up static IP
    eth.disconnect();
    status = eth.set_network(IP, MASK, GATEWAY);
    printf("set IP status: %i \r\n", status);

    // Start the network interface
    status = eth.connect();
    printf("Ethernet connection status: %i \r\n", status);

    // Create a socket
    TCPSocket socket;
    socket.open(&eth);

    // Connect
    status = socket.connect("192.168.1.72", 50000);
    printf("Socket connection status: %i \r\n", status);

    // Show the network address
    const char *ip = eth.get_ip_address();
    const char *mac = eth.get_mac_address();
    printf("IP address is: %s\n", ip ? ip : "No IP");
    printf("MAC address is: %s\n", mac ? mac : "No MAC");
    printf("The connection is established!\n\n\n");



    float buffer[10];
    int16_t result[10];

    while(true)
    {    
        // Receive data from the server
        status = socket.recv(buffer, sizeof(buffer));
        printf("Bytes recieved: %d\n", status);
        for(int i = 0; i < 10; i++)
        {
            printf("%.3f ", buffer[i]);
        }

        // Check recieved package and run the main_function
        if(status == sizeof(buffer))
        {
            main_function(result, buffer);

            printf("\n\n\nMain function performed!\n");
            for(int i = 0; i < 10; i++)
            {
                printf("%hi ", result[i]);
            }
            printf("\nDone!\n\n\n");
        }
        else
        {
            printf("Error! Bytes recieved (%d) != Size of buffer (%d)\n", status, sizeof(buffer));
        }


        // Send the result to the server
        // t.start();
        status = socket.send(result, sizeof(result));
        // t.stop();
        // printf("The time taken was %f seconds\n", t.read());
        if(status != sizeof(result))
        {
            printf("\nOnly %d bytes of the package (%d bytes) was sended!\n\n", status, sizeof(result));
        }
    }

    // Close the socket
    // status = socket.close();
    // printf("Socket closing status: %i \r\n", status);
}
