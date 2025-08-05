#!/bin/bash

# Переходим в папку tests
cd "$(dirname "$0")" || exit 1

# Очищаем предыдущий лог
rm -f test.log

# Создаем временный файл с тестовыми командами
cat > test_input.txt <<EOF
INFO:Тест 1
ERROR:Тест 2
exit
EOF

# Запускаем приложение с входными данными из файла
echo "Запуск теста..."
../build/app/threaded_enter_app test.log INFO < test_input.txt

# Проверяем результаты
echo -e "\n=== Содержимое лога ==="
cat test.log

if grep -q "Тест 1" test.log && grep -q "Тест 2" test.log; then
    echo -e "\nУра.Оба теста записаны успешно!"
    rm -f test_input.txt
    exit 0
else
    echo -e "\nОшибка: не все тесты записаны"
    rm -f test_input.txt
    exit 1
fi