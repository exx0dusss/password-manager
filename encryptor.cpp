#include <iostream>
#include <string>
#include <random>

using namespace std;

bool charInRange (char c, int min, int max);

struct Encryptor {
  const string lowerCase{ "abcdefghijklmnopqrstuvwxyz" };
  const string upperCase{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
  const string numerics{ "0123456789" };
  const string symbols{ "~`!@#$%^&*()_-+={[}]|\\:;\"'<,>.?/" };
  const string code{ lowerCase + upperCase + numerics + symbols };
  const string key{ "\"*)]Z/NL^`\\W+E(.3$;{B,G9JHQDY-V!T6KFU#<"
					"OM1?|%=5PCA:SR7xz[nl@web~gjhq8d2y0'}vtk>4fuomp_&ciasr" };
  std::string encrypt (const std::string message)
  {
	std::string encryptedMessage;
	for (int i = 0; i < message.length (); ++i)
	  {
		size_t position = code.find (message[i]);
		if (position != string::npos)
		  {
			encryptedMessage += { key.at (position) };
		  }
		else
		  {
			encryptedMessage += message[i];
		  }

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
					encryptedMessage += (char)(0x0 + randomNum);
				  }
				else
				  {
					break;
				  }
			  }
		  }

	  }
	return encryptedMessage;
  }

  std::string decrypt (const std::string message)
  {
	std::string decryptedMessage;

	for (int i = 0; i < message.length (); ++i)
	  {
		if (charInRange (message[i + 1], 160, 255))
		  {
			for (int j = i + 1; j < message.length (); ++j)
			  {
				if (charInRange (message[j], 160, 255))
				  {
					decryptedMessage += message[i];
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
			decryptedMessage += { code.at (position) };
		  }
		else
		  {
			decryptedMessage += message[i];
		  }
	  }
	return decryptedMessage;
  }

 private:
  bool charInRange (const char c, int min, int max)
  {
	if (c >= (char)(0x0 + min) && c <= (char)(0x0 + max))
	  return true;

	return false;
  }

};
int main ()
{

  string message;
  cout << "Enter your message:" << endl;
  getline (cin, message);
  auto encryptor = new Encryptor ();

  std::string encryptedMessage = encryptor->encrypt (message);
  cout << "Encrypted message: " << encryptedMessage << endl;

  std::string decryptedMessage = encryptor->decrypt (encryptedMessage);
  cout << "Decrypted message: " << decryptedMessage << endl;

}


