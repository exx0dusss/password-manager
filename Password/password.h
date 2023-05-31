#ifndef PASSWORD_MANAGER_PASSWORD_H
#define PASSWORD_MANAGER_PASSWORD_H

#include <string>
#include <utility>
#include <vector>

class Password {
private:
    std::string name;
    std::string password;
    std::vector<std::string> categories;
    std::string service;
    std::string login;
public:
    Password(std::string name,
             std::string password,
             const std::vector<std::string> &categories);

    Password(std::string name,
             std::string password,
             const std::vector<std::string> &categories,
             std::string service,
             std::string login);
};

#endif //PASSWORD_MANAGER_PASSWORD_H
