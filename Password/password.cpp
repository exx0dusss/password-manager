
#include "password.h"

Password::Password(std::string name,
                   std::string password,
                   std::string categories) :
        name(std::move(name)),
        password(std::move(password)),
        categories(categories) {}

Password::Password(std::string name,
                   std::string password,
                   std::string categories,
                   std::string service,
                   std::string login) :
        name(std::move(name)),
        password(std::move(password)),
        categories(categories),
        service(std::move(service)),
        login(std::move(login)) {}

const std::string &Password::get_name() const {
    return name;
}

void Password::set_name(const std::string &newName) {
    Password::name = newName;
}

const std::string &Password::get_password() const {
    return password;
}

void Password::set_password(const std::string &newPassword) {
    Password::password = newPassword;
}


const std::string &Password::get_service() const {
    return service;
}

void Password::set_service(const std::string &newService) {
    Password::service = newService;
}

const std::string &Password::get_login() const {
    return login;
}

void Password::set_login(const std::string &newLogin) {
    Password::login = newLogin;
}

const std::string &Password::getCategories() const {
    return categories;
}

void Password::setCategories(const std::string &newCategories) {
    Password::categories = newCategories;
}

