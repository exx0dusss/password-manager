#ifndef PASSWORD_MANAGER_PASSWORD_H
#define PASSWORD_MANAGER_PASSWORD_H

#include <string>
#include <utility>
#include <vector>
#include<set>

class Password {
private:
    std::string name;
    std::string password;
    std::string categories;
    std::string service;
    std::string login;
public:
    Password(std::string name,
             std::string password,
             std::string categories);

    Password(std::string name,
             std::string password,
             std::string categories,
             std::string service,
             std::string login);

    const std::string &get_name() const;

    void set_name(const std::string &newName);

    const std::string &get_password() const;

    void set_password(const std::string &newPassword);

    const std::string &getCategories() const;

    void setCategories(const std::string &newCategories);

    const std::string &get_service() const;

    void set_service(const std::string &newService);

    const std::string &get_login() const;

    void set_login(const std::string &newLogin);
};

#endif //PASSWORD_MANAGER_PASSWORD_H
