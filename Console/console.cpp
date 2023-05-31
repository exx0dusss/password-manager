#include "console.h"
#include <iostream>
#include <windows.h>
#include <ctime>
#include <filesystem>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include "../Encryptor/encryptor.h"

Console::Console ()
{
  SetConsoleOutputCP (CP_UTF8);

  do
	{
	  std::cout << "╔══════════════════════════════════╗" << std::endl;
	  std::cout << "║ Welcome to the Password Manager! ║" << std::endl;
	  std::cout << "║══════════════════════════════════║" << std::endl;
	  std::cout << "║                                  ║" << std::endl;
	  std::cout << "║       Choose option:             ║" << std::endl;
	  std::cout << "║       1. Open database           ║" << std::endl;
	  std::cout << "║       2. Create database         ║" << std::endl;
	  std::cout << "║       3. Exit                    ║" << std::endl;
	  std::cout << "║                                  ║" << std::endl;
	  std::cout << "╚══════════════════════════════════╝\n" << std::endl;
	  if (std::cin.fail ())
		{
		  std::cin.clear ();
		  std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');
		  continue;
		}
	  std::string option;
	  std::cin >> option;
	  option.erase (option.find_last_not_of (" \t\r\n") + 1);

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
		fmt::print ("Wrong input!\n");
		  break;
		}
	}
  while (!exit);
}

void Console::createDatabase ()
{
  fmt::print ("Input file path to create:\n");
  std::cin >> filePath;
  std::ifstream inputFile (filePath, std::ios::app);
  if (!is_empty (inputFile))
	{
	  fmt::print ("The file may contain a database already!\n");
	  fmt::print ("Override? 1/2\n");
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
	  std::cerr << "Failed to create file!\n";
	  exit = true;
	  return;
	}
  std::string password;
  while (true)
	{
	  fmt::print ("Create file password:\n");
	  std::cin >> password;
	  if (isStrongPassword (password))
		{
		  break;
		}
	  fmt::print ("Password is weak!\n");
	  fmt::print ("Input other? 1/2\n");
	  int choice;
	  std::cin >> choice;
	  if (choice == 2)
		{
		  break;
		}

	}

  outputFile << Encryptor::encrypt (password) << std::endl;
  fmt::print ("Success! Remember to save this password!\n");
  std::cin.clear ();
  std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');

}

void Console::openDatabase ()
{
  fmt::print ("Input source file path:\n");
  std::cin >> filePath;
  if (std::filesystem::exists (filePath))
	{
	  std::ifstream inputFile (filePath, std::ios::app);

	  if (!inputFile)
		{
		  std::cerr << "Failed to open output file!" << std::endl;
		  exit = true;
		  return;
		}

	  std::string password;
	  fmt::print ("Enter the file password:\n");
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
		  fmt::print ("Success!\n");
		  logFile << mdy << " " << time << " - Success! Path: " << filePath << std::endl;
		}
	  else
		{
		  fmt::print ("Failed!\n");
		  logFile << mdy << " " << time << " - Failed! Path: " << filePath << std::endl;
		}
	}
  else
	{
	  fmt::print ("File doesn`t exist!\n");
	  return;
	}

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
