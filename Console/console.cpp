#include "console.h"
#include <iostream>
#include <windows.h>
#include <ctime>
#include <filesystem>
#include <fmt/core.h>
#include <fmt/format.h>
#include "../Encryptor/encryptor.h"
#include <sstream>
#include <random>

Console::Console() {
    bool exit = false;
    SetConsoleOutputCP(CP_UTF8);
    fmt::print("\n--- Welcome to Password Manager! ---\n");

    do {

        fmt::print("\nSelect option:\n");
        fmt::print("1. Open database\n");
        fmt::print("2. Create database\n");
        fmt::print("3. Exit\n\n");
        std::string option;
        std::cin >> option;
        std::cin.ignore();
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
                fmt::print("\nWrong Input!\n");
                break;
        }
    } while (!exit);
}

bool Console::openDatabase() {
    fmt::print("\nInput source file path:\n");
    std::cin >> filePath;
    std::cin.ignore();
    std::string filePassword = getFilePassword();
    if (filePassword.empty()) {
        return false;
    }
    std::string password;
    fmt::print("\nEnter the file password:\n");
    std::cin >> password;
    std::cin.ignore();
    std::time_t currentTime = std::time(nullptr);
    std::tm *localTime = std::localtime(&currentTime);
    int year = localTime->tm_year + 1900;
    int month = localTime->tm_mon + 1;
    int day = localTime->tm_mday;

    int hour = localTime->tm_hour;
    int minute = localTime->tm_min;
    int second = localTime->tm_sec;

    std::ostringstream mdyStream;
    mdyStream << std::setfill('0') << std::setw(2) << month << "/"
              << std::setfill('0') << std::setw(2) << day << "/"
              << year;
    std::string mdy = mdyStream.str();

    std::ostringstream timeStream;
    timeStream << std::setfill('0') << std::setw(2) << hour << ":"
               << std::setfill('0') << std::setw(2) << minute << ":"
               << std::setfill('0') << std::setw(2) << second;
    std::string time = timeStream.str();
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
}

