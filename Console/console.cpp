#include "console.h"
#include <iostream>
#include <windows.h>
#include <ctime>
#include <filesystem>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include "../Encryptor/encryptor.h"
#include <iomanip>
#include <sstream>
#include <random>

//std::cout << "\033[1A";  // Move the cursor up 1 line
Console::Console() {
    bool exit = false;
    SetConsoleOutputCP(CP_UTF8);
    fmt::print("\n--- Welcome to Password Manager! ---\n");

    do {

        fmt::print("\nSelect option:\n");
        fmt::print("1. Open database\n");
        fmt::print("2. Create database\n");
        fmt::print("3. Exit\n");

        std::string option;
        std::cin >> option;
//	  std::getline (std::cin, option);
//	  option.erase (option.find_last_not_of (" \t\r\n") + 1);
        switch (option[0]) {
            case '1':
                if (openDatabase()) {
                    passwords = getPasswords();
                    if (passwords.empty()) {
                        addCategory();
                        addPassword();
                    }
                    printActions();


                }

                break;
            case '2':
                createDatabase();
                break;
            case '3':
                writePasswords(getFilePassword(), passwords);
                exit = true;
                break;
            default:
                fmt::print("Wrong Input!\n");
                break;
        }
    } while (!exit);
}

bool Console::openDatabase() {
    fmt::print("\nInput source file path:\n");
    std::cin >> filePath;
    std::string filePassword = getFilePassword();
    if (filePassword.empty()) {
        return false;
    }
    std::string password;
    fmt::print("\nEnter the file password:\n");
    std::cin >> password;

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

    if (password == Encryptor::decrypt(filePassword)) {
        fmt::print("\nSuccess!\n");
        logFile << mdy << " " << time << " - Success! Path: " << filePath << std::endl;
        return true;
    } else {
        fmt::print("\nFailed!\n");
        logFile << mdy << " " << time << " - Failed! Path: " << filePath << std::endl;
        return false;
    }

};


