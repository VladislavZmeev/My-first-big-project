#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "types.h"

// Текущий пороговый уровень логирования (по умолчанию INFO)
static int current_log_level = INFO;

// Массив для преобразования уровня в строку
static const char* level_strings[] = {
    "ERROR",
    "INFO",
    "TRACE",
    "DEBUG"
};

// Функция установки уровня логирования
int set_log_level(int level) 
{
    int old_level = current_log_level;
    if (level >= ERROR && level <= DEBUG) {
        current_log_level = level;
    }
    return old_level;
}

// Функция логирования
void trace(int level, const char* format, ...) 
{
    // Проверяем, нужно ли выводить сообщение
    if (level > current_log_level) {
        return;
    }
    
    // Выводим уровень логирования
    if (level >= ERROR && level <= DEBUG) {
        printf("[%s] ", level_strings[level]);
    }
    
    // Выводим основное сообщение
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    printf("\n");
}