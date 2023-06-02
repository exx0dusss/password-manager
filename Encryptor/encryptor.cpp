#include <random>
#include "encryptor.h"
#include <iostream>


const std::string Encryptor::lowerCase{"abcdefghijklmnopqrstuvwxyz"};
const std::string Encryptor::upperCase{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
const std::string Encryptor::numerics{"0123456789"};
const std::string Encryptor::symbols{"~`!@#$%^&*()_-+={[}]|:;\"'<>.?/"};


const std::string Encryptor::code{
        Encryptor::lowerCase + Encryptor::upperCase + Encryptor::numerics + Encryptor::symbols};

const std::string Encryptor::key{"\"*)]Z/NL^`W+E(.3$;{BG9JHQDY-V!T6KFU#<"
                                 "OM1?|%=5PCA:SR7xz[nl@web~gjhq8d2y0'}vtk>4fuomp_&ciasr"};


bool Encryptor::charInRange(const char c) {
    if (c >= (char) (0x0 + 160) && c <= (char) (0x0 + 255))
        return true;
    return false;
}

std::string Encryptor::encrypt(const std::string &message) {
    std::string encryptedMessage = message;
    for (int i = 0; i < encryptedMessage.length(); ++i) {
        if (encryptedMessage[i + 1] == encryptedMessage[i]) {
            for (int j = i + 1; j < encryptedMessage.length(); ++j) {
                if (encryptedMessage[j] == encryptedMessage[i]) {
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_int_distribution<> dist(160, 255);
                    int randomNum = dist(gen);
                    encryptedMessage[j] = (char) (0x0 + randomNum);
                } else {
                    break;
                }
            }
        }

        size_t position = code.find(encryptedMessage[i]);
        if (position != std::string::npos) {
            encryptedMessage[i] = {key.at(position)};
        }
    }
    return encryptedMessage;
}

std::string Encryptor::decrypt(const std::string &message) {
    std::string decryptedMessage = message;
    for (int i = 0; i < decryptedMessage.length(); ++i) {
        if (charInRange(decryptedMessage[i + 1])) {
            for (int j = i + 1; j < decryptedMessage.length(); ++j) {
                if (charInRange(decryptedMessage[j])) {
                    decryptedMessage[j] = decryptedMessage[i];
                } else {
                    break;
                }
            }
        }

        size_t position = key.find(decryptedMessage[i]);
        if (position != std::string::npos) {
            decryptedMessage[i] = {code.at(position)};
        }

    }
    return decryptedMessage;
}

const std::string &Encryptor::getLowerCase() {
    return lowerCase;
}

const std::string &Encryptor::getUpperCase() {
    return upperCase;
}

const std::string &Encryptor::getNumerics() {
    return numerics;
}

const std::string &Encryptor::getSymbols() {
    return symbols;
}