void Console::createDatabase() {
    fmt::print("\nInput file path to create:\n");
    std::cin >> filePath;
    std::ifstream inputFile(filePath, std::ios::app);
    if (!is_empty(inputFile)) {
        fmt::print("\nThe file may contain a database already!\n Override? (1/2)\n");
        int choice;
        std::cin >> choice;
        if (choice == 2) {
            return;
        }
    }
    std::ofstream outputFile(filePath);
    if (!outputFile) {
        fmt::print("\nFailed to create file!\n");
        return;
    }
    std::string password = createPassword();
    outputFile << Encryptor::encrypt(password) + "\n" << std::endl;
    fmt::print("\nSuccess! Remember to save this password!\n");

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

void Console::printActions() {
    do {
        fmt::print("\n--- Data management ---\n");
        fmt::print("Select option:\n");
        fmt::print("1. Show Passwords\n");
        fmt::print("2. Add Password\n");
        fmt::print("3. Edit Password\n");
        fmt::print("4. Delete Password\n");
        fmt::print("5. Search passwords\n");
        fmt::print("6. Sort passwords\n");
        fmt::print("7. Add Category\n");
        fmt::print("8. Delete Category\n");
        fmt::print("9. Back\n\n");
        std::string option;
        std::cin >> option;
        switch (option[0]) {
            case '1':
                printPasswords();
                break;
            case '2':
                addPassword();
                break;
            case '3':
                editPassword();
                break;
            case '4':
                deletePassword();
                break;
            case '5':
                searchPasswords();
                break;
            case '6':
                sortPasswords();
                break;
            case '7':
                addCategory();
                break;
            case '8':
                deleteCategory();
                break;
            case '9':
                writePasswords(getFilePassword(), passwords);
                return;
            default:
                fmt::print("Wrong Input!\n");
                break;
        }

    } while (true);

}

void Console::addPassword() {
    std::string name;
    std::string passwordCategories;
    std::string password;
    std::string service;
    std::string login;
    fmt::print("\nEnter name:\n");
    std::cin >> name;
    password = createPassword();
    do {
        fmt::print("\nEnter category name:\n");
        std::string category;

        std::cin >> category;
        if (findCategory(categories, category)) {
            passwordCategories += (category + ',');
            std::string option;
            fmt::print("\nAdd one more? (1/2)\n");
            std::cin >> option;
            if (option[0] == '2')
                break;
            continue;

        }
        fmt::print("\nCategory missing!\n");
        fmt::print("\nCreate new? (1/2)\n");
        std::string option1;
        std::cin >> option1;
        if (option1[0] == '2') {
            fmt::print("\nPassword wasn't created!\n");
            return;
        }
        addCategory();
    } while (true);

    fmt::print("\nAdd Website/Service and Login? (1/2)\n");
    std::string option;
    std::cin >> option;
    if (option[0] == '1') {
        fmt::print("\nEnter service name:\n");
        std::cin >> service;
        fmt::print("\nEnter login:\n");
        std::cin >> login;

    }
    name = Encryptor::encrypt(name);
    password = Encryptor::encrypt(password);
    std::cout << passwordCategories << std::endl;

    passwordCategories = Encryptor::encrypt(passwordCategories);
    service = Encryptor::encrypt(service);
    login = Encryptor::encrypt(login);
    passwords.push_back(
            *new Password(name, password, passwordCategories, service, login)
    );
}

void Console::editPassword() {
    fmt::print("\nEnter the name of password data:\n");
    std::string option;
    std::cin >> option;
    if (findPassword(option)) {
        fmt::print("\nEnter the index to edit:\n");
        std::string index;
        std::cin >> index;
        Password editPassword = passwords[index[0]];
        std::string name;
        std::string passwordCategories;
        std::string password;
        std::string service;
        std::string login;
        name = editPassword.get_name();
        passwordCategories = editPassword.getCategories();
        password = createPassword();
        service = editPassword.get_service();
        login = editPassword.get_login();
        passwords.erase(passwords.begin() + index[0]);
        passwords.push_back(*new Password(name, passwordCategories, password, service, login));
        return;
    }
    fmt::print("\nNo passwords with such name\n");
}

void Console::deletePassword() {
    fmt::print("\nEnter the name of password data:\n");
    std::string option;
    std::cin >> option;
    std::vector<int> indicesToDelete;

    if (findPassword(option)) {
        fmt::print("\nChoose Index (Enter 'end' when done):\n");
        std::string index;
        while (std::cin >> index) {
            if (index == "end") {
                break;
            }
            int parsedIndex = std::stoi(index);
            indicesToDelete.push_back(parsedIndex);
        }

        std::sort(indicesToDelete.begin(), indicesToDelete.end(), std::greater<int>());

        for (int index: indicesToDelete) {
            if (index >= 0 && index < passwords.size()) {
                passwords.erase(passwords.begin() + index);
            }
        }

        fmt::print("\nPassword(s) deleted successfully.\n");
        return;
    }

    fmt::print("\nNo passwords with such name\n");
}


void Console::searchPasswords() {
    fmt::print("Select search option:\n");
    fmt::print("1. Search by Name\n");
    fmt::print("2. Search by Category\n");
    fmt::print("3. Search by Login\n");
    fmt::print("4. Search by Service\n");

    std::string option;
    std::cin >> option;

    std::string searchValue;
    fmt::print("Enter the search value:\n");
    std::cin >> searchValue;

    bool found = false;

    switch (option[0]) {
        case '1':
            for (const auto &password: passwords) {
                std::string name = Encryptor::decrypt(password.get_name());
                if (name == searchValue) {
                    printPassword(password);
                    found = true;
                }
            }
            break;
        case '2':
            for (const auto &password: passwords) {
                std::string categories = Encryptor::decrypt(password.getCategories());
                std::stringstream ss(categories);
                std::string category;
                while (std::getline(ss, category, ',')) {
                    if (category == searchValue) {
                        printPassword(password);
                        found = true;
                        break;
                    }
                }
            }
            break;
        case '3':
            for (const auto &password: passwords) {
                std::string login = Encryptor::decrypt(password.get_login());
                if (login == searchValue) {
                    printPassword(password);
                    found = true;
                }
            }
            break;
        case '4':
            for (const auto &password: passwords) {
                std::string service = Encryptor::decrypt(password.get_service());
                if (service == searchValue) {
                    printPassword(password);
                    found = true;
                }
            }
            break;
        default:
            fmt::print("Wrong Input!\n");
            return;
    }

    if (!found) {
        fmt::print("\nNo passwords with such {} found.\n", (option[0] == '1' ? "name" :
                                                            (option[0] == '2' ? "category" :
                                                             (option[0] == '3' ? "login" :
                                                              (option[0] == '4' ? "service" : "")))));
    }
}

void Console::sortPasswords() {
    fmt::print("Select sorting option:\n");
    fmt::print("1. Sort by Name\n");
    fmt::print("2. Sort by Category\n");
    fmt::print("3. Sort by Login\n");
    fmt::print("4. Sort by Service\n");

    std::string option;
    std::cin >> option;

    std::function<bool(const Password &, const Password &)> comparator;

    switch (option[0]) {
        case '1':
            comparator = [](const Password &a, const Password &b) {
                std::string nameA = Encryptor::decrypt(a.get_name());
                std::string nameB = Encryptor::decrypt(b.get_name());
                return nameA < nameB;
            };
            break;
        case '2':
            comparator = [](const Password &a, const Password &b) {
                std::string categoryA = Encryptor::decrypt(a.getCategories());
                std::string categoryB = Encryptor::decrypt(b.getCategories());
                return categoryA < categoryB;
            };
            break;
        case '3':
            comparator = [](const Password &a, const Password &b) {
                std::string loginA = Encryptor::decrypt(a.get_login());
                std::string loginB = Encryptor::decrypt(b.get_login());
                return loginA < loginB;
            };
            break;
        case '4':
            comparator = [](const Password &a, const Password &b) {
                std::string serviceA = Encryptor::decrypt(a.get_service());
                std::string serviceB = Encryptor::decrypt(b.get_service());
                return serviceA < serviceB;
            };
            break;
        default:
            fmt::print("Wrong Input!\n");
            return;
    }

    std::sort(passwords.begin(), passwords.end(), comparator);

    fmt::print("\n--- Sorted Passwords ---\n");
    for (const auto &password: passwords) {
        printPassword(password);
        fmt::print("\n");
    }
}


void Console::printPassword(const Password &data) {
    std::string name;
    std::string categories;
    std::string password;
    std::string service;
    std::string login;
    name = Encryptor::decrypt(data.get_name());
    categories = Encryptor::decrypt(data.getCategories());
    password = Encryptor::decrypt(data.get_password());
    service = Encryptor::decrypt(data.get_service());
    login = Encryptor::decrypt(data.get_login());
    fmt::print("Name: {}\n", name);
    fmt::print("Categories: {}\n", categories);
    fmt::print("Password: {}\n", password);
    fmt::print("Service: {}\n", service);
    fmt::print("Login: {}\n", login);
}

void Console::printPasswords() {
    int index = 0;
    for (const auto &data: passwords) {
        fmt::print("\n{}.\n", index++);
        printPassword(data);
    }
}

void Console::addCategory() {
    std::string newCategory;
    fmt::print("\nEnter a new category name\n");
    std::cin >> newCategory;
    if (!findCategory(categories, newCategory)) {
        categories.append(Encryptor::encrypt(newCategory) + ',');
        return;

    }
    fmt::print("\nCategory already exists!\n");


}

void Console::deleteCategory() {
    std::string categoryToRemove;
    fmt::print("\nEnter a new category name\n");
    std::cin >> categoryToRemove;
    if (findCategory(categories, categoryToRemove)) {

        for (const auto &data: passwords) {
            auto passwordCategories = data.getCategories();
            if (findCategory(passwordCategories, categoryToRemove)) {
                std::string option;
                fmt::print("\nSome passwords may contain this category\nConfirm? (1/2)\n");
                std::cin >> option;
                if (option[0] == '2') {
                    return;
                }
                break;
            }
        }
        for (auto it = passwords.begin(); it != passwords.end();) {
            auto passwordCategories = it->getCategories();

            if (findCategory(passwordCategories, categoryToRemove)) {
                it = passwords.erase(it);

            } else {
                ++it;
            }

        }
        return;
    }
    fmt::print("\nCategory not found!\n");
}

std::string Console::createPassword() {
    std::string password;
    while (true) {
        fmt::print("\nChoose an option:\n");
        fmt::print("1. Generate a password\n");
        fmt::print("2. Enter a password manually\n");
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            password = generatePassword();
            fmt::print("\nGenerated password: {}\n", password);
        } else if (choice == 2) {
            fmt::print("\nEnter a new password:\n");
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> password;
        } else {
            fmt::print("\nInvalid choice. Please try again.\n");
            continue;
        }

        if (!findUsedPassword(password)) {
            return password;
        } else {
            fmt::print("\nThis password is already used.\n");
            fmt::print("Would you like to try again? (1/2)\n");
            std::cin >> choice;
            if (choice == 2) {
                break;
            }
        }

        if (!isStrongPassword(password)) {
            fmt::print("\nPassword is weak!\n");
            fmt::print("Would you like to try again? (1/2)\n");
            std::cin >> choice;
            if (choice == 2) {
                return password;
            }
        }

    }
    return "";
}

