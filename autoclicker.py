import pyautogui
import time

print("���������� �������. ������� Ctrl+C ��� ���������.")

try:
    while True:
        pyautogui.click()  # ��������� ����
        time.sleep(1)  # �������� � 1 ������� ����� �������
except KeyboardInterrupt:
    print("���������� ����������.")
