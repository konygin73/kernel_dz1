#!/bin/bash

MODULE_NAME="my_module"
SYSFS_PATH="/sys/module/${MODULE_NAME}/parameters"

if [ ! -d "${SYSFS_PATH}" ]; then
  echo "Ошибка: Модуль ${MODULE_NAME} не загружен или пути в sysfs не существуют."
  exit 1
fi

echo "[Шаг 1] Запись строки в 'message'..."
echo "Test message!" > "${SYSFS_PATH}/message"
if [ $? -eq 0 ]; then
  echo "-> Успешно записано: \"Test message!\""
else
  echo "-> Ошибка записи в message"
fi
echo "----------------------------------------"

echo "[Шаг 2] Чтение текущего значения 'message'..."
CURRENT_MSG=$(cat "${SYSFS_PATH}/message")
echo "-> Вывод из sysfs: \"${CURRENT_MSG}\""
echo "----------------------------------------"

echo "[Шаг 3] Установка 'index' = 5..."
echo "5" > "${SYSFS_PATH}/index"
echo "-> Индекс установлен."
echo "----------------------------------------"

echo "[Шаг 4] Запись символа 'M' по текущему индексу..."
echo "M" > "${SYSFS_PATH}/ch"
echo "-> Символ 'M' отправлен."
echo "----------------------------------------"

echo "[Шаг 5] Установка 'index' = 6..."
echo "6" > "${SYSFS_PATH}/index"
echo "-> Индекс установлен."
echo "----------------------------------------"

echo "[Шаг 6] Запись символа 'E' по текущему индексу..."
echo "E" > "${SYSFS_PATH}/ch"
echo "-> Символ 'E' отправлен."
echo "----------------------------------------"

# Финальная проверка результата
echo "[ФИНАЛ] Чтение измененной строки 'message'..."
FINAL_MSG=$(cat "${SYSFS_PATH}/message")
echo "-> Итоговый результат: \"${FINAL_MSG}\""
echo ""
echo "=== ТЕСТ ЗАВЕРШЕН ==="

# Вывод логов ядра для подтверждения, что callback-функции сработали
echo ""
echo "Последние сообщения из dmesg:"
dmesg | tail -n 7
