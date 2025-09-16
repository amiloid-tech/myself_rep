#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread> 

using namespace std;


void printArrayWithDelay(const vector<int>& arr, int delayMs) {
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
    this_thread::sleep_for(chrono::milliseconds(delayMs)); 
}


vector<int> generateRandomArray(int size) {
    vector<int> arr(size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 100);
    
    for (int i = 0; i < size; ++i) {
        arr[i] = dist(gen);
    }
    
    return arr;
}


void bubbleSort(vector<int> arr, int delayMs) {
    int n = arr.size();
    bool swapped;
    for (int i = 0; i < n-1; ++i) {
        swapped = false;
        for (int j = 0; j < n-i-1; ++j) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
                printArrayWithDelay(arr, delayMs); 
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}


void selectionSort(vector<int> arr, int delayMs) {
    int n = arr.size();
    for (int i = 0; i < n-1; ++i) {
        int min_idx = i;
        for (int j = i+1; j < n; ++j) {
            if (arr[j] < arr[min_idx]) min_idx = j;
        }
        if (min_idx != i) {
            swap(arr[i], arr[min_idx]);
            printArrayWithDelay(arr, delayMs); 
        }
    }
}


void insertionSort(vector<int> arr, int delayMs) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            printArrayWithDelay(arr, delayMs); 
            j--;
        }
        arr[j+1] = key;
        printArrayWithDelay(arr, delayMs);
    }
}

int main() {
    int size;
    cout << "Введите размер массива: ";
    cin >> size;
    
    vector<int> originalArray = generateRandomArray(size);
    
    cout << "\nИсходный массив:" << endl;
    printArrayWithDelay(originalArray, 0); 
    
    cout << "\nВыберите метод сортировки:\n"
         << "1. Пузырьковая сортировка\n"
         << "2. Сортировка выбором\n"
         << "3. Сортировка вставками\n"
         << "Ваш выбор: ";
    int choice;
    cin >> choice;
    
    int delayMs;
    cout << "Введите задержку (в миллисекундах): ";
    cin >> delayMs;
    
    cout << endl;
    switch(choice) {
        case 1:
            cout << "Процесс пузырьковой сортировки:" << endl;
            bubbleSort(originalArray, delayMs);
            break;
        case 2:
            cout << "Процесс сортировки выбором:" << endl;
            selectionSort(originalArray, delayMs);
            break;
        case 3:
            cout << "Процесс сортировки вставками:" << endl;
            insertionSort(originalArray, delayMs);
            break;
        default:
            cout << "Неверный выбор!" << endl;
            return 1;
    }
    
    cout << "\nСортировка завершена!" << endl;
    return 0;
}