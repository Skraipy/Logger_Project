//By Skraipy//
#pragma once  // Защита от множественного включения заголовочного файла 
// Подключаем необходимые стандартные библиотеки
#include <string>     // для работы со строками
#include <fstream>    // для работы с файлами
#include <chrono>     // для работы со временем
#include <iomanip>    // для форматирования вывода
#include <memory>     // для умных указателей
//--------------------------------------------------------------------------------//
// Перечисление уровней логирования
enum class LogLevel {
    DEBUG,   // Отладочные сообщения
    INFO,    // Информационные сообщения
    ERROR    // Сообщения об ошибках
};
//--------------------------------------------------------------------------------//
/**
 * @class Logger
 * @brief Класс для логирования сообщений в файл
 * 
 * Реализован как Singleton - может существовать только один экземпляр
 */
class Logger {
public:
    /**
     * @brief Инициализирует логгер
     * @param filename Имя файла для записи логов
     * @param defaultLevel Уровень логирования по умолчанию
     * @return true если инициализация прошла успешно, false в случае ошибки
     */
    static bool initialize(const std::string& filename, LogLevel defaultLevel);
    
    /**
     * @brief Устанавливает уровень логирования
     * @param level Новый уровень логирования
     */
    static void setLogLevel(LogLevel level);
    
    /**
     * @brief Записывает сообщение в лог
     * @param message Текст сообщения
     * @param level Уровень важности сообщения (по умолчанию INFO)
     */
    static void log(const std::string& message, LogLevel level = LogLevel::INFO);
        
private:
    static std::unique_ptr<Logger> instance;  // Указатель на единственный экземпляр

    std::ofstream LogFile;    // Файловый поток для записи логов
    LogLevel currentLevel;    // Текущий уровень логирования

    Logger() = default;  // Приватный конструктор
    Logger(const Logger&) = delete;  // Запрет копирования
    Logger& operator=(const Logger&) = delete;  // Запрет присваивания

    /**
     * @brief Внутренний метод для записи сообщения в файл
     * @param message Текст сообщения
     * @param level Уровень важности сообщения
     */
    void writeLog(const std::string& message, LogLevel level);
    
    /**
     * @brief Возвращает текущее время в виде строки
     * @return Строка с текущим временем в формате ГГГГ-ММ-ДД ЧЧ:ММ:СС
     */
    static std::string getCurrentTime();
    
    /**
     * @brief Преобразует уровень логирования в строку
     * @param level Уровень логирования
     * @return Строковое представление уровня
     */
    static std::string levelString(LogLevel level);
};
//--------------------------------------------------------------------------------//