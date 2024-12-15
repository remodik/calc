#include <iostream>
#include <cstring>
#include <cctype>
#include <cmath>
#include <fstream>
using namespace std;

// Функция для проверки выражения
bool validExpr(const char* expr) {
    for (int i = 0; expr[i] != '\0'; ++i) {
        // Проверяем символы: цифры, операторы, скобки, пробелы и "log"
        if (!(isdigit(expr[i]) || strchr("+-*/^()!., ", expr[i]) ||
              (strncmp(&expr[i], "log", 3) == 0) || isalpha(expr[i]))) {
            cout << "Ошибка: Неизвестный символ: " << expr[i] << " на позиции " << i << endl;
            return false;
        }
    }
    return true;
}

// Факториал
double fact(int n) {
    if (n < 0) return 0;
    if (n == 0) return 1;
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

// Выполнение операций
double performOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b != 0) return a / b;
            cout << "Ошибка: Деление на ноль!\n";
            return 0;
        case '^': return power(a, b);
        default: return 0;
    }
}

// Проверка приоритета операторов
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

// Вычисление выражения
double eval(const char* expr) {
    double values[100];
    char operators[100];
    int valTop = -1, opTop = -1;
    int i = 0;

    while (expr[i] != '\0') {
        if (isdigit(expr[i]) || expr[i] == '.') {
            double num = 0, frac = 0;
            int fracDiv = 1;
            bool isFraction = false;

            // Считываем число с возможной дробной частью
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
            values[++valTop] = num;
        } else if (expr[i] == '(') {
            operators[++opTop] = expr[i];
            i++;
        } else if (expr[i] == ')') {
            while (opTop >= 0 && operators[opTop] != '(') {
                double b = values[valTop--];
                double a = values[valTop--];
                char op = operators[opTop--];
                values[++valTop] = performOp(a, b, op);
            }
            opTop--;  // Убираем '('
            i++;
        } else if (strncmp(&expr[i], "log", 3) == 0) {
            i += 3;
            if (expr[i] == '(') {
                i++;
                double base = 0, value = 0;
                sscanf(&expr[i], "%lf,%lf", &base, &value);

                // Пропустить параметры до ')'
                while (expr[i] != ')' && expr[i] != '\0') i++;

                if (expr[i] == ')') {
                    i++; // Пропускаем ')'
                    values[++valTop] = logFunc(base, value);
                } else {
                    cout << "Ошибка: Некорректный формат функции log. Ожидается log(base,value).\n";
                    return 0;
                }
            } else {
                cout << "Ошибка: Некорректный формат функции log. Ожидается log(base,value).\n";
                return 0;
            }
        } else if (expr[i] == '!') {
            double a = values[valTop--];
            values[++valTop] = fact((int)a);
            i++;
        } else if (strchr("+-*/^", expr[i])) {
            // Обрабатываем операторы с учётом приоритета
            while (opTop >= 0 && precedence(operators[opTop]) >= precedence(expr[i])) {
                double b = values[valTop--];
                double a = values[valTop--];
                char op = operators[opTop--];
                values[++valTop] = performOp(a, b, op);
            }
            operators[++opTop] = expr[i];
            i++;
        } else {
            i++;
        }
    }

    // Выполнение оставшихся операций
    while (opTop >= 0) {
        double b = values[valTop--];
        double a = values[valTop--];
        char op = operators[opTop--];
        values[++valTop] = performOp(a, b, op);
    }

    return values[valTop];
}

// Сохранение истории
void saveHistory(const char history[][100], int historyIndex) {
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

// Меню
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

            if (!validExpr(expression)) {
                continue;
            }

            result = eval(expression);
            cout << "Результат: " << result << endl;

            if (historyIndex < 100) {
                snprintf(history[historyIndex], 100, "Выражение: %s, Результат: %.2f", expression, result);
                ++historyIndex;
            } else {
                cout << "История заполнена.\n";
            }
        } else if (choice == 2) {
            if (historyIndex == 0) {
                cout << "История пуста.\n";
            } else {
                for (int i = 0; i < historyIndex; i++) {
                    cout << history[i] << endl;
                }
            }
        } else if (choice == 3) {
            historyIndex = 0;
            cout << "История очищена.\n";
        } else if (choice == 4) {
            saveHistory(history, historyIndex);
            cout << "История сохранена в файл history.txt\n";
        } else if (choice == 5) {
            cout << "Выход...\n";
            break;
        } else {
            cout << "Некорректный выбор!\n";
        }
    }
}
