#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cctype>
using namespace std;

// Факториал
double fact(int n) {
    if (n < 0) return 0;
    double result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// Логарифм
double logFunc(double base, double value) {
    if (base <= 0 || base == 1 || value <= 0) {
        cout << "Ошибка: Некорректные параметры для логарифма.\n";
        return 0;
    }
    return log(value) / log(base); // Формула для логарифма по произвольному основанию
}

// Функция для выполнения арифметических операций
double performOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b != 0) return a / b;
            else {
                cout << "Ошибка: Деление на ноль!\n";
                return 0;
            }
        default:
            cout << "Ошибка: Неизвестная операция " << op << endl;
            return 0;
    }
}

// Проверка приоритета операторов
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '!') return 3; // Высокий приоритет для факториала
    return 0;
}

// Основная функция для вычисления выражений
double eval(const char* expr) {
    double values[100]; // Стек чисел
    char operators[100]; // Стек операторов
    int valuesTop = -1, operatorsTop = -1;

    for (int i = 0; expr[i] != '\0'; ++i) {
        if (isdigit(expr[i]) || expr[i] == '.') {
            // Чтение числа
            double num = 0;
            double frac = 0;
            int fracDiv = 1;
            bool isFraction = false;

            while (isdigit(expr[i]) || expr[i] == '.') {
                if (expr[i] == '.') {
                    isFraction = true;
                } else if (!isFraction) {
                    num = num * 10 + (expr[i] - '0');
                } else {
                    frac = frac * 10 + (expr[i] - '0');
                    fracDiv *= 10;
                }
                i++;
            }
            num += frac / fracDiv;
            values[++valuesTop] = num;
            i--; // Возвращаемся на шаг назад
        } else if (expr[i] == '(') {
            operators[++operatorsTop] = expr[i];
        } else if (expr[i] == ')') {
            while (operatorsTop >= 0 && operators[operatorsTop] != '(') {
                if (operators[operatorsTop] == '!') {
                    // Обработка факториала
                    double a = values[valuesTop--];
                    values[++valuesTop] = fact(int(a)); // Преобразуем в int
                    operatorsTop--;
                } else {
                    double b = values[valuesTop--];
                    double a = values[valuesTop--];
                    char op = operators[operatorsTop--];
                    values[++valuesTop] = performOp(a, b, op);
                }
            }
            operatorsTop--; // Убираем '('
        } else if (expr[i] == '!' || expr[i] == '+' || expr[i] == '-' ||
                   expr[i] == '*' || expr[i] == '/') {
            while (operatorsTop >= 0 && precedence(operators[operatorsTop]) >= precedence(expr[i])) {
                if (operators[operatorsTop] == '!') {
                    // Обработка факториала
                    double a = values[valuesTop--];
                    values[++valuesTop] = fact(int(a)); // Преобразуем в int
                    operatorsTop--;
                } else {
                    double b = values[valuesTop--];
                    double a = values[valuesTop--];
                    char op = operators[operatorsTop--];
                    values[++valuesTop] = performOp(a, b, op);
                }
            }
            operators[++operatorsTop] = expr[i];
        } else if (strncmp(&expr[i], "log", 3) == 0) {
            // Обработка логарифма
            i += 3; // Пропускаем "log"
            if (expr[i] == '(') {
                i++; // Пропускаем '('
                double base = 0, value = 0;
                bool readingBase = true;
                while (expr[i] != ',') {
                    base = base * 10 + (expr[i] - '0');
                    i++;
                }
                i++; // Пропускаем запятую
                while (expr[i] != ')') {
                    value = value * 10 + (expr[i] - '0');
                    i++;
                }
                values[++valuesTop] = logFunc(base, value);
            }
        }
    }

    while (operatorsTop >= 0) {
        if (operators[operatorsTop] == '!') {
            double a = values[valuesTop--];
            values[++valuesTop] = fact(int(a));
            operatorsTop--;
        } else {
            double b = values[valuesTop--];
            double a = values[valuesTop--];
            char op = operators[operatorsTop--];
            values[++valuesTop] = performOp(a, b, op);
        }
    }
    return values[valuesTop];
}

void showMenu() {
    cout << "--- Калькулятор ---\n";
    cout << "1. Ввести выражение\n";
    cout << "2. Показать историю вычислений\n";
    cout << "3. Очистить историю\n";
    cout << "4. Выход\n";
}

int main() {
    double result;
    char expression[100];
    char history[100][100];
    int historyIndex = 0;
    int choice;

    while (true) {
        showMenu();
        cout << "Выберите действие: ";
        cin >> choice;
        cin.ignore();  // Для игнорирования символа новой строки после выбора действия

        if (choice == 1) {
            cout << "Введите выражение: ";
            cin.getline(expression, 100);
            result = eval(expression);

            // Сохранение результата в историю
            sprintf(history[historyIndex], "Выражение: %s, Результат: %.2f", expression, result);
            historyIndex++;

            cout << "Результат: " << result << endl;
        } else if (choice == 2) {
            if (historyIndex == 0) {
                cout << "История пуста.\n";
            } else {
                cout << "--- История вычислений ---\n";
                for (int i = 0; i < historyIndex; i++) {
                    cout << history[i] << endl;
                }
            }
        } else if (choice == 3) {
            historyIndex = 0;  // Очистка истории
            cout << "История очищена.\n";
        } else if (choice == 4) {
            cout << "Выход...\n";
            break;
        } else {
            cout << "Некорректный выбор! Пожалуйста, выберите снова.\n";
        }
    }
    return 0;
}
