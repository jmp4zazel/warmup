#include "../../include/common.hpp"
#include "../../include/crypto.hpp"
#include "../../include/json.hpp"

using ordered_json = nlohmann::ordered_json;
using json = nlohmann::json;
namespace fs = std::filesystem;

bool checkUser ( std::string &user )
{
  fs::path directoryPath{ "users" };
  fs::path userFile{ directoryPath / ( user + ".json" ) };
  std::string target{ user };

  if ( fs::create_directories ( directoryPath ) )
  {
    std::cout << "Created user DB: " << directoryPath << '\n';
  }

  for ( auto &entry : fs::directory_iterator ( directoryPath ) )
  {
    if ( entry.is_regular_file () && entry.path ().stem () == target &&
         entry.path ().extension () == ".json" )

    {
      return true;
    }
  }

  return false;
}

bool authenticateUser ( std::string &passedUser, std::string &passedPassword )
{
  // Decrypt target file via username

  // If successfully decrypted, return true (authenticated = true) and program can now perform
  // listing passwords, and viewing passwords If not, exit

  // Decrypt the target user's password in json file, and store it in a string

  // Concatenate target user with .json
  std::string fileName{ passedUser + ".json" };
  fs::path directoryPath{ "users" };
  fs::path fileFullPath{ directoryPath / fileName };

  std::ifstream userFile ( fileFullPath );

  json jsonData = json::parse ( userFile );

  // Parse the password element value
  std::string password{ jsonData.at ( "password" ) };

  if ( sha256 ( passedPassword ) == password )
  {
    // Authenticated
    return true;
  }

  else
  {
    // Authentication failure, meaning that the original hashed password didn't matched to the user
    // passed password
    return false;
  }
}

int createUser ( std::string &user, std::string &password )
{
  if ( checkUser ( user ) )
  {
    std::cout << "User already exists, please choose a different username\n";
    return 1;
  }

  std::string userPassword{ sha256 ( password ) };
  std::string fileName{ user + ".json" };
  fs::path directoryPath{ "users" };
  fs::path fileOutputPath{ directoryPath / fileName };

  std::ofstream outputFile ( fileOutputPath );

  ordered_json userInfo{

      { "user", user },
      { "password", userPassword },

  };

  std::cout << "User: " << user << " created\n";

  outputFile << userInfo.dump ( 4 );

  return 0;
}

int changePassword ( bool authenticationStatus, std::string &user, std::string &newPassword )
{
  if ( !authenticationStatus )
  {
    std::cout << "Error, you're not authenticated.\n";
    return 1;
  }

  std::string newUserPassword{ sha256 ( newPassword ) };
  std::string fileName{ user + ".json" };
  fs::path directoryPath{ "users" };
  fs::path filePath{ directoryPath / fileName };

  // Delete old file
  std::filesystem::remove ( filePath );

  // Create new file
  std::ofstream newCredentiaFile ( filePath );

  ordered_json userInfo{

      { "user", user },
      { "password", newUserPassword },

  };

  std::cout << "Successfully changed: " << user << " password\n";

  newCredentiaFile << userInfo.dump ( 4 );

  return 0;
}
