#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>
#include <fmt/ranges.h>
#include <cstring>
#include <string>
#include <random>

using namespace std;

bool charInRange (char c, int min, int max);

class Encryptor {
  
};
int main ()
{
  const string lowerCase{ "abcdefghijklmnopqrstuvwxyz" };
  const string upperCase{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
  const string numerics{ "0123456789" };
  const string symbols{ "~`!@#$%^&*()_-+={[}]|\\:;\"'<,>.?/" };
  const string code{ lowerCase + upperCase + numerics + symbols };

  const string key{ "\"*)]Z/NL^`\\W+E(.3$;{B,G9JHQDY-V!T6KFU#<"
					"OM1?|%=5PCA:SR7xz[nl@web~gjhq8d2y0'}vtk>4fuomp_&ciasr" };
  string message;
  cout << "Enter your message:" << endl;
  getline (cin, message);
  for (int i = 0; i < message.length (); ++i)
	{
	  if (message[i + 1] == message[i])
		{
		  for (int j = i + 1; j < message.length (); ++j)
			{
			  if (message[j] == message[i])
				{
				  std::random_device rd;
				  std::mt19937 gen (rd ());
				  std::uniform_int_distribution<> dist (160, 255);
				  int randomNum = dist (gen);
				  message[j] = (char)(0x0 + randomNum);
				}
			  else
				{
				  break;
				}
			}
		}

	  size_t position = code.find (message[i]);
	  if (position != string::npos)
		{
		  message[i] = { key.at (position) };
		}

	}
  cout << "Encrypted message: " << message << endl;

  for (int i = 0; i < message.length (); ++i)
	{

	  if (charInRange (message[i + 1], 160, 255))
		{
		  for (int j = i + 1; j < message.length (); ++j)
			{
			  if (charInRange (message[j], 160, 255))
				{
				  message[j] = message[i];
				}
			  else
				{
				  break;
				}
			}
		}

	  size_t position = key.find (message[i]);
	  if (position != string::npos)
		{
		  message[i] = { code.at (position) };
		}
	}
  cout << "Decrypted message: " << message << endl;

}

bool charInRange (const char c, int min, int max)
{
  if (c >= (char)(0x0 + min) && c <= (char)(0x0 + max))
	return true;

  return false;
}
