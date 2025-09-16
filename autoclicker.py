import pyautogui
import time

print("Автокликер запущен. Нажмите Ctrl+C для остановки.")

try:
    while True:
        pyautogui.click()  
        time.sleep(1)  
except KeyboardInterrupt:
    print("Автокликер остановлен.")
