#include <mbed.h>
#include <rtos.h>
#include <EthernetInterface.h>

#include <string>

std::string nsapi_size_or_error_to_string(const nsapi_size_or_error_t status)
{
    switch (status) {
        case NSAPI_ERROR_OK:
            return "Success.";
        case NSAPI_ERROR_WOULD_BLOCK:
            return "No data is not available but call is non-blocking.";
        case NSAPI_ERROR_UNSUPPORTED:
            return "Unsupported functionality.";
        case NSAPI_ERROR_PARAMETER:
            return "Invalid configuration.";
        case NSAPI_ERROR_NO_CONNECTION:
            return "Not connected to a network.";
        case NSAPI_ERROR_NO_SOCKET:
            return "Socket not available for use.";
        case NSAPI_ERROR_NO_ADDRESS:
            return "IP address is not known.";
        case NSAPI_ERROR_NO_MEMORY:
            return "Memory resource not available.";
        case NSAPI_ERROR_NO_SSID:
            return "SSID not found.";
        case NSAPI_ERROR_DNS_FAILURE:
            return "DNS failed to complete successfully.";
        case NSAPI_ERROR_DHCP_FAILURE:
            return "DHCP failed to complete successfully.";
        case NSAPI_ERROR_AUTH_FAILURE:
            return "Connection to access point failed.";
        case NSAPI_ERROR_DEVICE_ERROR:
            return "Failure interfacing with the network processor.";
        case NSAPI_ERROR_IN_PROGRESS:
            return "Operation (eg connect) in progress.";
        case  NSAPI_ERROR_ALREADY:
            return "Operation (eg connect) already in progress.";
        case NSAPI_ERROR_IS_CONNECTED:
            return "Socket is already connected.";
        case NSAPI_ERROR_CONNECTION_LOST:
            return "Connection lost.";
        case NSAPI_ERROR_CONNECTION_TIMEOUT:
            return "Connection timed out.";
        case NSAPI_ERROR_ADDRESS_IN_USE:
            return "Address already in use.";
        case NSAPI_ERROR_TIMEOUT:
            return "Operation timed out.";
        case NSAPI_ERROR_BUSY:
            return "Device is busy and cannot accept new operation.";
        default:
            return "Unknown problem.";
    }
}

void soft_reset(void)
{
    printf("Soft reset.\n\n\n");
    ThisThread::sleep_for(1e+3); // wait a second
    NVIC_SystemReset();
}

void check_nsapi_error(std::string checkpoint, const nsapi_size_or_error_t status, const bool Print)
{
    if (status == 0 && Print)
        printf("%s: %s\n", checkpoint.c_str(), nsapi_size_or_error_to_string(status).c_str());
    else if (status > 0 && Print)
        printf("%s: %d.\n", checkpoint.c_str(), status);
    else if (status < 0)
    {
        printf("%s: %s Error code %d!\n", checkpoint.c_str(), nsapi_size_or_error_to_string(status).c_str(), status);
        soft_reset();
    }

    // Should we return something?
}