#include "console.h"
#include <iostream>
#include <windows.h>
#include <ctime>
#include <filesystem>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include "../Encryptor/encryptor.h"
#include <iomanip>
#include <sstream>

//std::cout << "\033[1A";  // Move the cursor up 1 line
Console::Console ()
{
  SetConsoleOutputCP (CP_UTF8);

  do
	{
	  printConsole (
		  "Welcome to Password Manager!",
		  "",
		  "1. Open database",
		  "2. Create database",
		  "3. Exit"
	  );
	  std::string option;
	  std::cin >> option;
//	  std::getline (std::cin, option);
//	  option.erase (option.find_last_not_of (" \t\r\n") + 1);
	  switch (option[0])
		{
	  case '1':
		openDatabase ();
		  break;
	  case '2':
		createDatabase ();
		  break;
	  case '3':
		exit = true;
		  break;
	  default:
		printExtra ("Wrong Input!");
		  break;
		}
	}

  while (!exit);
}

void Console::openDatabase ()
{
  printConsole ("", "", "Input source file path:");
  std::cin >> filePath;
  if (std::filesystem::exists (filePath))
	{
	  std::ifstream inputFile (filePath, std::ios::app);

	  if (!inputFile)
		{
		  printExtra ("Failed to open output file!");
		  exit = true;
		  return;
		}

	  std::string password;
	  printConsole ("", "", "Enter the file password:");
	  std::cin >> password;

	  std::string line;
	  std::getline (inputFile, line);
	  std::time_t currentTime = std::time (nullptr);
	  std::tm *localTime = std::localtime (&currentTime);
	  int year = localTime->tm_year + 1900;
	  int month = localTime->tm_mon + 1;
	  int day = localTime->tm_mday;

	  int hour = localTime->tm_hour;
	  int minute = localTime->tm_min;
	  int second = localTime->tm_sec;

	  std::string mdy = (std::to_string (month) + "/"
						 + std::to_string (day) + "/"
						 + std::to_string (year));
	  std::string time = (std::to_string (hour) + ":"
						  + std::to_string (minute) + ":"
						  + std::to_string (second)
	  );

	  std::ofstream logFile ("../data/logs.txt", std::ios::app);

	  if (Encryptor::decrypt (line) == password)
		{
		  printExtra ("Success!");
		  logFile << mdy << " " << time << " - Success! Path: " << filePath << std::endl;
		}
	  else
		{
		  printExtra ("Failed!");
		  logFile << mdy << " " << time << " - Failed! Path: " << filePath << std::endl;
		  return;
		}
	}
  else
	{
	  printExtra ("File doesn`t exist!");
	  return;
	};

}

void Console::createDatabase ()
{
  printConsole ("", "", "Input file path to create:");
  std::cin >> filePath;
  std::ifstream inputFile (filePath, std::ios::app);
  if (!is_empty (inputFile))
	{
	  printExtra ("The file may contain a database already! Override? 1/2");
	  int choice;
	  std::cin >> choice;
	  if (choice == 2)
		{
		  return;
		}
	}
  std::ofstream outputFile (filePath);
  if (!outputFile)
	{
	  printExtra ("Failed to create file!");
	  exit = true;
	  return;
	}
  std::string password;
  while (true)
	{
	  printConsole ("", "", "", "Create file password:");
	  std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');

	  std::cin >> password;
	  if (isStrongPassword (password))
		{
		  break;
		}
	  printConsole ("", "", "", "Password is weak! Input other? 1/2");
	  int choice;
	  std::cin >> choice;
	  if (choice == 2)
		{
		  break;
		}

	}

  outputFile << Encryptor::encrypt (password) << std::endl;
  printExtra ("Success! Remember to save this password!");

}

bool Console::is_empty (std::ifstream& pFile)
{
  return pFile.peek () == std::ifstream::traits_type::eof ();

}

bool Console::isStrongPassword (const std::string& password)
{
  int length = 0;
  bool hasUpperCase = false;
  bool hasLowerCase = false;
  bool hasSpecialChar = false;
  bool hasNumber = false;
  for (auto c : password)
	{
	  if (!hasUpperCase && Encryptor::getUpperCase ().find (c) != std::string::npos)
		{
		  hasUpperCase = true;
		}
	  if (!hasLowerCase && Encryptor::getLowerCase ().find (c) != std::string::npos)
		{
		  hasLowerCase = true;
		}
	  if (!hasSpecialChar && Encryptor::getSymbols ().find (c) != std::string::npos)
		{
		  hasSpecialChar = true;
		}
	  if (!hasNumber && Encryptor::getNumerics ().find (c) != std::string::npos)
		{
		  hasNumber = true;
		}
	  length++;
	}
  if (length >= 8 && hasUpperCase && hasLowerCase && hasSpecialChar && hasNumber)
	{
	  return true;
	}
  return false;
}

void
Console::printConsole (const std::string& title, const std::string& line1, const std::string& line2, const std::string& line3, const std::string& line4, const std::string& line5)
{
  std::cout << "\033[14A";
  std::cout << "\r";
  std::cout << "╔══════════════════════════════════════╗" << std::endl;
  std::cout << "║" << printCentered (title) << "║" << std::endl;
  std::cout << "║══════════════════════════════════════║" << std::endl;
  std::cout << "║  Choose option:                      ║" << std::endl;
  std::cout << "║" << printCentered (line1) << "║" << std::endl;
  std::cout << "║" << printCentered (line2) << "║" << std::endl;
  std::cout << "║" << printCentered (line3) << "║" << std::endl;
  std::cout << "║" << printCentered (line4) << "║" << std::endl;
  std::cout << "║" << printCentered (line5) << "║" << std::endl;
  std::cout << "║                                      ║" << std::endl;
  std::cout << "║══════════════════════════════════════║" << std::endl;
  std::cout << "║ :                                    ║" << std::endl;
  std::cout << "╚══════════════════════════════════════╝";
  std::cout << "\033[1A";
  std::cout << "\r";
  std::cout << "\033[4C";
  std::cout << "                                   ";
  std::cout << "\r";
  std::cout << "\033[4C";
}

std::string Console::printCentered (const std::string& text)
{
  std::ostringstream oss;
  int padding = 38 - text.length ();
  int leftPadding = padding / 2;
  int rightPadding = padding - leftPadding;

  oss << std::setw (leftPadding + text.length ()) << std::setfill (' ') << text
	  << std::setw (rightPadding) << std::setfill (' ') << "";

  return oss.str ();
}
void Console::printExtra (const std::string& error)
{
  std::cout << "\033[3B";
  std::cout << "\r";
  std::cout << error;
  std::cout << "\033[3A";
  std::cout << "\r";
}