std::string Console::generatePassword() {
    const std::string &lowerCase = Encryptor::getLowerCase();
    const std::string &upperCase = Encryptor::getUpperCase();
    const std::string &numerics = Encryptor::getNumerics();
    const std::string &symbols = Encryptor::getSymbols();

    std::string password;
    std::random_device rd;
    std::mt19937 gen(rd());

    int length;
    bool includeUpperCase;
    bool includeSymbols;

    fmt::print("\nEnter the desired length of the password: ");
    std::cin >> length;

    fmt::print("Include uppercase letters? (1/2): ");
    std::string choice;
    std::cin >> choice;
    includeUpperCase = (choice[0]);

    fmt::print("Include special symbols? (1/2): ");
    std::cin >> choice;
    includeSymbols = (choice[0]);

    std::string charSet;
    charSet += lowerCase;
    if (includeUpperCase) {
        charSet += upperCase;
    }
    charSet += numerics;
    if (includeSymbols) {
        charSet += symbols;
    }

    if (charSet.empty()) {
        fmt::print("\nError: No character set selected.\n");
        return "";
    }

    for (int i = 0; i < length; ++i) {
        std::uniform_int_distribution<> dis(0, charSet.size() - 1);
        password += charSet[dis(gen)];
    }

    return password;
}


