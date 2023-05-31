#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>

class Encryptor {
private:
    static bool charInRange(char c);

    static const std::string lowerCase;
    static const std::string upperCase;
    static const std::string numerics;
    static const std::string symbols;
    static const std::string code;
    static const std::string key;
public:
    static std::string encrypt(const std::string &message);

    static std::string decrypt(const std::string &message);

    static const std::string &getLowerCase();

    static const std::string &getUpperCase();

    static const std::string &getNumerics();

    static const std::string &getSymbols();
};

#endif // ENCRYPTION_H
