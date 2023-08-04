#include <iostream>
#include <chrono>
#include <thread>

#include "serial.hxx"
#include "system_info.hxx"

int main() {
    int port = serial_open_port("/dev/ttyACM0", B115200, 5);

    serial_send_message(port, "cpu:52,mem:70");

    char* send_buffer = (char*)calloc(128, sizeof(char));

    while (true) {
    //for (int i = 0; i < 20; i++) {
        snprintf(send_buffer, 128, "cpu:%i,mem:%i", sys_get_cpu_usage(), sys_get_mem_usage());

        serial_send_message(port, send_buffer);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}
