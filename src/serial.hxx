#ifndef PICOLED_DRIVER_LINUX_SERIAL_HXX
#define PICOLED_DRIVER_LINUX_SERIAL_HXX

#include <termios.h>

/// \brief Open the serial port specified
/// \param port The path to the serial port to be opened
/// \param baud The baud rate to request the port run at
/// \param read_timeout_ds The length of time in deci-seconds (0.1 second) to wait before timeout on read
int serial_open_port(const char* port, speed_t baud, unsigned char read_timeout_ds);

int serial_find_picoled(const char* port_path, speed_t baud);

/// \brief Send a string over the serial port specified
/// \param serial_port The serial port, as obtained using serial_open_port
/// \param message The message to send over the serial port
int serial_send_message(int serial_port, const char* message);

/// \brief Read a message from the serial port specified
/// \param serial_port The serial port, as obtained using serial_open_port
/// \param read_buffer Pointer to the buffer to read data into
/// \param read_buffer_len Length of the specified read buffer
long serial_read_message(int serial_port, char* read_buffer, size_t read_buffer_len);

#endif //PICOLED_DRIVER_LINUX_SERIAL_HXX