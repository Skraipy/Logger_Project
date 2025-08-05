//By Skraipy//
#include "logger.h"
#include <sstream>
//--------------------------------------------------------------------------------//
// Инициализация статической переменной-указателя на единственный экземпляр логгера
std::unique_ptr<Logger> Logger::instance = nullptr;

/**
 * @brief Инициализирует систему логирования
 * @param filename Имя файла для записи логов
 * @param defaultLevel Уровень логирования по умолчанию
 * @return true если инициализация прошла успешно, false в случае ошибки
 * 
 * @note Если логгер уже был инициализирован, повторная инициализация невозможна
 */
bool Logger::initialize(const std::string& filename, LogLevel defaultLevel) {
    // Проверка, что логгер еще не был инициализирован
    if (instance) {
        return false;
    }
    
    // Создаем единственный экземпляр логгера
    instance = std::unique_ptr<Logger>(new Logger());
    
    // Устанавливаем уровень логирования
    instance->currentLevel = defaultLevel;
    
    // Открываем файл для записи логов (в режиме дополнения)
    instance->LogFile.open(filename, std::ios::app);
    
    // Проверяем успешность открытия файла
    if (!instance->LogFile.is_open()) {
        // Если файл не открылся, сбрасываем указатель
        instance.reset();
        return false;
    }
    
    return true;
}
//--------------------------------------------------------------------------------//
/**
 * @brief Устанавливает новый уровень логирования
 * @param level Новый уровень логирования (DEBUG, INFO, ERROR)
 */
void Logger::setLogLevel(LogLevel level) {
    if (instance) {
        instance->currentLevel = level;
    } 
}
//--------------------------------------------------------------------------------//
/**
 * @brief Записывает сообщение в лог
 * @param message Текст сообщения
 * @param level Уровень важности сообщения (по умолчанию INFO)
 * 
 * @note Сообщения с уровнем ниже текущего уровня логирования игнорируются
 */
void Logger::log(const std::string& message, LogLevel level) {
    // Проверяем, что логгер инициализирован и уровень сообщения достаточен
    if (!instance || level < instance->currentLevel) {
        return;
    }
    
    // Передаем сообщение на запись
    instance->writeLog(message, level);
}
//--------------------------------------------------------------------------------//
/**
 * @brief Внутренний метод для записи сообщения в файл
 * @param message Текст сообщения
 * @param level Уровень важности сообщения
 */
void Logger::writeLog(const std::string& message, LogLevel level) {
    // Формируем строку для записи
    std::stringstream ss;
    ss << getCurrentTime() << " [" << levelString(level) << "] " << message << "\n";

    // Если файл открыт, записываем сообщение
    if (LogFile.is_open()) {
        LogFile << ss.str();
        LogFile.flush(); // Сбрасываем буфер для немедленной записи
        
    }
}
//--------------------------------------------------------------------------------//
/**
 * @brief Возвращает текущее время в формате строки
 * @return Строка с текущей датой и временем в формате ГГГГ-ММ-ДД ЧЧ:ММ:СС
 */
std::string Logger::getCurrentTime() {
    // Получаем текущее время
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    // Форматируем время в строку
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
//--------------------------------------------------------------------------------//
/**
 * @brief Преобразует enum LogLevel в строковое представление
 * @param level Уровень логирования
 * @return Строковое представление уровня
 */
std::string Logger::levelString(LogLevel level) {
    switch (level) {
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::ERROR:
        return "ERROR";
    default:
        return "UNKNOWN"; 
    }
}
//--------------------------------------------------------------------------------//