bool Console::findCategory(const std::string &sourceCategories, const std::string &categoryToFind) {
    std::istringstream iss(sourceCategories);
    std::string category;
    while (std::getline(iss, category, ',')) {
        if (category == Encryptor::encrypt(categoryToFind)) {
            return true;
        }
    }
    return false;
}


std::string Console::getFilePassword() {
    if (std::filesystem::exists(filePath)) {
        std::ifstream inputFile(filePath, std::ios::app);

        if (!inputFile) {
            fmt::print("\nFailed to open output file!\n");
            return "";
        }

        std::string line;
        std::getline(inputFile, line);
        return line;


    } else {
        fmt::print("\nFile doesn't exist!\n");
        return "";
    }


}

std::vector<Password> Console::getPasswords() {
    if (std::filesystem::exists(this->filePath)) {
        std::ifstream inputFile(filePath, std::ios::app);

        if (!inputFile) {
            fmt::print("\nFailed to open output file!\n");
            return {};
        }

        std::string line;
        std::getline(inputFile, line);
        std::vector<Password> oldPasswords;


        char splitter = (char) (0x0 + 685);
        while (std::getline(inputFile, line)) {
            std::istringstream iss(line);
            std::string value;
            std::vector<std::string> values;
            while (std::getline(iss, value, splitter)) {
                values.push_back(value);
            }
            std::string name;
            std::string passwordCategories;

            std::string password;
            std::string service;
            std::string login;
            if (!values.empty()) {
                name = values[0];
                passwordCategories = values[1];
                password = values[2];
                service = values[3];
                login = values[4];
                oldPasswords.push_back(
                        *new Password(name, password, passwordCategories, service, login)
                );
                {
                    std::istringstream oldIss(passwordCategories);
                    std::string category;
                    while (std::getline(oldIss, category, ',')) {
                        if (!findCategory(categories, Encryptor::decrypt(category))) {
                            categories += category + ',';
                        }
                    }
                }
            } else {
                fmt::print("\nNo passwords!\n");
            }

        }


        inputFile.close();
        return oldPasswords;


    } else {
        fmt::print("\nFile doesn't exist!\n");
        return {};
    }
}

void Console::writePasswords(const std::string &filePassword,
                             const std::vector<Password> &newPasswords) const {
    std::ofstream outputFile(filePath);

    if (!outputFile) {
        fmt::print("\nFailed to open a file!\n");
        return;
    }

    outputFile << filePassword + "\n";
    for (const auto &data: newPasswords) {
        const std::string &name = data.get_name();
        const std::string &category = data.getCategories();
        const std::string &password = data.get_password();
        const std::string &service = data.get_service();
        const std::string &login = data.get_login();

        char splitter = (char) (0x0 + 685);
        std::string record;
        record += name;
        record += splitter;
        record += category;
        record += splitter;
        record += password;
        record += splitter;
        record += service;
        record += splitter;
        record += login;
        record += splitter;
        record += "\n";

        outputFile << record;

    }
    outputFile.close();

}

bool Console::findPassword(const std::string &nameToFind) {
    int index = 0;
    for (const auto &data: passwords) {
        if (Encryptor::encrypt(nameToFind) == data.get_name()) {
            fmt::print("\n{}.\n", index++);
            printPassword(data);
        }
    }
    if (index > 0)
        return true;
    return false;

}

bool Console::findUsedPassword(const std::string &password) {
    for (const auto &data: passwords) {
        if (Encryptor::encrypt(password) == data.get_password()) {
            return true;
        }
    }

    return false;

}


