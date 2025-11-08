#include "log_sys.h"
#include <stdarg.h>

/*日志配置*/
static log_config_t g_log_config = {
    .global_level = LOG_LEVEL_INFO,
    .module_levels = {0},
    .enable_timestamp = 1,
    .enable_async = 0
};

/*模块名称映射*/
static const char* MODULE_NAMES[] = {
    "SYSTEM",
    "NETWORK", 
    "SENSOR",
    "STORAGE",
    "APPLICATION"
};

/*级别名称映射*/
static const char* LEVEL_NAMES[] = {
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR"
};


/*初始化日志系统*/
void log_init(log_config_t *config)
{
    if (config) {
        memcpy(&g_log_config, config, sizeof(log_config_t));
    }
    
    for (int i = 0; i < MODULE_MAX; i++) {
        if (g_log_config.module_levels[i] == 0) {
            g_log_config.module_levels[i] = g_log_config.global_level;
        }
    }
    
    LOG_INFO(MODULE_SYSTEM, "Logger initialized, global level: %s", 
             get_level_name(g_log_config.global_level));
}

/*设置全局日志级别*/
void log_set_level(log_level_t level)
{
    g_log_config.global_level = level;
}

void log_set_module_level(log_module_t module, log_level_t level)
{
    if (module < MODULE_MAX) {
        g_log_config.module_levels[module] = level;
    }
}

/*获取全局日志级别*/
log_level_t get_global_log_level(void)
{
    return g_log_config.global_level;
}

/*获取模块日志级别*/
log_level_t get_module_log_level(log_module_t module)
{
    if (module < MODULE_MAX) {
        return g_log_config.module_levels[module];
    }
    return LOG_LEVEL_NONE;
}

/*获取模块名称*/
const char* get_module_name(log_module_t module)
{
    if (module < MODULE_MAX) {
        return MODULE_NAMES[module];
    }
    return "UNKNOWN";
}

/*获取级别名称*/
const char* get_level_name(log_level_t level)
{
    if (level < LOG_LEVEL_NONE) {
        return LEVEL_NAMES[level];
    }
    return "NONE";
}

/*获取当前时间戳字符串*/
static void get_timestamp_string(char *buffer, int buffer_size)
{
    time_t raw_time;
    struct tm *time_info;
    
    time(&raw_time);
    time_info = localtime(&raw_time);
    
    strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", time_info);
}

/*日志输出核心函数*/
void log_output(log_level_t level, log_module_t module, const char *file, 
                int line, const char *fmt, ...)
{
    char timestamp[32] = {0};
    char log_buffer[512] = {0};
    char message_buffer[256] = {0};
    va_list args;
    
    if (g_log_config.enable_timestamp) {
        get_timestamp_string(timestamp, sizeof(timestamp));
    }
    
    const char *filename = strrchr(file, '/');
    if (filename == NULL) {
        filename = strrchr(file, '\\');
    }
    filename = (filename != NULL) ? filename + 1 : file;
    
    va_start(args, fmt);
    vsnprintf(message_buffer, sizeof(message_buffer), fmt, args);
    va_end(args);

    snprintf(log_buffer, sizeof(log_buffer), 
             "[%s][%s][%s][%s:%d] %s",
             timestamp,
             get_level_name(level),
             get_module_name(module),
             filename, line,
             message_buffer);

    printf("%s\n", log_buffer);

	return;
}

