
#include "password.h"

Password::Password(std::string name,
                   std::string password,
                   const std::vector<std::string> &categories) :
        name(std::move(name)),
        password(std::move(password)),
        categories(categories) {}

Password::Password(std::string name,
                   std::string password,
                   const std::vector<std::string> &categories,
                   std::string service,
                   std::string login) :
        name(std::move(name)),
        password(std::move(password)),
        categories(categories),
        service(std::move(service)),
        login(std::move(login)) {}
