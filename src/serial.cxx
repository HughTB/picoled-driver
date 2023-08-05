#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <csignal>

#include "serial.hxx"

int serial_open_port(const char* port, speed_t baud, unsigned char read_timeout_ds) {
    struct termios tty{};
    int serial_port = open(port, O_RDWR);

    // Check if the port was opened successfully
    if (serial_port < 0) {
        std::cout << "WARN: Failed to open port " << port << ": " << strerror(errno) << std::endl;
        return -1;
    }

    // Attempt to load existing settings for the serial port
    if (tcgetattr(serial_port, &tty) != 0) {
        std::cout << "WARN: Failed to get attributes for port " << port << ": " << strerror(errno) << std::endl;
        return -1;
    }

    // Set control flags
    tty.c_cflag &= ~PARENB; // Disable parity
    tty.c_cflag &= ~CSTOPB; // Set single stop bit
    tty.c_cflag &= ~CSIZE; // Clear byte size bits
    tty.c_cflag |= CS8; // 8 bits per byte
    tty.c_cflag &= ~CRTSCTS; // Disable RTC/CTS hardware flow control (not sure what this does but off works for the pico :D)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ, and disable Modem specific control lines

    tty.c_lflag &= ~ICANON; // Enable canonical mode
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure

    // Set read timeout to 0 bytes, read_timeout_dc deci-seconds
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = read_timeout_ds;

    // Set the baud rate
    cfsetspeed(&tty, baud);

    // Attempt to sent new settings for the serial port
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        std::cout << "WARN: Failed to set attributes for port " << port << ": " << strerror(errno) << std::endl;
        return -1;
    }

    return serial_port;
}

int serial_find_picoled(const char* port_path, speed_t baud) {
    char* port_buffer = (char*)calloc(strlen(port_path) + 3, sizeof(char));
    char* read_buffer = (char*)calloc(256, sizeof(char));

    for (int i = 0; i < 100; i++) {
        snprintf(port_buffer, strlen(port_path) + 3, "%s%i", port_path, i);

        std::cout << "INFO: Attempting connection to " << port_buffer << std::endl;

        int port = serial_open_port(port_buffer, baud, 10);

        if (port > 0) {
            serial_send_message(port, "ident:0,");

            if (serial_read_message(port, read_buffer, 256) > 0) {
                if (!strcmp("miaow", strtok(read_buffer, "\n")))
                    return port;
            }
        } else {
            std::cout << "WARN: Failed to connect to " << port_buffer << std::endl;
        }
    }

    return -1;
}

int serial_send_message(int serial_port, const char* message) {
    char* formatted_message = (char*)calloc(strlen(message) + 2, sizeof(char));
    snprintf(formatted_message, strlen(message) + 2, "%s\n", message);

    if (write(serial_port, formatted_message, strlen(formatted_message)) < 0) {
        std::cout << "WARN: Failed to send message on serial port " << serial_port << ": " << strerror(errno) << std::endl;
        return -1;
    }

    std::cout << "INFO: Sent message: " << message << std::endl;

    return 0;
}

long serial_read_message(int serial_port, char* read_buffer, size_t read_buffer_len) {
    long read_chars = 0;

    read_chars = read(serial_port, read_buffer, read_buffer_len);

    return read_chars;
}