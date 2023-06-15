#ifndef CONSOLE_H
#define CONSOLE_H

#include <fstream>
#include "../Password/password.h"
#include <set>


class Console {
public:
    Console();

private:
    char splitter = (char) (0x0 + 685); ///< character that splits data written to a file.

    std::string filePath; ///< path of a file containing database.
    std::string categories; ///< encrypted string of categories.
    std::vector<Password> passwords; ///< vector of encrypted passwords.

    /**
      * @brief Method for opening database.\n
      * 1. Gets the source file with database.\n
      * 2. Gets the source file password.\n
      * 3. Saves the log state to logs.txt file.\n
      * @return True if database exists and the password is correct.
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
      * @return std::string password.
      */
    std::string createPassword();

    /**
     * @brief Method for adding password data.\n
     * 1. Gets a field name.\n
     * 2. Gets category name.\n
     * 3. Gets password.\n
     * 4. Gets Website and Login by option.\n
     * 5. Encrypts all data and writes to a vector of passwords.\n
     */
    void addPassword();

    /**
     * @brief Method for generating password.\n
     * 1. Gets a field name.\n
     * 2. Gets category name.\n
     * 3. Gets password.\n
     * 4. Gets Website and Login by option.\n
     * 5. Encrypts all data and writes to a vector of passwords.\n
     * @return std::string generated password.
     */
    static std::string generatePassword();

    /**
      * @brief Method for editing password data.\n
      * Edits and encrypts a field of password data.\n
      */
    void editPassword();

    /**
      * @brief Method for deleting password.\n
      * Deletes password from vector.\n
      */
    void deletePassword();

    /**
      * @brief Method for searching password.\n
      * Prints password by input parameters.\n
      * Accepts name, category, login and service.\n
      */
    void searchPasswords();

    /**
     * @brief Method for printing all password from vector.\n
     * Prints enumerated passwords.\n
     */
    void printPasswords();

    /**
     * @brief Method for getting database password.\n
     * @return std::String encrypted password.\n
     */
    std::string getFilePassword() const;

    /**
     * @brief Checks if password already exists.\n
     * @return True if password is already used.\n
     */
    bool findUsedPassword(const std::string &passwordToFind);

    /**
     * @brief Writes encrypted passwords to a database.\n
     * @param password\n Encrypted database password.\n
     * @param newPasswords\n Vector of encrypted passwords.\n
     */
    void
    writePasswords(const std::string &password, const std::vector<Password> &newPasswords) const;

    /**
     * @brief Prints sorted passwords.\n
     * Accepts name, category, login and service.
     */
    void sortPasswords();

    /**
     * @brief Adds a new category.
     */
    void addCategory();

    /**
     * @brief Deletes category.
     */
    void deleteCategory();

    /**
     * @brief Checks if category exists.
     * @param sourceCategories\n String of categories to search in
     * @param categoryToFind\n Category to search.
     * @return True if found.
     */
    static bool findCategory(const std::string &sourceCategories,
                             const std::string &categoryToFind);

    /**
     * @brief Splits string into vector of strings.\n
     * @param str\n String to parse.
     * @param delimiter\n Delimiter to split by.
     * @return Vector of parsed strings.
     */
    static std::vector<std::string> splitString(const std::string &str, char delimiter);

    /**
     * @brief Checks if password is strong.
     * @param password
     * @return True if password has upperCase, numbers and symbols.
     */
    static bool isStrongPassword(const std::string &password);

    /**
     * @brief Checks if file is empty.
     * @param pFile\n Pointer to a file.
     * @return True if empty.
     */
    static bool is_empty(std::ifstream &pFile);

    /**
       * @brief Method for printing a specific password from vector.\n
       */
    static void printPassword(const Password &data);

    /**
     * @brief Gets encrypted passwords from file.\n
     * Splits them by parameters and pushes to a vector.\n
     * Adds categories to a string.
     * @return Vector of encrypted passwords.
     */
    std::vector<Password> getPasswords();

};

#endif // CONSOLE_H
