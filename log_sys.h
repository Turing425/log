#ifndef __LOG_SYS_H
#define __LOG_SYS_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

/* 日志级别定义 */
typedef enum {
    LOG_LEVEL_TRACE = 0,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_NONE
} log_level_t;

/* 日志模块定义 */
typedef enum {
    MODULE_SYSTEM = 0,
    MODULE_NETWORK,
    MODULE_SENSOR,
    MODULE_STORAGE,
    MODULE_APPLICATION,
    MODULE_MAX
} log_module_t;

/* 日志系统配置 */
typedef struct {
    log_level_t global_level;      // 全局日志级别
    log_level_t module_levels[MODULE_MAX]; // 各模块日志级别
    uint8_t enable_timestamp : 1;  // 启用时间戳
    uint8_t enable_async : 1;      // 启用异步模式
} log_config_t;

void log_init(log_config_t *config);
void log_set_level(log_level_t level);
void log_set_module_level(log_module_t module, log_level_t level);

#define LOG_COMMON(level, module, fmt, ...) \
    do { \
        if (level >= get_global_log_level() && level >= get_module_log_level(module)) { \
            log_output(level, module, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
        } \
    } while(0)

#define LOG_TRACE(module, fmt, ...)   LOG_COMMON(LOG_LEVEL_TRACE, module, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(module, fmt, ...)   LOG_COMMON(LOG_LEVEL_DEBUG, module, fmt, ##__VA_ARGS__)
#define LOG_INFO(module, fmt, ...)    LOG_COMMON(LOG_LEVEL_INFO, module, fmt, ##__VA_ARGS__)
#define LOG_WARN(module, fmt, ...)    LOG_COMMON(LOG_LEVEL_WARN, module, fmt, ##__VA_ARGS__)
#define LOG_ERROR(module, fmt, ...)   LOG_COMMON(LOG_LEVEL_ERROR, module, fmt, ##__VA_ARGS__)

log_level_t get_global_log_level(void);
log_level_t get_module_log_level(log_module_t module);
void log_output(log_level_t level, log_module_t module, const char *file, 
                int line, const char *fmt, ...);

const char* get_module_name(log_module_t module);
const char* get_level_name(log_level_t level);

#endif 

