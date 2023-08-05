#include <iostream>
#include <chrono>
#include <thread>

#include "serial.hxx"
#include "system_info.hxx"

#define WRITE_BUFFER_SIZE 256
#define READ_BUFFER_SIZE 256

int main() {
    int port = serial_find_picoled("/dev/ttyACM", B115200);

    if (port < 0) {
        std::cout << "ERROR: Failed to find PicOLED device... Exiting" << std::endl;
        return -1;
    }

    char* write_buffer = (char*)calloc(WRITE_BUFFER_SIZE, sizeof(char));
    char* read_buffer = (char*)calloc(READ_BUFFER_SIZE, sizeof(char));

    while (true) {
        snprintf(write_buffer, 128, "cpu:%i,mem:%i", sys_get_cpu_usage(), sys_get_mem_usage());

        serial_send_message(port, write_buffer);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}
