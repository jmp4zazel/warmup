#pragma once

#include <string>

bool checkUser ( std::string &user );
int createUser ( std::string &user, std::string &password );
bool authenticateUser ( std::string &user, std::string &password );
int changePassword ( bool authenticated, std::string &user, std::string &newPassword );
int addCredentials ( bool authenticationStatus, std::string &name, std::string &secretKey,
                     std::string &siteName, std::string &sitePassword );

int viewCredentials ( bool authenticationStatus, std::string &name, std::string &secretKey,
                      std::string &siteName );
int deleteCredentials ( bool authenticationStatus, std::string &name, std::string &siteName );
int changeSiteCreds ( bool authenticationStatus, std::string &user, std::string &secretKey,
                      std::string &sitename, std::string &newSitePassword );
