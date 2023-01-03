#pragma once

#ifndef CSAPP_LOG_H
#define CSAPP_LOG_H

#include <csapp/s_config.h>

typedef enum
{
    S_LOG_LEVEL_ERROR,
    S_LOG_LEVEL_WARNING,
    S_LOG_LEVEL_INFO,
    S_LOG_LEVEL_TRACE,
} s_log_level_e;

#define s_log_error(msg)
#define s_log_warn(msg)
#define s_log_info(msg)
#define s_log_trace(msg)

#endif // CSAPP_LOG_H
