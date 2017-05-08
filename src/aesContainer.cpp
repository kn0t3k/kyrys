//Our headers
#include <reference.hpp>
#include <aesContainer.hpp>

//STD C++ Library
#include <cstring>

using Kyrys::AesContainer;


//Constructors
AesContainer::AesContainer(unsigned char *key, unsigned char *iv, const std::string &plain_input, unsigned int version) : m_version(version), m_data(plain_input){
	//copying of key inside container
	memcpy(m_key, key, 16); //type safe copying

	//copying of initializer vector
	memcpy(m_iv, iv, 16); //type safe copying

	//Initializing of aes context by m_Key
	if(mbedtls_aes_setkey_enc( &m_aes, m_key, m_version) == MBEDTLS_ERR_AES_INVALID_KEY_LENGTH){
		std::cout << "AesContainer::Parametric_Constructor: key has wrong length" << std::endl;
	}

	//Initializing of iv
	setIV(iv);

	m_input_length = plain_input.length();

	m_paddingLength = m_version - (unsigned int) (m_input_length % m_version); //repair this
	if(m_paddingLength != 0)
		m_output_length = m_input_length - (m_input_length % m_version) + m_version;
}


//Getters
unsigned int AesContainer::getVersion() const { return m_version; }
const string &AesContainer::getData() const { return m_data; }
unsigned long AesContainer::getInput_length() const { return m_input_length; }
unsigned long AesContainer::getOutput_length() const { return m_output_length; }
unsigned int AesContainer::getPaddingLength() const { return m_paddingLength; }
const unsigned char *AesContainer::getKey() const { return m_key; }
const mbedtls_aes_context &AesContainer::getAES() const { return m_aes; }
const unsigned char *AesContainer::getIV() const { return m_iv; }
const unsigned char *AesContainer::getBlock_input() const { return m_block_input; }
const unsigned char *AesContainer::getBlock_output() const { return m_block_output; }

bool AesContainer::isValidKey() const {
	//todo
	return true;
}


//Setters
void AesContainer::clear(){
	//todo: deside what should be default values of atributes
	m_version = 128;
}


//Other methods
//todo: change this for true random generator, iv must be generated by true random generator
void AesContainer::setIV(unsigned char *iv) {
	char defaultVector[16] = {35, 34, 100, 35, 81, 125, 77, 40, 51, 55, 42, 71, 111, 70, 77, 101}; //Every char in iv must be lesser than 128 !
	if(iv == nullptr){
		strncpy((char*)m_iv, defaultVector, 16);
	} else
		strncpy((char*)m_iv, (char*)iv, 16);
}


//Other methods
std::string AesContainer::encrypt() {

	std::string buffer;
	std::string outputBuffer;

	for(unsigned int i = 0; i < m_output_length / (m_version / 8); ++i){
		prepareBlock(i);
		if(mbedtls_aes_crypt_cbc( &m_aes, MBEDTLS_AES_ENCRYPT, 128, m_iv, m_block_input, m_block_output ) == MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH){
			std::cout << "AesContainer::encrypt::mbettls_aes_crypt_cbc: error after calling mbedTLS ecnryption function" << std::endl;
		}
		outputBuffer = (char*) m_block_output; //be careful: conversion signed -> unsigned char !
		buffer.append(outputBuffer);
	}
	return buffer;
}

std::string AesContainer::decrypt() {

	std::string buffer;
	std::string outputBuffer;

	for(unsigned int i = 0; i < m_output_length / (m_version / 8); ++i){
		prepareBlock(i);
		if(mbedtls_aes_crypt_cbc( &m_aes, MBEDTLS_AES_DECRYPT, 128, m_iv, m_block_input, m_block_output ) == MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH){
			std::cout << "AesContainer::decrypt::mbettls_aes_crypt_cbc: error after calling mbedTLS ecnryption function" << std::endl;
		}
		outputBuffer = (char*) m_block_output; //be careful: conversion signed -> unsigned char !
		buffer.append(outputBuffer);
	}
	return buffer;
}

int AesContainer::prepareBlock(unsigned int iterator){
	std::string buffer = m_data.substr(iterator*m_version, iterator*m_version + m_version);
	std::strncpy((char*)m_block_output, buffer.c_str(), 128); //be careful: conversion signed -> unsigned char !
	//std::copy(buffer.begin(), buffer.end(), m_block_output);
	return 0;
}

void AesContainer::addPadding(const std::string& paddingType){
	if(paddingType == "PKCS#7"){
		addPKCS7Padding();
	}
	else if(paddingType == "ZEROS"){
		addZerosPadding();
	}
}

void AesContainer::addPKCS7Padding(){

}

void AesContainer::addZerosPadding(){

}


//Service methods for testing
void AesContainer::printArray(const unsigned char* array, unsigned int length, const std::string& mode) const {
	if(mode == "char"){
		std::cout << "| ";
		for(unsigned int i = 0; i < length; ++i){
			if(array[i] != 0 && array[i] != '\0')
				std::cout << array[i] << " | ";
			else
				std::cout << "\0 | ";
		}
	}
	else if(mode == "int") {
		std::cout << "| ";
		for(unsigned int i = 0; i < length; ++i){
			if(array[i] != 0 && array[i] != '\0')
				std::cout << (unsigned int) array[i] << " | ";
			else
				std::cout << "\0 | ";
		}
	}
}

/*
//pre-generated random key
unsigned char key[32] = { 'o', 'a', 'b', 's', 'w', 'o', 'e', 'd', 'v', 'h', 'q', 'm', 'z', 'g', 'a', 'u','y','q','g','l','5','`','1','Z','q','H','7','F','f','b','n',' '};
unsigned char iv[16] = { 14, 31, 60, 126, 81, 12, 36, 102, 57, 9, 42, 51, 111, 4, 3, 25 };
char defaultVector[16] = {1, 5, 100, 127, 81, 12, 77, 13, 7, 5, 42, 71, 111, 4, 11, 25};
*/