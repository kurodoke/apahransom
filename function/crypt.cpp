#include "../include/common.h"
#include "../include/crypt.h"

bool fileExist(const std::string& filename) {
	{
		std::ifstream f(filename.c_str());
		if (f.good()) {
			f.close();
			return true;
		}
		f.close();
		return false;
	}
}

void encrypt(std::filesystem::path pathname, bool isEncrypt) {
	std::string keyfilename = "key.bin";
	std::string ivfilename = "iv.bin";
	std::cout << "\tEncrypting";
	while (isEncrypt) {
		if (fileExist(keyfilename) && fileExist(ivfilename)) { 
			try {
				//stream key file that exist
				std::ifstream keyStream(keyfilename);

				std::string keybuffer, rawKey;
				while (!keyStream.eof()) {
					std::getline(keyStream, keybuffer);
				}

				CryptoPP::StringSource sKey(keybuffer, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(rawKey)));
				CryptoPP::SecByteBlock key((const unsigned char*)(rawKey.data()), CryptoPP::AES::DEFAULT_KEYLENGTH);
				keyStream.close();
				//std::string keybuffer;
				//std::getline(keyStream, keybuffer);
				//CryptoPP::SecByteBlock key((const unsigned char*)(keybuffer.data()), keybuffer.size());
				//keyStream.close();
				//hexdecode

				//stream iv file that exist
				std::ifstream ivStream(ivfilename);

				std::string ivbuffer, rawIv;
				while (!ivStream.eof()) {
					std::getline(ivStream, ivbuffer);
				}

				CryptoPP::StringSource siv(ivbuffer, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(rawIv)));
				CryptoPP::SecByteBlock iv((const unsigned char*)(rawIv.data()), CryptoPP::AES::BLOCKSIZE);
				ivStream.close();
				//std::ifstream ivStream(ivfilename);
				//std::string ivbuffer;
				//std::getline(ivStream, ivbuffer);
				//CryptoPP::SecByteBlock iv((const unsigned char*)(ivbuffer.data()), ivbuffer.size());
				//ivStream.close();

				//std::cout << "key to enc :" << std::endl;
				//std::cout << rawKey << std::endl;
				//std::cout << "iv to enc :" << std::endl;
				//std::cout << rawIv << std::endl << std::endl;

				//stream target file 
				std::ifstream infile(pathname, std::ios::binary);

				std::ofstream outputfile(pathname += ".trelele", std::ios::binary);

				CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption ncrypt;

				ncrypt.SetKeyWithIV(key, key.size(), iv);

				CryptoPP::FileSource fn(infile, true, new CryptoPP::StreamTransformationFilter(ncrypt, new CryptoPP::FileSink(outputfile)));

				infile.close();
				outputfile.close();
				std::cout << "\tOK" << std::endl;
				isEncrypt = false;

			}
			catch (const std::exception& e) {
				std::cerr << e.what() << std::endl;
				exit(1);
			}
		}
		else {
			CryptoPP::AutoSeededRandomPool rng;

			//alloccate  block
			CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
			CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE];

			//generate random block
			rng.GenerateBlock(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
			rng.GenerateBlock(iv, CryptoPP::AES::BLOCKSIZE);

			//out stream key to key.bin
			std::string keyHex;
			CryptoPP::HexEncoder keyEncode;
			keyEncode.Put(key, sizeof(key));
			keyEncode.MessageEnd();

			if (CryptoPP::word32 size = keyEncode.MaxRetrievable()) {
				keyHex.resize(size);
				keyEncode.Get((CryptoPP::byte*)keyHex.data(), keyHex.size());
			}

			std::ofstream(keyfilename).write(keyHex.c_str(), sizeof(keyHex));

			//iv out stream
			std::string ivHex;
			CryptoPP::HexEncoder ivEncode;
			ivEncode.Put(iv, sizeof(iv));
			ivEncode.MessageEnd();

			if (CryptoPP::word32 size = ivEncode.MaxRetrievable()) {
				ivHex.resize(size);
				ivEncode.Get((CryptoPP::byte*)ivHex.data(), ivHex.size());
			}
			std::ofstream(ivfilename).write(ivHex.c_str(), sizeof(ivHex));

			//std::cout << "key to file :" << std::endl;
			//std::cout << key << std::endl;
			//std::cout << "iv to file :" << std::endl;
			//std::cout << iv << std::endl << std::endl;
		}
	}

}

void decrypt(std::filesystem::path pathname, bool isDecrypt) {
	std::string keyfilename = "key.bin";
	std::string ivfilename = "iv.bin";
	std::cout << "\tDecrypting";
	while (isDecrypt) {
		if (fileExist(keyfilename) && fileExist(ivfilename)) {
			//stream key file that exist
			std::ifstream keyStream(keyfilename);

			std::string keybuffer, rawKey;
			while (!keyStream.eof()) {
				std::getline(keyStream, keybuffer);
			}

			CryptoPP::StringSource sKey(keybuffer, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(rawKey)));
			CryptoPP::SecByteBlock key((const unsigned char*)(rawKey.data()), CryptoPP::AES::DEFAULT_KEYLENGTH);
			keyStream.close();
			//std::ifstream keyStream(keyfilename);
			//std::string keybuffer;
			//std::getline(keyStream, keybuffer);
			//CryptoPP::SecByteBlock key((const unsigned char*)(keybuffer.data()), keybuffer.size());
			//keyStream.close();

			//stream iv file that exist
			std::ifstream ivStream(ivfilename);

			std::string ivbuffer, rawIv;
			while (!ivStream.eof()) {
				std::getline(ivStream, ivbuffer);
			}

			CryptoPP::StringSource sIv(ivbuffer, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(rawIv)));
			CryptoPP::SecByteBlock iv((const unsigned char*)(rawIv.data()), CryptoPP::AES::BLOCKSIZE);
			ivStream.close();
			//std::ifstream ivStream(ivfilename);
			//std::string ivbuffer;
			//std::getline(ivStream, ivbuffer);
			//CryptoPP::SecByteBlock iv((const unsigned char*)(ivbuffer.data()), ivbuffer.size());
			//ivStream.close();

			//std::cout << "key to dec :" << std::endl;
			//std::cout << rawKey << std::endl;
			//std::cout << "iv to dec :" << std::endl;
			//std::cout << rawIv << std::endl << std::endl;

			//stream target file 
			std::ifstream infile(pathname, std::ios::binary);

			//trim string path
			std::string s = pathname.string();
			s.erase(s.find_last_not_of(".trelele") + 1);
			std::filesystem::path pathTrim = s;

			std::ofstream outputfile(pathTrim, std::ios::binary);

			CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption dcrypt;
			dcrypt.SetKeyWithIV(key, key.size(), iv);

			CryptoPP::FileSource fd(infile, true, new CryptoPP::StreamTransformationFilter(dcrypt, new CryptoPP::FileSink(outputfile)));

			infile.close();
			outputfile.close();
			std::cout << "\tOK" << std::endl;
			isDecrypt = false;

		}
		else {
			std::cout << "no key";
			exit(EXIT_FAILURE);
		}
	}
}