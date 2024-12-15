#include <iostream>
#include <cstring>
#include <cctype>
#include <cmath>
#include <fstream>
using namespace std;

// �㭪�� ��� �஢�ન ��ࠦ����
bool validExpr(const char* expr) {
    for (int i = 0; expr[i] != '\0'; ++i) {
        // �஢��塞 ᨬ����: ����, �������, ᪮���, �஡��� � "log"
        if (!(isdigit(expr[i]) || strchr("+-*/^()!., ", expr[i]) ||
              (strncmp(&expr[i], "log", 3) == 0) || isalpha(expr[i]))) {
            cout << "�訡��: ��������� ᨬ���: " << expr[i] << " �� ����樨 " << i << endl;
            return false;
        }
    }
    return true;
}

// ����ਠ�
double fact(int n) {
    if (n < 0) return 0;
    if (n == 0) return 1;
    double result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// ������
double logFunc(double base, double value) {
    if (base <= 0 || base == 1 || value <= 0) {
        cout << "�訡��: �����४�� ��ࠬ���� ��� �����䬠.\n";
        return 0;
    }
    return log(value) / log(base);
}

// ���������� � �⥯���
double power(double base, double exp) {
    return pow(base, exp);
}

// �믮������ ����権
double performOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b != 0) return a / b;
            cout << "�訡��: ������� �� ����!\n";
            return 0;
        case '^': return power(a, b);
        default: return 0;
    }
}

// �஢�ઠ �ਮ��� �����஢
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

// ���᫥��� ��ࠦ����
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

            // ���뢠�� �᫮ � ��������� �஡��� �����
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
            opTop--;  // ���ࠥ� '('
            i++;
        } else if (strncmp(&expr[i], "log", 3) == 0) {
            i += 3;
            if (expr[i] == '(') {
                i++;
                double base = 0, value = 0;
                sscanf(&expr[i], "%lf,%lf", &base, &value);

                // �ய����� ��ࠬ���� �� ')'
                while (expr[i] != ')' && expr[i] != '\0') i++;

                if (expr[i] == ')') {
                    i++; // �ய�᪠�� ')'
                    values[++valTop] = logFunc(base, value);
                } else {
                    cout << "�訡��: �����४�� �ଠ� �㭪樨 log. ��������� log(base,value).\n";
                    return 0;
                }
            } else {
                cout << "�訡��: �����४�� �ଠ� �㭪樨 log. ��������� log(base,value).\n";
                return 0;
            }
        } else if (expr[i] == '!') {
            double a = values[valTop--];
            values[++valTop] = fact((int)a);
            i++;
        } else if (strchr("+-*/^", expr[i])) {
            // ��ࠡ��뢠�� ������� � ���⮬ �ਮ���
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

    // �믮������ ��⠢���� ����権
    while (opTop >= 0) {
        double b = values[valTop--];
        double a = values[valTop--];
        char op = operators[opTop--];
        values[++valTop] = performOp(a, b, op);
    }

    return values[valTop];
}

// ���࠭���� ���ਨ
void saveHistory(const char history[][100], int historyIndex) {
    ofstream outFile("history.txt", ios::out);
    if (!outFile) {
        cout << "�訡��: �� 㤠���� ������ 䠩� ��� �����.\n";
        return;
    }
    for (int i = 0; i < historyIndex; ++i) {
        outFile << history[i] << endl;
    }
    outFile.close();
}

// ����
void showMenu() {
    cout << "--- �������� ---\n";
    cout << "1. ����� ��ࠦ����\n";
    cout << "2. �������� ����� ���᫥���\n";
    cout << "3. ������ �����\n";
    cout << "4. ���࠭��� ����� � 䠩�\n";
    cout << "5. ��室\n";
}

int main() {
    double result;
    char expression[100];
    char history[100][100];
    int historyIndex = 0;
    int choice;

    while (true) {
        showMenu();
        cout << "�롥�� ����⢨�: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            cout << "������ ��ࠦ����: ";
            cin.getline(expression, 100);

            if (!validExpr(expression)) {
                continue;
            }

            result = eval(expression);
            cout << "�������: " << result << endl;

            if (historyIndex < 100) {
                snprintf(history[historyIndex], 100, "��ࠦ����: %s, �������: %.2f", expression, result);
                ++historyIndex;
            } else {
                cout << "����� ���������.\n";
            }
        } else if (choice == 2) {
            if (historyIndex == 0) {
                cout << "����� ����.\n";
            } else {
                for (int i = 0; i < historyIndex; i++) {
                    cout << history[i] << endl;
                }
            }
        } else if (choice == 3) {
            historyIndex = 0;
            cout << "����� ��饭�.\n";
        } else if (choice == 4) {
            saveHistory(history, historyIndex);
            cout << "����� ��࠭��� � 䠩� history.txt\n";
        } else if (choice == 5) {
            cout << "��室...\n";
            break;
        } else {
            cout << "�����४�� �롮�!\n";
        }
    }
}
