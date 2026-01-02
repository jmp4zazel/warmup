#pragma once
#include <vector>

char calculate( const std::string_view str );
std::string sha256 ( const std::string &str );
std::string xorEncryptDecrypt( std::string& password, std::string& secretKey );
