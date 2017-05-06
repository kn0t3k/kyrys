#pragma once
#include <reference.hpp>


namespace Kyrys {
	/**
	 * @brief	AesContainer encapsulates AES 128bit cipher from mbedTLS library into C++ class and simplify using of the code
	 * @param[in]	m_version	Holds version of AES cipher which you want to use. Default version is AES with 128bits long key
	 * @warning					A lot of conversions inside this class is from unsigned to signed char which means, that this class accept only
	 * 							ASCII chars with value from 0 to 127 ! more info here: https://en.wikipedia.org/wiki/ASCII
	 *
	 */
	class AesContainer {

	private:
		unsigned int m_version;				//version of AES, but also length of encrypting block in Bytes, length of key in bits, ...
		const std::string& m_data;			//input plain text

		unsigned long m_input_length;		//Length of m_data
		unsigned long m_output_length;		//Length of m_block_output
		unsigned int m_paddingLength;		//Length of padding block at the end of last block of data

		//mbedTLS variables
		unsigned char m_key[16];			//key used for encryption/decryption - 128bit long
		mbedtls_aes_context m_aes;			//AES context
		unsigned char m_iv[16];  			//Initialization Vector - vector of really random numbers
		unsigned char m_block_input[128];	//block of data from m_data
		unsigned char m_block_output[128];	//block of encrypted data created by encrypting m_block_input


	public:
		//Constructors
		AesContainer(unsigned char* key, const std::string& plain_input, unsigned int version = 128);

		//Getters
		unsigned int getVersion() const;

		const string &getData() const;

		unsigned long getInput_length() const;

		unsigned long getOutput_length() const;

		unsigned int getPaddingLength() const;

		const unsigned char *getKey() const;

		const mbedtls_aes_context &getAES() const;

		const unsigned char *getIV() const;

		const unsigned char *getBlock_input() const;

		const unsigned char *getBlock_output() const;

		bool isValidKey() const; //controls if key is valid according to version of AES Cipher


		//Setters
		void clear();
		void initializeIV();


		//Other methods
		std::string encrypt();
		std::string decrypt();


	private:
		int prepareBlock(unsigned int iterator); //copy i-th block of data pointed by iterator into m_block_input


	};
}
