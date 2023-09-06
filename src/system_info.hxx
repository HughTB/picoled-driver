#ifndef PICOLED_DRIVER_LINUX_SYSTEM_INFO_HXX
#define PICOLED_DRIVER_LINUX_SYSTEM_INFO_HXX

#include <vector>

/// \brief Obtains the current CPU usage from /proc/stat as a percentage
int sys_get_cpu_usage();

/// \brief Obtains the current memory usage from /proc/meminfo as a percentage
int sys_get_mem_usage();

/// \brief Obtains the current temperature from the sensor specified
int sys_get_temp(const char* thermal_sensor);

/// \brief Find a temperature sensor for use with sys_get_temp
/// \param hwmon_names Array of names which could be used to identify the hwmon device in question
/// \param sensor_names Array of names which could be used to identify the sensor in question
char* sys_find_thermal_sensor(const std::vector<std::string> &hwmon_names, const std::vector<std::string> &sensor_names);

/// \brief Uses sys_find_thermal_sensor to automatically find the CPU package sensor
char* sys_find_cpu_sensor();

#endif //PICOLED_DRIVER_LINUX_SYSTEM_INFO_HXX