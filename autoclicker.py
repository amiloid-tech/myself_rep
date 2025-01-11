import pyautogui
import time

print("Автокликер запущен. Нажмите Ctrl+C для остановки.")

try:
    while True:
        pyautogui.click()  # выполняем клик
        time.sleep(1)  # задержка в 1 секунду между кликами
except KeyboardInterrupt:
    print("Автокликер остановлен.")
