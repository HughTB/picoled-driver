#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>

#include "system_info.hxx"

long previous_cpu_total = 0;
long previous_cpu_idle = 0;

int sys_get_cpu_usage() {
    size_t read_buffer_size = 48;
    char* read_buffer = (char*)calloc(read_buffer_size, sizeof(char));

    // Read stats from /proc/stat
    FILE* file = fopen("/proc/stat", "r");
    fread(read_buffer, sizeof(char), read_buffer_size, file);
    fclose(file);

    // Discard the first token, it contains just "cpu"
    strtok(read_buffer, " ");

    char* token = strtok(nullptr, " ");
    long user = strtol(token, nullptr, 10);
    token = strtok(nullptr, " ");
    long nice = strtol(token, nullptr, 10);
    token = strtok(nullptr, " ");
    long system = strtol(token, nullptr, 10);
    token = strtok(nullptr, " ");
    long idle = strtol(token, nullptr, 10);
    token = strtok(nullptr, " ");
    long iowait = strtol(token, nullptr, 10);

    free(read_buffer);

    long total = user + nice + system + idle + iowait;

    long delta_total = total - previous_cpu_total;
    long delta_idle = idle - previous_cpu_idle;

    long cpu_percent = (1000 * (delta_total - delta_idle) / (delta_total + 5)) / 10;

    previous_cpu_total = total;
    previous_cpu_idle = idle;

    return (int)cpu_percent;
}

int sys_get_mem_usage() {
    size_t read_buffer_size = 256;
    char* read_buffer = (char*)calloc(read_buffer_size, sizeof(char));

    FILE* file = fopen("/proc/meminfo", "r");
    fread(read_buffer, sizeof(char), read_buffer_size, file);
    fclose(file);

    read_buffer[read_buffer_size - 1] = '\0';

    // For now, we only need the first 3 lines of /proc/meminfo, so don't bother gathering extra
    int no_lines = 3;
    char* lines[no_lines];
    char* line = strtok(read_buffer, "\n");

    for (int i = 0; i < no_lines; i++) {
        lines[i] = line;
        line = strtok(nullptr, "\n");
    }

    // Separate each token from the lines
    // There's probably a better way to do this (with only one for-loop) but I can't think of it right now
    int tokens_per_line = 3;
    char* tokens[tokens_per_line * no_lines];

    for (int i = 0; i < no_lines; i++) {
        char* token = strtok(lines[i], " ");

        for (int j = 0; j < tokens_per_line; j++) {
            tokens[i * tokens_per_line + j] = token;
            token = strtok(nullptr, " ");
        }
    }

    long total_mem = strtol(tokens[1], nullptr, 10); // Total system memory
    // long free_mem = strtol(tokens[4], nullptr, 10); // Memory that is completely free; not reserved
    long available_mem = strtol(tokens[7], nullptr, 10); // Memory that is possibly reserved, but not used

    free(read_buffer); // Make sure we don't leak memory

    long mem_percent = ((total_mem - available_mem) * 100) / total_mem; // *100 before division to not lose precision in integer conversion

    return (int)mem_percent; // Since this is a percentage, it should always be <=100, so int conversion doesn't matter
}

int sys_get_cpu_temp(const char* thermal_sensor) {
    if (thermal_sensor == nullptr)
        return 0;

    size_t read_buffer_size = 24; // If the temperature is more than 24 characters long, it's probably incorrect anyway
    char* read_buffer = (char*)calloc(read_buffer_size, sizeof(char));

    FILE* file = fopen(thermal_sensor, "r");
    fread(read_buffer, sizeof(char), read_buffer_size, file);
    fclose(file);

    read_buffer[read_buffer_size - 1] = '\0';

    // Since the file just contains the temperature, we can just parse it, but divide by 1000
    int temp = (int)(strtol(read_buffer, nullptr, 10) / 1000);

    free(read_buffer);

    return std::clamp(temp, 0, 255);
}

char* sys_find_cpu_sensor() {
    char* cpu_sensor = (char*)calloc(256, sizeof(char));
    char* read_buffer = (char*)calloc(64, sizeof(char));

    size_t read_chars = 0;

    // Loop through all 100 possible thermal sensors
    for (int i = 0; i < 100; i++) {
        snprintf(cpu_sensor, 256, "/sys/class/thermal/thermal_zone%i/type", i);

        FILE* file = fopen(cpu_sensor, "r");

        // If file failed to open, skip it
        if (file == nullptr)
            continue;

        read_chars = fread(read_buffer, sizeof(char), 64, file);
        fclose(file);

        read_buffer[read_chars] = '\0';

        // If it matches the type "x86_pkg_temp" then it is the CPU sensor
        if (!strcmp(read_buffer, "x86_pkg_temp\n")) {
            free(read_buffer);
            snprintf(cpu_sensor, 256, "/sys/class/thermal/thermal_zone%i/temp", i);

            return cpu_sensor;
        }
    }

    // Don't leak memory!
    free(cpu_sensor);
    free(read_buffer);

    // We didn't find anything
    return nullptr;
}