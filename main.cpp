#include <iostream>
#include <cmath>
#include <cstring>
#include <cctype>
#include <fstream>
using namespace std;

// Функция для проверки выражения на валидность
bool validateExpression(const char* expr, int& errorPos) {
    for (int i = 0; expr[i] != '\0'; ++i) {
        if (!(isdigit(expr[i]) || expr[i] == '.' || expr[i] == '+' || expr[i] == '-' ||
              expr[i] == '*' || expr[i] == '/' || expr[i] == '^' || expr[i] == '(' ||
              expr[i] == ')' || isspace(expr[i]))) {
            cout << "Ошибка: Неизвестный символ: " << expr[i] << "\nПозиция: " << i << endl;
            errorPos = i; // Сохраняем позицию некорректного символа
            return false;
        }
    }
    return true;
}

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
    return log(value) / log(base);
}

// Возведение в степень
double power(double base, double exp) {
    return pow(base, exp);
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
        case '^': return power(a, b);
        default:
            cout << "Ошибка: Неизвестная операция " << op << endl;
            return 0;
    }
}

// Проверка приоритета операторов
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3; // Высокий приоритет для возведения в степень
    if (op == '!') return 4; // Высший приоритет для факториала
    return 0;
}

// Основная функция для вычисления выражений
double eval(const char* expr) {
    double values[100];
    char operators[100];
    int valuesTop = -1, operatorsTop = -1;
    bool validExpression = false; // Флаг для проверки наличия корректных операций

    for (int i = 0; expr[i] != '\0'; ++i) {
        if (isdigit(expr[i]) || expr[i] == '.') {
            validExpression = true;
            double num = 0, frac = 0;
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
            i--;
        } else if (expr[i] == '(') {
            operators[++operatorsTop] = expr[i];
        } else if (expr[i] == ')') {
            while (operatorsTop >= 0 && operators[operatorsTop] != '(') {
                if (valuesTop < 1) { // Проверка на достаточное количество операндов
                    cout << "Ошибка: Некорректное выражение.\n";
                    return 0;
                }
                char op = operators[operatorsTop--];
                double b = values[valuesTop--];
                double a = values[valuesTop--];
                values[++valuesTop] = performOp(a, b, op);
            }
            if (operatorsTop < 0 || operators[operatorsTop] != '(') {
                cout << "Ошибка: Некорректные скобки.\n";
                return 0;
            }
            operatorsTop--;
        } else if (strchr("+-*/^!", expr[i])) {
            validExpression = true;
            while (operatorsTop >= 0 && precedence(operators[operatorsTop]) >= precedence(expr[i])) {
                if (valuesTop < 1) {
                    cout << "Ошибка: Некорректное выражение.\n";
                    return 0;
                }
                char op = operators[operatorsTop--];
                double b = values[valuesTop--];
                double a = values[valuesTop--];
                values[++valuesTop] = performOp(a, b, op);
            }
            operators[++operatorsTop] = expr[i];
        } else if (isalpha(expr[i])) {
            cout << "Ошибка: Некорректный символ в выражении: " << expr[i] << endl;
            return 0;
        }
    }

    while (operatorsTop >= 0) {
        if (valuesTop < 1) {
            cout << "Ошибка: Некорректное выражение.\n";
            return 0;
        }
        char op = operators[operatorsTop--];
        double b = values[valuesTop--];
        double a = values[valuesTop--];
        values[++valuesTop] = performOp(a, b, op);
    }

    if (!validExpression) {
        cout << "Ошибка: Пустое или некорректное выражение.\n";
        return 0;
    }

    return valuesTop == 0 ? values[0] : 0;
}

void saveHistoryToFile(const char history[][100], int historyIndex) {
    ofstream outFile("history.txt", ios::out);
    if (!outFile) {
        cout << "Ошибка: Не удалось открыть файл для записи.\n";
        return;
    }
    for (int i = 0; i < historyIndex; ++i) {
        outFile << history[i] << endl;
    }
    outFile.close();
}

void showMenu() {
    cout << "--- Калькулятор ---\n";
    cout << "1. Ввести выражение\n";
    cout << "2. Показать историю вычислений\n";
    cout << "3. Очистить историю\n";
    cout << "4. Сохранить историю в файл\n";
    cout << "5. Выход\n";
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
        cin.ignore();

        if (choice == 1) {
            cout << "Введите выражение: ";
            cin.getline(expression, 100);

            int errorPos;
            if (!validateExpression(expression, errorPos)) {
                cout << expression << endl;
                for (int i = 0; i < errorPos; ++i) cout << " ";
                cout << "^\nОшибка: Неизвестный символ\n";
                continue;
            }

            result = eval(expression);
            cout << "Результат: " << result << endl;

            // Сохранение в историю
            if (historyIndex < 100) { // Ограничение размера истории
                snprintf(history[historyIndex], 100, "Выражение: %s, Результат: %.2f", expression, result);
                ++historyIndex;
            } else {
                cout << "История заполнена, новая запись невозможна.\n";
            }
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
            historyIndex = 0;
            cout << "\b=====================\n";
            cout << "История очищена.\n";
            cout << "=====================\n";
        } else if (choice == 4) {
            saveHistoryToFile(history, historyIndex);
            cout << "История сохранена в файл history.txt\n";
        } else if (choice == 5) {
            cout << "Выход...\n";
            break;
        } else {
            cout << "Некорректный выбор! Пожалуйста, выберите снова.\n";
        }
    }
    return 0;
}