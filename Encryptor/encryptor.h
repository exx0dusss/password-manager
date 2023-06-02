#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>

class Encryptor {
private:
    /**
     * @brief Checks if character is in decimal range [160, 255]
     * @param c Character that is checked
     * @return True if is contained in range.
     */
    static bool charInRange(char c);

    /**
     * @defgroup const ASCII-Characters
     * @brief Four string constants used for generating data.
     * @{
     */
    static const std::string lowerCase; ///< Lowercase characters.
    static const std::string upperCase; ///< Uppercase characters.
    static const std::string numerics;  ///< Numeric digits.
    static const std::string symbols;   ///< Special symbols.
    /** @} */

    /**
     * @defgroup Cipher constants
     * @brief Used in encrypting/decrypting of data
     * @{
     */
    static const std::string code; ///< Code for Encrypting.
    static const std::string key; ///< Key for Decrypting.
    /** @} */

public:
    /**
     * @brief A method for data encryption.
     * Uses a key to find a character to substitude.
     * If there are more than 1 same characters in a row
     * encrypts them as chars in range [160, 255]
     * @param message Data to encrypt.
     * @return std::string Encrypted data.
     */
    static std::string encrypt(const std::string &message);


    /**
  * @brief A method for data decryption.
  * Uses code constant to find a character to substitude.
  * If next characters in a sequence are a in range [160, 255]
  * decrypts them as a current character
  * @param message Data to decrypt.
  * @return std::string Decrypted data.
  */
    static std::string decrypt(const std::string &message);


    /**
     * @defgroup const ASCII-Characters return methods
     * @brief Four methods used for returning a specific sequence of characters
     * @{
     */
    static const std::string &getLowerCase(); ///<Returns a string of lowercase characters.

    static const std::string &getUpperCase(); ///<Returns a string of uppercase characters.

    static const std::string &getNumerics(); ///<Returns a string of digits.

    static const std::string &getSymbols(); ///<Returns a string of symbols.
    /** @} */
};

#endif // ENCRYPTION_H
