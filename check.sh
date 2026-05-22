#!/bin/bash

MODULE_NAME="my_module"
SYSFS_PATH="/sys/module/${MODULE_NAME}/parameters"
TEST_STRING="Hello, World!"

if [ ! -d "${SYSFS_PATH}" ]; then
  echo "Ошибка: Модуль ${MODULE_NAME} не загружен или пути в sysfs не существуют."
  exit 1
fi

len=${#TEST_STRING}

for (( idx=0; idx<len; idx++ )); do
    char="${TEST_STRING:$idx:1}"
    echo "$idx" > "${SYSFS_PATH}/idx"
    echo -n "$char" > "${SYSFS_PATH}/ch_val"
done
# Финальная проверка результата
echo "[ФИНАЛ] Чтение измененной строки 'message'..."
FINAL_MSG=$(cat "${SYSFS_PATH}/my_str")
echo "-> Итоговый результат: \"${FINAL_MSG}\""
echo ""
echo "=== ТЕСТ ЗАВЕРШЕН ==="
echo ""
echo "Последние сообщения из dmesg:"
dmesg | tail -n 7
