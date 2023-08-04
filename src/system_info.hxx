#ifndef PICOLED_DRIVER_LINUX_SYSTEM_INFO_HXX
#define PICOLED_DRIVER_LINUX_SYSTEM_INFO_HXX

/// \brief Obtains the current CPU usage from /proc/stat as a percentage
int sys_get_cpu_usage();

/// \brief Obtains the current memory usage from /proc/meminfo as a percentage
int sys_get_mem_usage();

#endif //PICOLED_DRIVER_LINUX_SYSTEM_INFO_HXX