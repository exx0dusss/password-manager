#ifndef PASSWORD_MANAGER_PASSWORD_H
#define PASSWORD_MANAGER_PASSWORD_H

#include <string>
#include <utility>
#include <vector>
#include<set>

class Password {
private:
    std::string name; ///< name of the password data
    std::string password; ///< name of the password
    std::string categories; ///< list of categories for password
    std::string service; ///< service of password data
    std::string login; ///< login of password data
public:
    Password(std::string name,
             std::string password,
             std::string categories,
             std::string service,
             std::string login);

    [[nodiscard]] const std::string &get_name() const; ///< name getter

    void set_name(const std::string &newName); ///< name setter

    [[nodiscard]] const std::string &get_password() const; ///< password getter

    void set_password(const std::string &newPassword); ///< password setter

    [[nodiscard]] const std::string &getCategories() const; ///< categories getter

    void setCategories(const std::string &newCategories); ///< categories setter

    [[nodiscard]] const std::string &get_service() const; ///< service getter

    void set_service(const std::string &newService); ///< service setter

    [[nodiscard]] const std::string &get_login() const; ///< login getter

    void set_login(const std::string &newLogin); ///< setter
};

#endif //PASSWORD_MANAGER_PASSWORD_H
