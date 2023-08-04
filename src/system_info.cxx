#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "system_info.hxx"

long previous_cpu_total = 0;
long previous_cpu_idle = 0;

int sys_get_cpu_usage() {
    size_t read_buffer_size = 48;
    char* read_buffer = (char*)calloc(read_buffer_size, sizeof(char));

    // Read stats from /proc/stat
    FILE* stat_file = fopen("/proc/stat", "r");
    fread(read_buffer, sizeof(char), read_buffer_size, stat_file);
    fclose(stat_file);

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

    FILE* stat_file = fopen("/proc/meminfo", "r");
    fread(read_buffer, sizeof(char), read_buffer_size, stat_file);
    fclose(stat_file);

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