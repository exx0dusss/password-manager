#ifndef CONSOLE_H
#define CONSOLE_H

#include <fstream>
#include "../Password/password.h"
#include <set>

/**
 * Choose a source file
 * Enter the file password
 *
 * if the password is wrong write that attempt failed,
 * else: attempt success
 *
 * Choose options:
 * 		Search passwords by params
 * 		Sort password: by 2 params at least
 * 		Add password: with notification
 */
class Console {
public:
    Console();

    std::vector<Password> getPasswords();

    std::string filePath;
    std::string categories;
    std::vector<Password> passwords;

    void createDatabase();

    bool openDatabase();

    void printActions();

    static std::string createPassword();

    void addPassword();

    void editPassword();

    void deletePassword();

    void searchPasswords();

    void sortPasswords();

    void addCategory();

    void deleteCategory();

    void printPasswords();

    static bool findCategory(const std::string &sourceCategories,
                             const std::string &categoryToFind);

    bool findPassword(const std::string &passwordToFind);

    void
    writePasswords(const std::string &password, const std::vector<Password> &newPasswords) const;


    static bool isStrongPassword(const std::string &password);

    static bool is_empty(std::ifstream &pFile);

    std::string getFilePassword();

    static void printPassword(const Password &data);
};

#endif // CONSOLE_H
