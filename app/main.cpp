#include <iostream>  // Для работы с вводом-выводом (std::cerr)
#include "logger.h"  // Подключаем нашу библиотеку логирования

int main() {
    // Пытаемся инициализировать логгер
    // Указываем:
    // - имя файла для логов: "app.log"
    // - уровень логирования по умолчанию: INFO
    if (!Logger::initialize("app.log", LogLevel::INFO)) {
        // Если инициализация не удалась, выводим ошибку и завершаем программу
        std::cerr << "Failed to initialize logger" << std::endl;
        return 1;  // Возвращаем код ошибки
    }
    
    // Пробуем записать разные сообщения:
    
    // 1. Это сообщение уровня DEBUG - НЕ будет записано,
    //    так как текущий уровень INFO (DEBUG < INFO)
    Logger::log("This is a debug message", LogLevel::DEBUG);
    
    // 2. Это сообщение уровня INFO (по умолчанию) - БУДЕТ записано
    Logger::log("This is an info message");
    
    // 3. Это сообщение уровня ERROR - БУДЕТ записано
    //    (ERROR > INFO)
    Logger::log("This is an error message", LogLevel::ERROR);

    // Изменяем уровень логирования на DEBUG
    // Теперь будут записываться и DEBUG сообщения
    Logger::setLogLevel(LogLevel::DEBUG);
    
    // 4. Это DEBUG сообщение теперь БУДЕТ записано
    Logger::log("Now debug messages are visible", LogLevel::DEBUG);
    
    return 0;  // Успешное завершение программы
}










