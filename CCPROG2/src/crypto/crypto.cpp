#include "../../include/sha.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

std::string sha256 ( const std::string &str )
{
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init ( &sha256 );
  SHA256_Update ( &sha256, str.c_str (), str.size () );
  SHA256_Final ( hash, &sha256 );
  std::stringstream ss;

  for ( int i = 0; i < SHA256_DIGEST_LENGTH; i++ )
  {
    ss << std::hex << std::setw ( 2 ) << std::setfill ( '0' ) << ( int )hash[i];
  }
  return ss.str ();
}

std::string xorEncryptDecrypt ( std::string &password, std::string &secretKey )

{
  for ( size_t i = 0, j = 0; i < password.size (); i++, j++ )
  {
    if ( j >= secretKey.size () )
    {
      j = 0;
    }

    password[i] = password[i] ^ secretKey[j]; // Use key[j] to XOR
                                              //
  }

  return password;
}
