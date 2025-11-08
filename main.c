#include "log_sys.h"
#include <stdlib.h> 


/* 模拟传感器读取函数 */
static int read_temperature_sensor(void)
{
    return 25 + (rand() % 10); // 模拟25-35°C的温度
}

/* 模拟网络操作 */
static int connect_to_server(const char *server)
{
    LOG_DEBUG(MODULE_NETWORK, "Attempting to connect to server: %s", server);
    
    if (rand() % 10 > 2) { 
        LOG_INFO(MODULE_NETWORK, "Successfully connected to %s", server);
        return 0;
    } else {
        LOG_ERROR(MODULE_NETWORK, "Failed to connect to %s", server);
        return -1;
    }
}

/* 数据处理的示例函数 */
static void process_sensor_data(void)
{
    LOG_TRACE(MODULE_SENSOR, "Starting sensor data processing");
    
    int temp = read_temperature_sensor();
    LOG_DEBUG(MODULE_SENSOR, "Raw temperature reading: %d°C", temp);
    
    if (temp > 30) {
        LOG_WARN(MODULE_SENSOR, "High temperature detected: %d°C", temp);
    }
    
    LOG_INFO(MODULE_APPLICATION, "Sensor data processed successfully");
    LOG_TRACE(MODULE_SENSOR, "Finished sensor data processing");
}

int main(void)
{
    log_config_t config = {
        .global_level = LOG_LEVEL_DEBUG,
        .enable_timestamp = 1,
        .enable_async = 0
    };
    config.module_levels[MODULE_SENSOR] = LOG_LEVEL_TRACE;  
    config.module_levels[MODULE_NETWORK] = LOG_LEVEL_INFO;  
    
    log_init(&config);
   
    LOG_INFO(MODULE_SYSTEM, "Application started");
    LOG_INFO(MODULE_SYSTEM, "Firmware version: 1.0.0");
    LOG_DEBUG(MODULE_SYSTEM, "Initializing hardware components");
    
    process_sensor_data();
    
    if (connect_to_server("mqtt.broker.com") == 0) {
        LOG_INFO(MODULE_NETWORK, "Data transmission completed");
    } else {
        LOG_ERROR(MODULE_NETWORK, "Data transmission failed, will retry in 5s");
    }
    
    LOG_INFO(MODULE_SYSTEM, "Application shutdown");
    
    return 0;
}

