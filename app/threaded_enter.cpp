#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include "logger.h"
#include <condition_variable>

// Структура для хранения сообщения и его уровня важности
struct LogMessage {
    std::string text;    // Текст сообщения
    LogLevel level;      // Уровень важности (DEBUG, INFO, ERROR)
};

// Потокобезопасная очередь сообщений
class ThreadSafeQueue {
public:
    // Добавление сообщения в очередь
    void push(const LogMessage& msg) {
        std::lock_guard<std::mutex> lock(mutex_);  // Блокируем мьютекс
        queue_.push(msg);                          // Добавляем сообщение
        cond_.notify_one();                        // Уведомляем один ожидающий поток
    }

    // Извлечение сообщения из очереди
    LogMessage pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (true) {
        if (stop_ && queue_.empty()) return {"", LogLevel::INFO};
        if (!queue_.empty()) break;
        cond_.wait(lock);
    }
    
    LogMessage msg = queue_.front();
    queue_.pop();
    return msg;
}

    // Остановка работы очереди
    void stop() {
        std::lock_guard<std::mutex> lock(mutex_);
        stop_ = true;                   // Устанавливаем флаг остановки
        cond_.notify_all();             // Уведомляем все ожидающие потоки
    }

    // Проверка флага остановки
    bool shouldStop() const {
        return stop_;
    }

private:
    std::queue<LogMessage> queue_;      // Очередь сообщений
    std::mutex mutex_;                  // Мьютекс для синхронизации
    std::condition_variable cond_;      // Условная переменная
    std::atomic<bool> stop_{false};     // Атомарный флаг остановки
};

// Глобальная очередь сообщений
ThreadSafeQueue logQueue;

// Функция потока-обработчика сообщений
void logWorker(ThreadSafeQueue& queue) {
    while (true) {
        LogMessage msg = queue.pop();  // Получаем сообщение из очереди
        
        // Проверяем условие выхода
        if (msg.text.empty() && queue.shouldStop()) break;
        
        // Записываем сообщение в лог
        Logger::log(msg.text, msg.level);
    }
}

// Преобразование строки в LogLevel
LogLevel parseLogLevel(const std::string& levelStr) {
    if (levelStr == "DEBUG") return LogLevel::DEBUG;
    if (levelStr == "INFO") return LogLevel::INFO;
    if (levelStr == "ERROR") return LogLevel::ERROR;
    return LogLevel::INFO;  // Значение по умолчанию
}

int main(int argc, char* argv[]) {
    // Проверка аргументов командной строки
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <logfile> <default_level> [DEBUG|INFO|ERROR]\n";
        return 1;
    }

    std::string logfile = argv[1];
    LogLevel defaultLevel = parseLogLevel(argv[2]);

    // Инициализация логгера
    if (!Logger::initialize(logfile, defaultLevel)) {
        std::cerr << "Failed to initialize logger\n";
        return 1;
    }

    // Запуск потока-обработчика
    std::thread worker(logWorker, std::ref(logQueue));

    std::cout << "Logger started. Enter messages (format: [LEVEL:]message). Type 'exit' to quit.\n";

    // Основной цикл ввода
    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "exit") {
            logQueue.stop();  // Останавливаем очередь
            break;
        }

        // Разбираем ввод пользователя
        size_t colonPos = input.find(':');
        if (colonPos != std::string::npos) {
            // Если есть указание уровня
            std::string levelStr = input.substr(0, colonPos);
            LogLevel level = parseLogLevel(levelStr);
            std::string message = input.substr(colonPos + 1);
            logQueue.push({message, level});
        } else {
            // Используем уровень по умолчанию
            logQueue.push({input, defaultLevel});
        }
    }

    worker.join();  // Ожидаем завершения потока
    return 0;
}