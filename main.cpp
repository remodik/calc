#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <limits>
#include <sstream>
#define MAX_INPUT_LENGTH 256

using namespace std;

// Функция для вычисления факториала
double factorial(int n) {
    if (n < 0) return -1; // Ошибка для отрицательных чисел
    double result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// Функция для вычисления логарифма
double custom_log(double base, double value) {
    return log(value) / log(base);
}

// Функция для выполнения операции
double calculate(double num1, char op, double num2) {
    switch (op) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return num2 != 0 ? num1 / num2 : NAN; // Деление на 0
        case '^': return pow(num1, num2);
        default: return NAN; // Неизвестная операция
    }
}

// Разбор и выполнение сложных операций
double evaluateExpression(const char* input) {
    char temp[MAX_INPUT_LENGTH];
    strncpy(temp, input, MAX_INPUT_LENGTH);

    // Проверка на логарифм (log(base, value))
    if (strncmp(temp, "log(", 4) == 0) {
        char* params = temp + 4;
        char* comma = strchr(params, ',');
        char* end = strchr(params, ')');
        if (comma && end) {
            *comma = '\0';
            *end = '\0';
            double base = atof(params);
            double value = atof(comma + 1);
            return custom_log(base, value);
        }
    }

    // Обработка выражений с помощью stringstream
    std::stringstream ss(temp);
    double result = 0, num = 0;
    char op = '+';

    while (ss >> num) {
        // Проверка на факториал (!число)
        if (ss.peek() == '!') {
            ss.get(); // Убираем '!'
            num = factorial(static_cast<int>(num)); // Вычисляем факториал
        }
        // Выполнение операции
        switch (op) {
            case '+': result += num; break;
            case '-': result -= num; break;
            case '*': result *= num; break;
            case '/': result /= num; break;
            case '^': result = pow(result, num); break; // Степень
        }
        ss >> op; // Считываем следующий оператор
    }

    return result;
}

// Функция меню
void showMenu() {
    cout << "Калькулятор:\n";
    cout << "1. Ввод выражения\n";
    cout << "2. Показать историю\n";
    cout << "3. Очистить историю\n";
    cout << "4. Выход\n\n";
}

// Функция для показа истории операций
void showHistory() {
    FILE* file = fopen("history.txt", "r");
    if (!file) {
        cout << "История пуста.\n";
        return;
    }
    char line[MAX_INPUT_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        cout << line;
    }
    fclose(file);
}

// Функция для записи результата в историю
void saveHistory(const char* input, double result) {
    FILE* file = fopen("history.txt", "a");
    if (!file) return;
    fprintf(file, "%s = %.6f\n", input, result);
    fclose(file);
}

// Функция для очистки истории
void clearHistory() {
    cout << "Сколько операций очистить (* для очистки всего): ";
    char input[MAX_INPUT_LENGTH];
    cin >> input;

    if (strcmp(input, "*") == 0) {
        // Полная очистка истории
        FILE* file = fopen("history.txt", "w");
        if (file) fclose(file);
        cout << "История полностью очищена.\n";
    } else {
        // Очистка указанного количества строк
        int numToRemove = atoi(input);
        if (numToRemove <= 0) {
            cout << "Некорректный ввод.\n";
            return;
        }

        FILE* file = fopen("history.txt", "r");
        if (!file) {
            cout << "История пуста.\n";
            return;
        }

        // Чтение оставшихся строк в память
        char lines[MAX_INPUT_LENGTH][MAX_INPUT_LENGTH];
        int count = 0;
        while (fgets(lines[count], sizeof(lines[count]), file)) {
            ++count;
        }
        fclose(file);

        // Перезапись файла без удаленных строк
        file = fopen("history.txt", "w");
        if (!file) return;
        for (int i = numToRemove; i < count; ++i) {
            fputs(lines[i], file);
        }
        fclose(file);
        cout << "Удалено " << min(numToRemove, count) << " операций из истории.\n";
    }
}

int main() {
    char input[MAX_INPUT_LENGTH];
    int choice;

    while (true) {
        showMenu();
        cout << "Выберите опцию: ";
        cin >> choice;

        // Проверка на ошибку ввода
        if (cin.fail()) {
            cin.clear(); // Сброс флага ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка ввода
            cout << "Ошибка: введите корректное число.\n";
            continue; // Возвращаемся в начало цикла
        }

        switch (choice) {
            case 1: {
                cout << "Введите выражение: ";
                cin.ignore(); // Очистка остатков ввода
                cin.getline(input, MAX_INPUT_LENGTH);

                double result = evaluateExpression(input);
                if (!isnan(result)) {
                    cout << "Результат: " << result << endl;
                    saveHistory(input, result);
                } else {
                    cout << "Ошибка: неверное выражение.\n";
                }
                break;
            }
            case 2: {
                showHistory();
                break;
            }
            case 3: {
                clearHistory();
                break;
            }
            case 4: {
                cout << "Выход из программы.\n";
                return 0; // Завершение программы
            }
            default: {
                cout << "Ошибка: неверный выбор. Введите число от 1 до 4.\n";
                break;
            }
        }
    }
}