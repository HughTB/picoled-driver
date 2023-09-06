#ifndef PICOLED_DRIVER_LINUX_SYSTEM_INFO_HXX
#define PICOLED_DRIVER_LINUX_SYSTEM_INFO_HXX

/// \brief Obtains the current CPU usage from /proc/stat as a percentage
int sys_get_cpu_usage();

/// \brief Obtains the current memory usage from /proc/meminfo as a percentage
int sys_get_mem_usage();

/// \brief Obtains the current CPU temperature from the sensor specified
int sys_get_cpu_temp(const char* thermal_sensor);

/// \brief Find the CPU temperature sensor for use with sys_get_cpu_temp
char* sys_find_cpu_sensor();

#endif //PICOLED_DRIVER_LINUX_SYSTEM_INFO_HXX