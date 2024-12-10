#include <iostream>
#include <cmath>
#include <cstring>
#include <cctype>
#include <fstream>
using namespace std;

// �㭪�� ��� �஢�ન ��ࠦ���� �� ����������
bool validateExpression(const char* expr, int& errorPos) {
    for (int i = 0; expr[i] != '\0'; ++i) {
        if (!(isdigit(expr[i]) || expr[i] == '.' || expr[i] == '+' || expr[i] == '-' ||
              expr[i] == '*' || expr[i] == '/' || expr[i] == '^' || expr[i] == '(' ||
              expr[i] == ')' || isspace(expr[i]))) {
            cout << "�訡��: ��������� ᨬ���: " << expr[i] << "\n������: " << i << endl;
            errorPos = i; // ���࠭塞 ������ �����४⭮�� ᨬ����
            return false;
        }
    }
    return true;
}

// ����ਠ�
double fact(int n) {
    if (n < 0) return 0;
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

// �㭪�� ��� �믮������ ��䬥��᪨� ����権
double performOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b != 0) return a / b;
            else {
                cout << "�訡��: ������� �� ����!\n";
                return 0;
            }
        case '^': return power(a, b);
        default:
            cout << "�訡��: �������⭠� ������ " << op << endl;
            return 0;
    }
}

// �஢�ઠ �ਮ��� �����஢
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3; // ��᮪�� �ਮ��� ��� ���������� � �⥯���
    if (op == '!') return 4; // ���訩 �ਮ��� ��� 䠪�ਠ��
    return 0;
}

// �᭮���� �㭪�� ��� ���᫥��� ��ࠦ����
double eval(const char* expr) {
    double values[100];
    char operators[100];
    int valuesTop = -1, operatorsTop = -1;
    bool validExpression = false; // ���� ��� �஢�ન ������ ���४��� ����権

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
                if (valuesTop < 1) { // �஢�ઠ �� �����筮� ������⢮ ���࠭���
                    cout << "�訡��: �����४⭮� ��ࠦ����.\n";
                    return 0;
                }
                char op = operators[operatorsTop--];
                double b = values[valuesTop--];
                double a = values[valuesTop--];
                values[++valuesTop] = performOp(a, b, op);
            }
            if (operatorsTop < 0 || operators[operatorsTop] != '(') {
                cout << "�訡��: �����४�� ᪮���.\n";
                return 0;
            }
            operatorsTop--;
        } else if (strchr("+-*/^!", expr[i])) {
            validExpression = true;
            while (operatorsTop >= 0 && precedence(operators[operatorsTop]) >= precedence(expr[i])) {
                if (valuesTop < 1) {
                    cout << "�訡��: �����४⭮� ��ࠦ����.\n";
                    return 0;
                }
                char op = operators[operatorsTop--];
                double b = values[valuesTop--];
                double a = values[valuesTop--];
                values[++valuesTop] = performOp(a, b, op);
            }
            operators[++operatorsTop] = expr[i];
        } else if (isalpha(expr[i])) {
            cout << "�訡��: �����४�� ᨬ��� � ��ࠦ����: " << expr[i] << endl;
            return 0;
        }
    }

    while (operatorsTop >= 0) {
        if (valuesTop < 1) {
            cout << "�訡��: �����४⭮� ��ࠦ����.\n";
            return 0;
        }
        char op = operators[operatorsTop--];
        double b = values[valuesTop--];
        double a = values[valuesTop--];
        values[++valuesTop] = performOp(a, b, op);
    }

    if (!validExpression) {
        cout << "�訡��: ���⮥ ��� �����४⭮� ��ࠦ����.\n";
        return 0;
    }

    return valuesTop == 0 ? values[0] : 0;
}

void saveHistoryToFile(const char history[][100], int historyIndex) {
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

            int errorPos;
            if (!validateExpression(expression, errorPos)) {
                cout << expression << endl;
                for (int i = 0; i < errorPos; ++i) cout << " ";
                cout << "^\n�訡��: ��������� ᨬ���\n";
                continue;
            }

            result = eval(expression);
            cout << "�������: " << result << endl;

            // ���࠭���� � �����
            if (historyIndex < 100) { // ��࠭�祭�� ࠧ��� ���ਨ
                snprintf(history[historyIndex], 100, "��ࠦ����: %s, �������: %.2f", expression, result);
                ++historyIndex;
            } else {
                cout << "����� ���������, ����� ������ ����������.\n";
            }
        } else if (choice == 2) {
            if (historyIndex == 0) {
                cout << "����� ����.\n";
            } else {
                cout << "--- ����� ���᫥��� ---\n";
                for (int i = 0; i < historyIndex; i++) {
                    cout << history[i] << endl;
                }
            }
        } else if (choice == 3) {
            historyIndex = 0;
            cout << "\b=====================\n";
            cout << "����� ��饭�.\n";
            cout << "=====================\n";
        } else if (choice == 4) {
            saveHistoryToFile(history, historyIndex);
            cout << "����� ��࠭��� � 䠩� history.txt\n";
        } else if (choice == 5) {
            cout << "��室...\n";
            break;
        } else {
            cout << "�����४�� �롮�! ��������, �롥�� ᭮��.\n";
        }
    }
    return 0;
}