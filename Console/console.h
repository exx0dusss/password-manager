#ifndef CONSOLE_H
#define CONSOLE_H

#include <fstream>
#include "../Password/password.h"
#include <set>


class Console {
public:
    Console();

private:
    char splitter = (char) (0x0 + 685); ///< character that splits written data to a file.

    std::string filePath; ///< path of a file containing database.
    std::string categories; ///< encrypted string of categories.
    std::vector<Password> passwords; ///< vector of encrypted passwords.

    /**
     * @brief Method for opening database.\n
     * 1. Gets the source file with database.\n
     * 2. Gets the source file password.\n
     * 3. Saves the log state to logs.txt file.\n
     * @return Returns true if database exists and the password is correct.
     */
    bool openDatabase();

    /**
     * @brief Method for creating database.\n
     * 1. Gets the database file path.\n
     * 2. Gets a password for database.\n
     * 3. Encrypts and writes password to a file.\n
     */
    void createDatabase();

    /**
     * @brief Method for printing actions if database is opened.\n
     */
    void printActions();

    /**
     * @brief Method for creating a new password.\n
     * 1. Gives option whether to generate or enter password.\n
     * 2. Checks if password is already used.\n
     * 3. Checks if password is weak (doesn't contain uppercase characters, symbols or numbers).\n
     * @return Returns std::string password.
     */
    std::string createPassword();

    void addPassword();

    static std::string generatePassword();

    void editPassword();

    void deletePassword();

    void searchPasswords();

    void printPasswords();

    std::string getFilePassword() const;

    bool findUsedPassword(const std::string &passwordToFind);

    void
    writePasswords(const std::string &password, const std::vector<Password> &newPasswords) const;

    void sortPasswords();

    void addCategory();

    void deleteCategory();


    static bool findCategory(const std::string &sourceCategories,
                             const std::string &categoryToFind);

    static std::vector<std::string> splitString(const std::string &str, char delimiter);

    static bool isStrongPassword(const std::string &password);

    static bool is_empty(std::ifstream &pFile);

    static void printPassword(const Password &data);

    std::vector<Password> getPasswords();

};

#endif // CONSOLE_H
