#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <map>
#include <functional>
#include <cctype>

using namespace std;

double eval_(const string& func_name, const string& args_str);

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
        {"max", [](const vector<double>& args) { return maximum(args); }},
        {"min", [](const vector<double>& args) { return minimum(args); }},
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

string preprocess(string expression) {
    size_t pos = expression.find("**");
    while (pos != string::npos) {
        expression.replace(pos, 2, "^");
        pos = expression.find("**");
    }
    return expression;
}

double calculate(string expression) {
    try {
        expression = preprocess(expression);

        if (expression[0] == '!') {
            double num = stod(expression.substr(1));
            return factorial(num);
        }

        if (expression.back() == '!') {
            double num = stod(expression.substr(0, expression.size() - 1));
            return factorial(num);
        }

        if (expression.find('^') != string::npos) {
            size_t pos = expression.find('^');
            double base = stod(expression.substr(0, pos));
            double exponent = stod(expression.substr(pos + 1));
            return pow(base, exponent);
        }

        size_t open_bracket = expression.find('(');
        if (open_bracket != string::npos) {
            string func_name = expression.substr(0, open_bracket);
            string args_str = expression.substr(open_bracket + 1, expression.size() - open_bracket - 2);
            return eval_(func_name, args_str);
        }

        return stod(expression);

    } catch (exception& e) {
        throw runtime_error("Ошибка обработки выражения: " + string(e.what()));
    }
}

double eval_(const string& func_name, const string& args_str) {
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
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Некорректный ввод. Пожалуйста, попробуйте ещё раз." << endl;
            continue;
        }
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
                return 0;
            default:
                cout << "Неверный выбор. Попробуйте снова." << endl;
        }
    }
}
