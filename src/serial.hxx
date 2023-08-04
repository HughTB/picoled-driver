#ifndef PICOLED_DRIVER_LINUX_SERIAL_HXX
#define PICOLED_DRIVER_LINUX_SERIAL_HXX

#include <termios.h>

/// \brief Open the serial port specified
/// \param port The path to the serial port to be opened
/// \param baud The baud rate to request the port run at
/// \param read_timeout_ds The length of time in deci-seconds (0.1 second) to wait before timeout on read
int serial_open_port(const char* port, speed_t baud, unsigned char read_timeout_ds);

/// \brief Send a string over the serial port specified
/// \param serial_port The serial port, as obtained using serial_open_port
/// \param message The message to send over the serial port
int serial_send_message(int serial_port, const char* message);

#endif //PICOLED_DRIVER_LINUX_SERIAL_HXX