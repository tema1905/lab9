#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <Windows.h>

using namespace std;

enum LogType {
    USER_INPUT,
    LOG_ERROR,
    ATTEMPTS,
    GENERATED_NUMBER
};

struct LogEntry {
    LogType type;
    string message;
    string timestamp;

    LogEntry(LogType t, const string& msg) : type(t), message(msg) {
        timestamp = getCurrentTime();
    }

    static string getCurrentTime() {
        time_t now = std::time(nullptr);
        tm localtime;
        localtime_s(&localtime, &now);

        ostringstream oss;
        oss << put_time(&localtime, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
};

void logMessage(const LogEntry& entry, const string& filename) {
    ofstream logFile(filename, ios_base::app);
    if (logFile.is_open()) {
        logFile << "[" << entry.timestamp << "] "
            << (entry.type == USER_INPUT ? "USER_INPUT" :
                entry.type == LOG_ERROR ? "ERROR" :
                entry.type == ATTEMPTS ? "ATTEMPTS" : "GENERATED_NUMBER")
            << ": " << entry.message << endl;
        logFile.close();
    }
    else {
        cerr << "Ошибка открытия файла лога!" << endl;
    }
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Rus");

    int N;

    while (true) {
        cout << "Введите количество бочонков от 1 до 100 включительно: ";
        cin >> N;

        if (cin.fail() || N < 1 || N > 100) {
            cin.clear();
            cin.ignore();
            cout << "Вы ввели некорректное число! Попробуйте снова." << endl;
            logMessage(LogEntry(LOG_ERROR, "Некорректный ввод числа"), "log.txt");
            continue;
        }


        logMessage(LogEntry(USER_INPUT, "Пользователь ввел количество бочонков: " + to_string(N)), "log.txt");
        break;
    }


    srand(static_cast<unsigned int>(time(0)));

    vector<int> drawnNumbers;

    while (true) {
        string input;
        cout << "Нажмите Enter для вытаскивания бочонка или введите 'exit' для выхода: ";

        while (true) {
            getline(cin, input);

            if (input == "exit") {
                cout << "Игра завершена." << endl;
                return 0;
            }
            else if (input.empty()) {
                break;
            }
            else {
                cout << "Некорректный ввод! Пожалуйста, введите 'exit' для выхода или нажмите Enter для продолжения: ";
                logMessage(LogEntry(USER_INPUT, "Пользователь ввел некорректную команду: " + input), "log.txt");
            }
        }


        if (drawnNumbers.size() >= N) {
            cout << "Все бочонки уже вытянуты!" << endl;
            break;
        }

        int number;
        do {
            number = rand() % N + 1;
        } while (find(drawnNumbers.begin(), drawnNumbers.end(), number) != drawnNumbers.end());

        drawnNumbers.push_back(number);


        logMessage(LogEntry(GENERATED_NUMBER, "Вытянут бочонок: " + to_string(number)), "log.txt");

        cout << "Вытянут бочонок: " << number << endl;
    }

    return 0;
}