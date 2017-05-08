#pragma once
#include <reference.hpp>


namespace Kyrys {
	/**
	 * @brief					AesContainer encapsulates AES 128bit cipher from mbedTLS library into C++ class and simplify using of the code
	 *
	 * @param[in]	m_version	Holds version of AES cipher which you want to use. Default version is AES with 128bits long key
	 *
	 * @warning					A lot of conversions inside this class is from unsigned to signed char which means, that this class accept only
	 * 							ASCII chars with value from 0 to 127 ! more info here: http://en.cppreference.com/w/cpp/language/ascii
	 */
	class AesContainer {

	private:
		unsigned int m_version;				//version of AES, but also length of encrypting block in Bytes, length of key in bits, ...
		const std::string& m_data;			//input text - can be plain but also encrypted data depending if you are encrypting or decrypting
		//std::string m_encrypted_data;

		unsigned long m_input_length;		//Length of m_data
		unsigned long m_output_length;		//Length of m_block_output
		unsigned int m_paddingLength;		//Length of padding block at the end of last block of data

		//mbedTLS variables
		unsigned char m_key[16];			//key used for encryption/decryption - 128bit long
		mbedtls_aes_context m_aes;			//AES context
		unsigned char m_iv[16];  			//Initialization Vector - vector of really random numbers, should be created by mbedTLS generator of random numbers in next version
		unsigned char m_block_input[128];	//128B long block of data from m_data, not ended by binary zero '\0'
		unsigned char m_block_output[128];	//128B long block of encrypted data created by encrypting m_block_input


	public:
		//Constructors
		AesContainer(unsigned char *key, unsigned char *iv, const std::string &plain_input, unsigned int version = 128);

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

		/**
		 * @brief			setIV sets m_iv atribute
		 *
		 * @param iv[in]	You can use own initializer vector consisting of random chars with values
		 * 					lesser than 128(ASCII chars) or set this param to nullptr and then will be
		 * 					used default vector inside method.
		 *
		 * @warning			Don't set value of any part of vector to zero or \0, strcpy could interpret
		 * 					this value as end of array and then it will be cut in this place and rest of
		 * 					string will be not copyied into m_iv atribute.
		 */
		void setIV(unsigned char *iv = nullptr); //done

		//Other methods
		std::string encrypt();

		std::string decrypt();

		//Service methods for testing
		void printArray(const unsigned char* array, unsigned int length, const std::string& mode) const;

		//Padding methods
		/**
		 * @brief					addPadding method adds padding at the end of m_data depepending on paddingType
		 *
		 * @param paddingType[in]	Supports 2 pading types, PKCS#7 and padding by zeros at the end of m_data
		 */
		void addPadding(const std::string& paddingType);

		/**
		 * @brief					addPadding method adds padding at the end of m_data depepending on paddingType
		 *
		 * @param paddingType[in]	Supports 2 pading types, PKCS#7 and padding by zeros at the end of m_data
		 */
		void removePadding(const std::string &paddingType, string &decryptedOutput);

	private:
		int prepareBlock(unsigned int iterator); //copy i-th block of data pointed by iterator into m_block_input


		//Padding methods
		void addPKCS7Padding(char paddingSymbol);

		void addZerosPadding();

		void removePKCS7Padding(string &decryptedOutput);

		void removeZerosPadding();

	};
}
