#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <limits>
#include <sstream>
#define MAX_INPUT_LENGTH 256

using namespace std;

// �㭪�� ��� ���᫥��� 䠪�ਠ��
double factorial(int n) {
    if (n < 0) return -1; // �訡�� ��� ����⥫��� �ᥫ
    double result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// �㭪�� ��� ���᫥��� �����䬠
double custom_log(double base, double value) {
    return log(value) / log(base);
}

// �㭪�� ��� �믮������ ����樨
double calculate(double num1, char op, double num2) {
    switch (op) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return num2 != 0 ? num1 / num2 : NAN; // ������� �� 0
        case '^': return pow(num1, num2);
        default: return NAN; // �������⭠� ������
    }
}

// ������ � �믮������ ᫮���� ����権
double evaluateExpression(const char* input) {
    char temp[MAX_INPUT_LENGTH];
    strncpy(temp, input, MAX_INPUT_LENGTH);

    // �஢�ઠ �� ������ (log(base, value))
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

    // ��ࠡ�⪠ ��ࠦ���� � ������� stringstream
    std::stringstream ss(temp);
    double result = 0, num = 0;
    char op = '+';

    while (ss >> num) {
        // �஢�ઠ �� 䠪�ਠ� (!�᫮)
        if (ss.peek() == '!') {
            ss.get(); // ���ࠥ� '!'
            num = factorial(static_cast<int>(num)); // ����塞 䠪�ਠ�
        }
        // �믮������ ����樨
        switch (op) {
            case '+': result += num; break;
            case '-': result -= num; break;
            case '*': result *= num; break;
            case '/': result /= num; break;
            case '^': result = pow(result, num); break; // �⥯���
        }
        ss >> op; // ���뢠�� ᫥���騩 ������
    }

    return result;
}

// �㭪�� ����
void showMenu() {
    cout << "��������:\n";
    cout << "1. ���� ��ࠦ����\n";
    cout << "2. �������� �����\n";
    cout << "3. ������ �����\n";
    cout << "4. ��室\n\n";
}

// �㭪�� ��� ������ ���ਨ ����権
void showHistory() {
    FILE* file = fopen("history.txt", "r");
    if (!file) {
        cout << "����� ����.\n";
        return;
    }
    char line[MAX_INPUT_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        cout << line;
    }
    fclose(file);
}

// �㭪�� ��� ����� १���� � �����
void saveHistory(const char* input, double result) {
    FILE* file = fopen("history.txt", "a");
    if (!file) return;
    fprintf(file, "%s = %.6f\n", input, result);
    fclose(file);
}

// �㭪�� ��� ���⪨ ���ਨ
void clearHistory() {
    cout << "����쪮 ����権 ������ (* ��� ���⪨ �ᥣ�): ";
    char input[MAX_INPUT_LENGTH];
    cin >> input;

    if (strcmp(input, "*") == 0) {
        // ������ ���⪠ ���ਨ
        FILE* file = fopen("history.txt", "w");
        if (file) fclose(file);
        cout << "����� ��������� ��饭�.\n";
    } else {
        // ���⪠ 㪠������� ������⢠ ��ப
        int numToRemove = atoi(input);
        if (numToRemove <= 0) {
            cout << "�����४�� ����.\n";
            return;
        }

        FILE* file = fopen("history.txt", "r");
        if (!file) {
            cout << "����� ����.\n";
            return;
        }

        // �⥭�� ��⠢���� ��ப � ������
        char lines[MAX_INPUT_LENGTH][MAX_INPUT_LENGTH];
        int count = 0;
        while (fgets(lines[count], sizeof(lines[count]), file)) {
            ++count;
        }
        fclose(file);

        // ��१����� 䠩�� ��� 㤠������ ��ப
        file = fopen("history.txt", "w");
        if (!file) return;
        for (int i = numToRemove; i < count; ++i) {
            fputs(lines[i], file);
        }
        fclose(file);
        cout << "������� " << min(numToRemove, count) << " ����権 �� ���ਨ.\n";
    }
}

int main() {
    char input[MAX_INPUT_LENGTH];
    int choice;

    while (true) {
        showMenu();
        cout << "�롥�� ����: ";
        cin >> choice;

        // �஢�ઠ �� �訡�� �����
        if (cin.fail()) {
            cin.clear(); // ���� 䫠�� �訡��
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ���⪠ �����
            cout << "�訡��: ������ ���४⭮� �᫮.\n";
            continue; // �����頥��� � ��砫� 横��
        }

        switch (choice) {
            case 1: {
                cout << "������ ��ࠦ����: ";
                cin.ignore(); // ���⪠ ���⪮� �����
                cin.getline(input, MAX_INPUT_LENGTH);

                double result = evaluateExpression(input);
                if (!isnan(result)) {
                    cout << "�������: " << result << endl;
                    saveHistory(input, result);
                } else {
                    cout << "�訡��: ����୮� ��ࠦ����.\n";
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
                cout << "��室 �� �ணࠬ��.\n";
                return 0; // �����襭�� �ணࠬ��
            }
            default: {
                cout << "�訡��: ������ �롮�. ������ �᫮ �� 1 �� 4.\n";
                break;
            }
        }
    }
}