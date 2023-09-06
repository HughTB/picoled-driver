#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>

#include "serial.hxx"
#include "system_info.hxx"

#define WRITE_BUFFER_SIZE 256
#define READ_BUFFER_SIZE 256

volatile sig_atomic_t stop;

void sigint_handler(int signum) {
    if (signum == SIGINT) {
        stop = 1;
        std::cout << "INFO: Received SIGINT. Terminating..." << std::endl;
    }
}

int main() {
    signal(SIGINT, sigint_handler);
    stop = 0;

    std::cout << "INFO: Attempting to find CPU temperature sensor..." << std::endl;
    char* cpu_sensor = sys_find_cpu_sensor();

    if (cpu_sensor == nullptr) {
        std::cout << "WARN: Failed to find CPU temperature sensor. The temperature display will not function" << std::endl;
    }

    std::cout << "INFO: Found CPU package sensor at \"" << cpu_sensor << "\"" << std::endl;

    std::cout << "INFO: Attempting to find connected PicOLED device..." << std::endl;
    int port = serial_find_picoled("/dev/ttyACM", B115200);

    if (port < 0) {
        std::cerr << "ERROR: Failed to find PicOLED device... Exiting" << std::endl;
        return -1;
    }

    std::cout << "INFO: Found PicOLED device" << std::endl;

    char* write_buffer = (char*)calloc(WRITE_BUFFER_SIZE, sizeof(char));
    char* read_buffer = (char*)calloc(READ_BUFFER_SIZE, sizeof(char));

    while (!stop) {
        snprintf(write_buffer, 128, "cpu:%i,mem:%i,cpu_temp:%i,gpu_temp:%i",
                 sys_get_cpu_usage(), sys_get_mem_usage(), sys_get_temp(cpu_sensor), 0);

        serial_send_message(port, write_buffer);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // Make sure to free memory before terminating
    free(write_buffer);
    free(read_buffer);

    free(cpu_sensor);

    return 0;
}
