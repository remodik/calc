#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cctype>
using namespace std;

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
    return log(value) / log(base); // ���㫠 ��� �����䬠 �� �ந����쭮�� �᭮�����
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
        default:
            cout << "�訡��: �������⭠� ������ " << op << endl;
            return 0;
    }
}

// �஢�ઠ �ਮ��� �����஢
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '!') return 3; // ��᮪�� �ਮ��� ��� 䠪�ਠ��
    return 0;
}

// �᭮���� �㭪�� ��� ���᫥��� ��ࠦ����
double eval(const char* expr) {
    double values[100]; // �⥪ �ᥫ
    char operators[100]; // �⥪ �����஢
    int valuesTop = -1, operatorsTop = -1;

    for (int i = 0; expr[i] != '\0'; ++i) {
        if (isdigit(expr[i]) || expr[i] == '.') {
            // �⥭�� �᫠
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
            i--; // �����頥��� �� 蠣 �����
        } else if (expr[i] == '(') {
            operators[++operatorsTop] = expr[i];
        } else if (expr[i] == ')') {
            while (operatorsTop >= 0 && operators[operatorsTop] != '(') {
                if (operators[operatorsTop] == '!') {
                    // ��ࠡ�⪠ 䠪�ਠ��
                    double a = values[valuesTop--];
                    values[++valuesTop] = fact(int(a)); // �८�ࠧ㥬 � int
                    operatorsTop--;
                } else {
                    double b = values[valuesTop--];
                    double a = values[valuesTop--];
                    char op = operators[operatorsTop--];
                    values[++valuesTop] = performOp(a, b, op);
                }
            }
            operatorsTop--; // ���ࠥ� '('
        } else if (expr[i] == '!' || expr[i] == '+' || expr[i] == '-' ||
                   expr[i] == '*' || expr[i] == '/') {
            while (operatorsTop >= 0 && precedence(operators[operatorsTop]) >= precedence(expr[i])) {
                if (operators[operatorsTop] == '!') {
                    // ��ࠡ�⪠ 䠪�ਠ��
                    double a = values[valuesTop--];
                    values[++valuesTop] = fact(int(a)); // �८�ࠧ㥬 � int
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
            // ��ࠡ�⪠ �����䬠
            i += 3; // �ய�᪠�� "log"
            if (expr[i] == '(') {
                i++; // �ய�᪠�� '('
                double base = 0, value = 0;
                bool readingBase = true;
                while (expr[i] != ',') {
                    base = base * 10 + (expr[i] - '0');
                    i++;
                }
                i++; // �ய�᪠�� �������
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
    cout << "--- �������� ---\n";
    cout << "1. ����� ��ࠦ����\n";
    cout << "2. �������� ����� ���᫥���\n";
    cout << "3. ������ �����\n";
    cout << "4. ��室\n";
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
        cin.ignore();  // ��� �����஢���� ᨬ���� ����� ��ப� ��᫥ �롮� ����⢨�

        if (choice == 1) {
            cout << "������ ��ࠦ����: ";
            cin.getline(expression, 100);
            result = eval(expression);

            // ���࠭���� १���� � �����
            sprintf(history[historyIndex], "��ࠦ����: %s, �������: %.2f", expression, result);
            historyIndex++;

            cout << "�������: " << result << endl;
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
            historyIndex = 0;  // ���⪠ ���ਨ
            cout << "����� ��饭�.\n";
        } else if (choice == 4) {
            cout << "��室...\n";
            break;
        } else {
            cout << "�����४�� �롮�! ��������, �롥�� ᭮��.\n";
        }
    }
    return 0;
}
