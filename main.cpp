#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <map>
#include <functional>
#include <stack>
#include <cctype>

using namespace std;

double factorial(double n) {
    if (n < 0) return NAN;
    if (n == 0 || n == 1) return 1;
    double result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}

double maximum(const vector<double>& values) {
    return *max_element(values.begin(), values.end());
}

double minimum(const vector<double>& values) {
    return *min_element(values.begin(), values.end());
}

map<string, function<double(vector<double>)>> FUNCTIONS = {
        {"abs", [](vector<double> args) { return fabs(args[0]); }},
        {"max", [](vector<double> args) { return maximum(args); }},
        {"min", [](vector<double> args) { return minimum(args); }},
        {"sqrt", [](vector<double> args) { return sqrt(args[0]); }},
        {"round", [](vector<double> args) { return round(args[0]); }},
        {"pow", [](vector<double> args) { return pow(args[0], args[1]); }},
        {"log", [](vector<double> args) { return log(args[0]); }},
        {"sin", [](vector<double> args) { return sin(args[0]); }},
        {"cos", [](vector<double> args) { return cos(args[0]); }},
        {"tan", [](vector<double> args) { return tan(args[0]); }},
        {"!", [](vector<double> args) { return factorial(args[0]); }},
};

vector<string> history;

string preprocess_expression(string expression) {
    size_t pos = expression.find("!");
    while (pos != string::npos) {
        if (pos > 0 && isdigit(expression[pos - 1])) {
            expression.replace(pos, 1, " !");
        } else {
            expression.erase(pos, 1);
        }
        pos = expression.find("!");
    }
    return expression;
}

double evaluateFunction(string func_name, const string& args_str) {
    if (FUNCTIONS.find(func_name) == FUNCTIONS.end()) {
        throw runtime_error("Неподдерживаемая функция: " + func_name);
    }

    vector<double> args;
    stringstream ss(args_str);
    string token;
    while (getline(ss, token, ',')) {
        args.push_back(stod(token));
    }

    return FUNCTIONS[func_name](args);
}

double calculate(string expression) {
    try {
        expression = preprocess_expression(expression);

        expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());

        stack<double> values;
        stack<char> ops;

        auto applyOperation = [](double a, double b, char op) -> double {
            switch (op) {
                case '+': return a + b;
                case '-': return a - b;
                case '*': return a * b;
                case '/': return (b != 0) ? a / b : throw runtime_error("Деление на ноль!");
                default: throw runtime_error("Неизвестная операция.");
            }
        };

        for (size_t i = 0; i < expression.length(); ++i) {
            if (isdigit(expression[i]) || expression[i] == '.') {
                size_t end_pos;
                double value = stod(expression.substr(i), &end_pos);
                values.push(value);
                i += end_pos - 1;
            } else if (expression[i] == '(') {
                ops.push(expression[i]);
            } else if (expression[i] == ')') {
                while (!ops.empty() && ops.top() != '(') {
                    double b = values.top(); values.pop();
                    double a = values.top(); values.pop();
                    char op = ops.top(); ops.pop();
                    values.push(applyOperation(a, b, op));
                }
                if (!ops.empty() && ops.top() == '(') {
                    ops.pop();
                }

            } else if (string("+-*/").find(expression[i]) != string::npos) {
                while (!ops.empty() && string("*/").find(ops.top()) != string::npos && string("+-").find(expression[i]) != string::npos) {
                    double b = values.top(); values.pop();
                    double a = values.top(); values.pop();
                    char op = ops.top(); ops.pop();
                    values.push(applyOperation(a, b, op));
                }
                ops.push(expression[i]);
            } else {
                throw runtime_error("Некорректный символ в выражении.");
            }
        }

        while (!ops.empty()) {
            double b = values.top(); values.pop();
            double a = values.top(); values.pop();
            char op = ops.top(); ops.pop();
            values.push(applyOperation(a, b, op));
        }

        return values.top();

    } catch (exception& e) {
        throw runtime_error("Ошибка обработки выражения: " + string(e.what()));
    }
}

void menu() {
    cout << "=== Калькулятор ===" << endl;
    cout << "1. Ввести выражение" << endl;
    cout << "2. Показать историю" << endl;
    cout << "3. Очистить историю" << endl;
    cout << "4. Выход" << endl;
    cout << "Выберите пункт: ";
}

int main() {
    while (true) {
        menu();
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Введите выражение: ";
                string expression;
                cin.ignore();
                getline(cin, expression);

                try {
                    double result = calculate(expression);
                    cout << "Результат: " << result << endl;
                    history.push_back(expression + " = " + to_string(result));
                } catch (exception &e) {
                    cout << e.what() << endl;
                }
                break;
            }
            case 2:
                cout << "История вычислений:" << endl;
                for (const string& entry : history) {
                    cout << entry << endl;
                }
                break;
            case 3:
                history.clear();
                cout << "История успешно очищена." << endl;
                break;
            case 4:
                cout << "Выход из программы." << endl;
                return 0;
            default:
                cout << "Неверный выбор. Попробуйте снова." << endl;
        }
    }
}
