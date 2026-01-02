#include "../../include/aes.hpp"
#include "../../include/common.hpp"
#include "../../include/crypto.hpp"
#include "../../include/json.hpp"
#include <filesystem>

using ordered_json = nlohmann::ordered_json;
using json = nlohmann::json;
namespace fs = std::filesystem;

bool ensureDirectoryExistence ( fs::path directoryPath )
{
  if ( fs::exists ( directoryPath ) )
  {
    // If the directory already exist, it'll return true
    return true;
  }

  // If it doesn't exist, it'll create a directory and still return true
  return fs::create_directories ( directoryPath );
}

/*
bool checkFileExistence(fs::path directoryPath, std::string fileName) {

  std::cout << "called checkFileExistence()" << '\n';
  std::cout << "Passed directoryPath in function: " << directoryPath << '\n';
  std::cout << "Passed fileName in function: " << fileName << '\n';

  for (auto &entry : fs::directory_iterator(directoryPath)) {

    if (entry.is_regular_file() && entry.path().stem() == fileName &&
        entry.path().extension() == ".json")

    {

      // TRUE, the file already exist
      std::cout << "Credential file exist." << '\n';
      return true;
    }
  }

  // FALSE, the file doesn't exist
  std::cout << "Credential file doesn't exist" << '\n';
  return false;
}
*/

bool checkFileExistence ( fs::path fullFilePath )
{
  if ( fs::exists ( fullFilePath ) )
  {
    return true;
  }

  return false;
}

int addCredentials ( bool authenticationStatus, std::string &name, std::string &secretKey,
                     std::string &siteName, std::string &sitePassword )
{
  if ( !authenticationStatus )
  {
    std::cout << "Error, you're not authenticated.\n";
    return 1;
  }

  std::cerr << "called addCredntials()";

  fs::path directoryPath{ "db" };

  ensureDirectoryExistence ( directoryPath );

  std::string encryptedPassword{ aesEncrypt ( sitePassword, secretKey ) };
  std::string fileName{ name + "_" + siteName + ".json" };
  fs::path credentialsOutputPath{ directoryPath / fileName };

  if ( checkFileExistence ( credentialsOutputPath ) == true )
  {
    std::cout << "Credential file with the sitename: " << siteName
              << " already exist please choose a different sitename\n";
    return 1;
  }

  std::ofstream credentialsOutputFile ( credentialsOutputPath );

  if ( !credentialsOutputFile.is_open () )
  {
    std::cout << "Error can't create credentials. \n";
    return 1;
  }

  ordered_json userInfo{

      { "siteName", siteName },
      { "sitePassword", encryptedPassword },

  };

  credentialsOutputFile << userInfo.dump ( 4 );

  return 0;
}

int changeSiteCreds ( bool authenticationStatus, std::string &name, std::string &secretKey,
                      std::string &siteName, std::string &newSitePassword )
{
  if ( !authenticationStatus )
  {
    std::cout << "Error, you're not authenticated.\n";
    return 1;
  }

  std::string sitePassword{ aesEncrypt ( newSitePassword, secretKey ) };
  std::string fileName{ name + "_" + siteName + ".json" };
  fs::path directoryPath{ "db" };
  fs::path filePath{ directoryPath / fileName };

  if ( !fs::exists ( filePath ) )
  {
    std::cout << "Credential doesn't exist.";
    return 1;
  }

  // Delete old file
  std::filesystem::remove ( filePath );

  // Create new file
  std::ofstream newCredentiaFile ( filePath );

  ordered_json userInfo{

      { "siteName", siteName },
      { "sitePassword", sitePassword },

  };

  std::cout << "Successfully changed: " << siteName << " password\n";

  newCredentiaFile << userInfo.dump ( 4 );

  return 0;
}

int deleteCredentials ( bool authenticationStatus, std::string &name, std::string &siteName )
{
  if ( authenticationStatus == false )
  {
    std::cerr << "Error, you're not authenticated.\n";
    return 1;
  }

  fs::path directoryPath{ "db" };

  ensureDirectoryExistence ( directoryPath );

  std::string fileName{ name + "_" + siteName + ".json" };
  fs::path credentialsFullPath{ directoryPath / fileName };

  if ( checkFileExistence ( credentialsFullPath ) == true )
  {
    if ( fs::remove ( credentialsFullPath ) )
    {
      std::cout << "Deleted credentials: " << credentialsFullPath << '\n';
      return 0;
    }
  }

  std::cout << "Credentials doesnt exist." << '\n';
  return 1;
}

int viewCredentials ( bool authenticationStatus, std::string &name, std::string &secretKey,
                      std::string &siteName )
{
  if ( authenticationStatus == false )
  {
    std::cout << "Error, you're not authenticated.\n";
    return 1;
  }

  fs::path directoryPath{ "db" };

  ensureDirectoryExistence ( directoryPath );

  std::string fileName{ name + "_" + siteName + ".json" };
  fs::path credentialsFullPath{ directoryPath / fileName };

  if ( checkFileExistence ( credentialsFullPath ) == true )
  {
    std::ifstream credentialFile ( credentialsFullPath );

    if ( !credentialFile.is_open () )
    {
      std::cout << "Error can't view credentials. \n";
      return 1;
    }

    json jsonData = json::parse ( credentialFile );

    std::string passwordHolder{ jsonData.at ( "sitePassword" ) };
    std::string decryptedPassword{ aesDecrypt ( passwordHolder, secretKey ) };
    std::cout << "Password: " << decryptedPassword << '\n';
    return 0;
  }

  std::cout << "Credentials doesn't exist";
  return 1;
}
