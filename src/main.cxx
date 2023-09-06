#include <iostream>
#include <chrono>
#include <thread>

#include "serial.hxx"
#include "system_info.hxx"

#define WRITE_BUFFER_SIZE 256
#define READ_BUFFER_SIZE 256

int main() {
    std::cout << "INFO: Attempting to find CPU temperature sensor..." << std::endl;
    char* cpu_sensor = sys_find_cpu_sensor();

    if (cpu_sensor == nullptr) {
        std::cout << "WARN: Failed to find CPU temperature sensor. The temperature display will not function" << std::endl;
    }

    std::cout << "INFO: Found CPU package sensor at \"" << cpu_sensor << "\"" << std::endl;

    int port = serial_find_picoled("/dev/ttyACM", B115200);

    if (port < 0) {
        std::cerr << "ERROR: Failed to find PicOLED device... Exiting" << std::endl;
        return -1;
    }

    char* write_buffer = (char*)calloc(WRITE_BUFFER_SIZE, sizeof(char));
    char* read_buffer = (char*)calloc(READ_BUFFER_SIZE, sizeof(char));

    while (true) {
        snprintf(write_buffer, 128, "cpu:%i,mem:%i,cpu_temp:%i,gpu_temp:%i",
                 sys_get_cpu_usage(), sys_get_mem_usage(), sys_get_temp(cpu_sensor), 0);

        serial_send_message(port, write_buffer);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    free(write_buffer);
    free(read_buffer);

    return 0;
}
