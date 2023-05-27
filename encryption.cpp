#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>
#include <fmt/ranges.h>
#include <cstring>
#include <string>
using namespace std;

//  string alphabetSpecials{ "^39}:)1%&4$+?'{#75;2(!,6@|8>_*<." };
//  string keySpecials{ "123456789!@#$%^&*()_+{};:',.<>?|" };

int main ()
{
  string alphabet{ "] abcdefghijklmnopqrstuvwxyzABCDEFGHlJKLMNOPQRSTUVWXYZ" };
  char alphabetSpecial{ '%' };
  string spaceAlphabet{ "|_=#*!" };

  string key{ " ]ZNLWEBGJHQDYVTKFUOMPClASRxznlwebgjhqdyvtkfuompciasr" };
  char keySpecial{ '<' };

  string message;

  cout << "Enter your message:" << endl;
  getline (cin, message);
  for (int i = 0; i < message.length (); ++i)
	{
	  if (i + 1 < message.length () && message[i] == message[i + 1])
		{
		  message[i] = alphabetSpecial;
		}
	  else
		{
		  size_t position = alphabet.find (message[i]);
		  if (position != string::npos)
			{
			  message[i] = { key.at (position) };

			}
		}

	}
  cout << "Encrypted message: " << message << endl;

  for (int i = 0; i < message.length (); ++i)
	{
	  size_t position = key.find (message[i]);
	  if (position != string::npos)
		{
		  message[i] = { alphabet.at (position) };

		}
	  if (message[i - 1] == alphabetSpecial)
		{
		  message[i - 1] = message[i];
		}

	}
  cout << "Decrypted message: " << message << endl;

}