void Console::createDatabase() {
    fmt::print("\nInput file path to create:\n");
    std::cin >> filePath;
    std::cin.ignore();
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
        std::cin.ignore();
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

std::string Console::createPassword() {
    std::string password;
    while (true) {
        fmt::print("\nChoose an option:\n");
        fmt::print("1. Generate a password\n");
        fmt::print("2. Enter a password manually\n");
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        if (choice == 1) {
            password = generatePassword();
            fmt::print("\nGenerated password: {}\n", password);
        } else if (choice == 2) {
            fmt::print("\nEnter a new password:\n");
            std::getline(std::cin, password);
            fmt::print("\nYour password: {}\n", password);
        } else {
            fmt::print("\nInvalid choice. Please try again.\n");
            continue;
        }

        if (findUsedPassword(password)) {
            fmt::print("\nThis password is already used.\n");
            fmt::print("Would you like to try again? (1/2)\n");
            std::cin >> choice;
            std::cin.ignore();
            if (choice == 1) {
                continue;
            }
        }

        if (!isStrongPassword(password)) {
            fmt::print("\nPassword is weak!\n");
            fmt::print("Would you like to try again? (1/2)\n");
            std::cin >> choice;
            std::cin.ignore();
            if (choice == 1) {
                continue;
            }
        }
        break;

    }
    return password;
}

void Console::addPassword() {
    std::string name;
    std::string passwordCategories;
    std::string password;
    std::string service;
    std::string login;
    fmt::print("\nEnter new field name:\n");
    std::getline(std::cin, name);
    password = createPassword();
    do {
        fmt::print("\nEnter category name:\n");
        std::string category;
        std::getline(std::cin, category);
        if (findCategory(categories, category)) {
            passwordCategories += (category + ',');
            std::string option;
            fmt::print("\nAdd one more category? (1/2)\n");
            std::cin >> option;
            std::cin.ignore();
            if (option[0] == '2')
                break;
            continue;

        }
        fmt::print("\nCategory missing!\n");
        fmt::print("\nCreate new? (1/2)\n");
        std::string option1;
        std::cin >> option1;
        std::cin.ignore();
        if (option1[0] == '2') {
            fmt::print("\nPassword wasn't created!\n");
            return;
        }
        addCategory();
    } while (true);

    fmt::print("\nAdd Website/Service and Login? (1/2)\n");
    std::string option;
    std::getline(std::cin, option);
    if (option[0] == '1') {
        fmt::print("\nEnter service name:\n");
        std::getline(std::cin, service);
        fmt::print("\nEnter login:\n");
        std::getline(std::cin, login);

    }
    name = Encryptor::encrypt(name);
    password = Encryptor::encrypt(password);
    passwordCategories = Encryptor::encrypt(passwordCategories);
    service = Encryptor::encrypt(service);
    login = Encryptor::encrypt(login);
    passwords.push_back(
            *new Password(name, password, passwordCategories, service, login)
    );
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
    std::cin.ignore();
    fmt::print("Include uppercase letters? (1/2): ");
    std::string choice;
    std::cin >> choice;
    std::cin.ignore();
    includeUpperCase = (choice[0]);

    fmt::print("Include special symbols? (1/2): ");
    std::cin >> choice;
    std::cin.ignore();
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

void Console::editPassword() {

    fmt::print("\nEnter the index to edit:\n");
    std::string index;
    std::cin >> index;
    std::cin.ignore();
    int passwordIndex = index[0] - '0';

    if (passwordIndex >= 0 && passwordIndex < passwords.size()) {
        Password &editPassword = passwords[passwordIndex];

        std::string name;
        std::string passwordCategories;
        std::string password;
        std::string service;
        std::string login;

        while (true) {
            fmt::print("\nChoose the field to edit:\n");
            fmt::print("1. Name\n");
            fmt::print("2. Password Categories\n");
            fmt::print("3. Password\n");
            fmt::print("4. Service\n");
            fmt::print("5. Login\n");
            fmt::print("6. Finish Editing\n");

            std::string choice;
            std::cin >> choice;
            std::cin.ignore();
            if (choice == "1") {
                fmt::print("\nEnter new name:\n");
                std::getline(std::cin, name);
                name = Encryptor::encrypt(name);
                editPassword.set_name(name);
            } else if (choice[0] == '2') {
                fmt::print("\nEnter new password categories (separated by comma):\n");
                std::getline(std::cin, passwordCategories);
                passwordCategories = Encryptor::encrypt(passwordCategories);
                editPassword.setCategories(passwordCategories);

            } else if (choice[0] == '3') {
                password = createPassword();
                password = Encryptor::encrypt(password);
                editPassword.set_password(password);

            } else if (choice[0] == '4') {
                fmt::print("\nEnter new service:\n");
                std::getline(std::cin, service);
                service = Encryptor::encrypt(service);
                editPassword.set_service(service);

            } else if (choice[0] == '5') {
                fmt::print("\nEnter new login:\n");
                std::getline(std::cin, login);
                login = Encryptor::encrypt(login);
                editPassword.set_login(login);

            } else if (choice[0] == '6') {
                break;
            } else {
                fmt::print("\nInvalid choice. Please try again.\n");
            }
        }
        fmt::print("\nPassword data updated successfully.\n");
        return;
    } else {
        fmt::print("\nInvalid index. Please try again.\n");
    }

}

void Console::deletePassword() {

    std::vector<int> indicesToDelete;

    fmt::print("\nChoose Index (Enter 'end' when done):\n");
    std::string index;
    while (std::cin >> index) {
        std::cin.ignore();
        if (index == "end") {
            break;
        }
        int parsedIndex = std::stoi(index);
        indicesToDelete.push_back(parsedIndex);
    }

    std::sort(indicesToDelete.begin(), indicesToDelete.end(), std::greater<>());

    for (int position: indicesToDelete) {
        if (position >= 0 && position < passwords.size()) {
            passwords.erase(passwords.begin() + position);
        }
    }

    fmt::print("\nPassword(s) deleted successfully.\n");

}

void Console::searchPasswords() {
    fmt::print("Select search option:\n");
    fmt::print("1. Search by Name\n");
    fmt::print("2. Search by Category\n");
    fmt::print("3. Search by Login\n");
    fmt::print("4. Search by Service\n");

    std::string option;
    std::cin >> option;
    std::cin.ignore();
    std::string searchValue;
    fmt::print("Enter the search value:\n");
    std::cin >> searchValue;
    std::cin.ignore();
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
                std::string tempCategories = Encryptor::decrypt(password.getCategories());
                std::stringstream ss(tempCategories);
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

void Console::printPasswords() {
    int index = 0;
    for (const auto &data: passwords) {
        fmt::print("\n{}.\n", index++);
        printPassword(data);
    }
}

std::string Console::getFilePassword() const {
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

bool Console::findUsedPassword(const std::string &password) {
    for (const auto &data: passwords) {
        if (Encryptor::encrypt(password) == data.get_password()) {
            return true;
        }
    }

    return false;

}

void Console::sortPasswords() {
    fmt::print("Select sorting option:\n");
    fmt::print("1. Sort by Name\n");
    fmt::print("2. Sort by Category\n");
    fmt::print("3. Sort by Login\n");
    fmt::print("4. Sort by Service\n");

    std::string option;
    std::cin >> option;
    std::cin.ignore();
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


void Console::addCategory() {
    std::string newCategory;
    fmt::print("\nEnter a new category name\n");

    std::getline(std::cin, newCategory);
    if (!findCategory(categories, newCategory)) {
        categories.append(Encryptor::encrypt(newCategory + ','));
        return;

    }
    fmt::print("\nCategory already exists!\n");


}

void Console::deleteCategory() {
    std::string categoryToRemove;
    fmt::print("\nEnter category name\n");
    std::getline(std::cin, categoryToRemove);
    fmt::print("\n{}\n", Encryptor::decrypt(categories));
    if (findCategory(categories, categoryToRemove)) {
        if (passwords.empty()) {
            size_t pos = categories.find(categoryToRemove);
            if (pos != std::string::npos) {
                categories.erase(pos, categoryToRemove.length());
                fmt::print("\nSuccessfully removed!\n");

            }
            return;
        }
        bool categoryFoundInPasswords = false;
        for (const auto &data: passwords) {
            auto passwordCategories = data.getCategories();
            if (findCategory(passwordCategories, categoryToRemove)) {
                std::string option;
                fmt::print("\nSome passwords may contain this category\nConfirm? (1/2)\n");
                categoryFoundInPasswords = true;
                std::cin >> option;
                std::cin.ignore();
                if (option[0] == '2') {
                    return;
                }
                break;
            }

        }
        if (categoryFoundInPasswords) {
            for (int i = 0; i < passwords.size(); ++i) {
                auto passwordCategories = passwords[i].getCategories();
                if (findCategory(passwordCategories, categoryToRemove)) {
                    passwords.erase(passwords.begin() + i);
                    --i;
                }

            }
        }

        std::vector<std::string> categoryList = splitString(categories, ',');
        categories.clear();
        for (const auto &category: categoryList) {
            if (category != Encryptor::encrypt(categoryToRemove)) {
                categories += category;
                if (!categories.empty()) {
                    categories += ',';
                }
            }
        }
        fmt::print("\nSuccessfully removed!\n");
        return;
    } else {
        fmt::print("\nCategory not found!\n");

    }
}

std::vector<std::string> Console::splitString(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
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


        while (std::getline(inputFile, line)) {
            std::istringstream iss(line);
            std::string value;
            std::vector<std::string> values;
            while (std::getline(iss, value, splitter)) {
                values.push_back(value);
            }

            if (values.size() >= 5) {  // Check if values has enough elements
                std::string name = values[0];
                std::string passwordCategories = values[1];
                std::string password = values[2];
                std::string service = values[3];
                std::string login = values[4];
                oldPasswords.emplace_back(name, password, passwordCategories, service, login);

                std::istringstream oldIss(passwordCategories);
                std::string category;
                while (std::getline(oldIss, category, ',')) {
                    if (!findCategory(categories, Encryptor::decrypt(category))) {
                        categories += category + ',';
                    }
                }
            } else {
                fmt::print("\nDatabase is empty!\n");
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




