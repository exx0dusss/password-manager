#include "console.h"
#include <iostream>
#include <windows.h>
#include <ctime>
#include <filesystem>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include "../Encryptor/encryptor.h"

//std::cout << "\033[1A";  // Move the cursor up 1 line
//std::cout << "\033[10C"; // Move the cursor forward 10 columns
Console::Console() {
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "╔══════════════════════════════════╗" << std::endl;
    std::cout << "║ Welcome to the Password Manager! ║" << std::endl;
    std::cout << "║══════════════════════════════════║" << std::endl;
    std::cout << "║                                  ║" << std::endl;
    std::cout << "║       Choose option:             ║" << std::endl;
    std::cout << "║       1. Open database           ║" << std::endl;
    std::cout << "║       2. Create database         ║" << std::endl;
    std::cout << "║       3. Exit                    ║" << std::endl;
    std::cout << "║                                  ║" << std::endl;
    std::cout << "╚══════════════════════════════════╝\n\n\n\n\n" << std::endl;
    do {
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                            '\n');
            std::cout << "Wrong input!" << std::endl;
            continue;
        }
        std::string option;
        std::getline(std::cin, option);
        option.erase(option.find_last_not_of(" \t\r\n") + 1);
        switch (option[0]) {
            case '1':
                openDatabase();
                break;
            case '2':
                createDatabase();
                break;
            case '3':
                exit = true;
                break;
            default:
                print("Wrong input!");
                break;
        }
    } while (!exit);
}

void Console::createDatabase() {
    print("Input file path to create:");
    std::cin >> filePath;
    print("File: ", filePath);
    std::ifstream inputFile(filePath, std::ios::app);
    if (!is_empty(inputFile)) {
        print("The file may contain a database already!",
              "Override? 1/2");
        int choice;
        std::cin >> choice;
        if (choice == 2) {
            return;
        }
    }
    std::ofstream outputFile(filePath);
    if (!outputFile) {
        print("Failed to create file!");
        exit = true;
        return;
    }
    std::string password;
    while (true) {
        print("Create file password:");
        std::cin >> password;
        clearInput();
        if (isStrongPassword(password)) {
            break;
        }
        print("Password is weak!", "Input other? 1/2");
        int choice;
        std::cin >> choice;
        if (choice == 2) {
            break;
        }

    }

    outputFile << Encryptor::encrypt(password) << std::endl;
    print("Success! Remember to save this password!");

}

void Console::openDatabase() {
    print("Input source file path:");
    std::cin >> filePath;
    if (std::filesystem::exists(filePath)) {
        std::ifstream inputFile(filePath, std::ios::app);

        if (!inputFile) {
            print("Failed to open output file!");
            exit = true;
            return;
        }

        std::string password;
        print("Enter the file password:");
        std::cin >> password;

        std::string line;

        std::getline(inputFile, line);
        std::time_t currentTime = std::time(nullptr);
        std::tm *localTime = std::localtime(&currentTime);
        int year = localTime->tm_year + 1900;
        int month = localTime->tm_mon + 1;
        int day = localTime->tm_mday;

        int hour = localTime->tm_hour;
        int minute = localTime->tm_min;
        int second = localTime->tm_sec;

        std::string mdy = (std::to_string(month) + "/"
                           + std::to_string(day) + "/"
                           + std::to_string(year));
        std::string time = (std::to_string(hour) + ":"
                            + std::to_string(minute) + ":"
                            + std::to_string(second)
        );

        std::ofstream logFile("../data/logs.txt", std::ios::app);

        if (Encryptor::decrypt(line) == password) {
            print("Success!");
            logFile << mdy << " " << time << " - Success! Path: " << filePath << std::endl;
        } else {
            print("Failed!");
            logFile << mdy << " " << time << " - Failed! Path: " << filePath << std::endl;
        }
    } else {
        print("File doesn`t exist!");
        return;
    };

}

bool Console::is_empty(std::ifstream &pFile) {
    return pFile.peek() == std::ifstream::traits_type::eof();

}

bool Console::isStrongPassword(const std::string &password) {
    int length = 0;
    bool hasUpperCase = false;
    bool hasLowerCase = false;
    bool hasSpecialChar = false;
    bool hasNumber = false;
    for (auto c: password) {
        if (!hasUpperCase && Encryptor::getUpperCase().find(c) != std::string::npos) {
            hasUpperCase = true;
        }
        if (!hasLowerCase && Encryptor::getLowerCase().find(c) != std::string::npos) {
            hasLowerCase = true;
        }
        if (!hasSpecialChar && Encryptor::getSymbols().find(c) != std::string::npos) {
            hasSpecialChar = true;
        }
        if (!hasNumber && Encryptor::getNumerics().find(c) != std::string::npos) {
            hasNumber = true;
        }
        length++;
    }
    if (length >= 8 && hasUpperCase && hasLowerCase && hasSpecialChar && hasNumber) {
        return true;
    }
    return false;
}


void Console::clearInput() {
    std::cout << "\r";
    std::cout << "                                            ";
    std::cout << "\r";
}

void Console::print(const std::string &line1,
                    const std::string &line2,
                    const std::string &line3) {
    std::cout << "\r";
    std::cout << "\033[5A";
    std::cout << "\r";
    fmt::print("{}                                         ", line1);
    std::cout << "\r";
    std::cout << "\033[1B";
    std::cout << "\r";
    fmt::print("{}                                         ", line2);
    std::cout << "\r";
    std::cout << "\033[1B";
    std::cout << "\r";
    fmt::print("{}                                         ", line3);
    std::cout << "\r";
    std::cout << "\033[2B";
    std::cout << "\r";
    clearInput();

};
