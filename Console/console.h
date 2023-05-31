#ifndef CONSOLE_H
#define CONSOLE_H

#include <fstream>

/**
 * Choose a source file
 * Enter the file password
 *
 * if the password is wrong write that attempt failed,
 * else: attempt success
 *
 * Choose options:
 * 		Search passwords by params
 * 		Sort password: by 2 params at least
 * 		Add password: with notification
 */
class Console {
  bool exit = false;
  std::string filePath;

 public:
  Console ();

 private:
  void createDatabase ();

  void openDatabase ();

  static bool isStrongPassword (const std::string& password);

  static bool is_empty (std::ifstream& pFile);

  static void printExtra (const std::string& error = " ");
  static void printConsole (

	  const std::string& title = " ",
	  const std::string& line1 = " ",
	  const std::string& line2 = " ",
	  const std::string& line3 = " ",
	  const std::string& line4 = " ",
	  const std::string& line5 = " "
  );
  static std::string printCentered (const std::string& text);

};

#endif // CONSOLE_H
