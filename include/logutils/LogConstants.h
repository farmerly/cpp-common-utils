#pragma once

namespace logutils {
static constexpr int LOG_LINE_SIZE = 1024 * 128; // 单行日志大小, 128kb
static constexpr int LOG_QUEUE_SIZE = 1024 * 1;  // Log 队列长度
} // namespace logutils