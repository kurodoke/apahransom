#include "../include/common.h"
#include "../include/crypt.h"

CryptoPP::SecByteBlock contentBinParser(std::string filestream, bool isKeyFile = true) {
	std::ifstream stream(filestream);
	std::string buffer, raw;
	while (!stream.eof()) {
		std::getline(stream, buffer);
	}

	CryptoPP::StringSource s(buffer, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(raw)));
	CryptoPP::SecByteBlock block((const unsigned char*)(raw.data()), (isKeyFile ? CryptoPP::AES::DEFAULT_KEYLENGTH : CryptoPP::AES::BLOCKSIZE));
	stream.close();

	return block;
}

bool fileExist(const std::string& filename) {
	std::ifstream f(filename.c_str());
	if (f.good()) {
		f.close();
		return true;
	}
	f.close();
	return false;
}

void encrypt(std::filesystem::path pathname, bool isEncrypt) {
	std::string keyfilename = "key.bin";
	std::string ivfilename = "iv.bin";
	std::cout << "\tEncrypting";
	while (isEncrypt) {
		if (fileExist(keyfilename) && fileExist(ivfilename)) { 
			try {
				//get the content of key
				CryptoPP::SecByteBlock key = contentBinParser(keyfilename, true);

				//get the content of iv
				CryptoPP::SecByteBlock iv = contentBinParser(ivfilename, false);

				//stream target file 
				std::ifstream infile(pathname, std::ios::binary);

				std::filesystem::path outpath = pathname.string() + ".trelele";
				std::ofstream outputfile(outpath, std::ios::binary);

				CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption ncrypt;

				ncrypt.SetKeyWithIV(key, key.size(), iv);

				CryptoPP::FileSource fn(infile, true, new CryptoPP::StreamTransformationFilter(ncrypt, new CryptoPP::FileSink(outputfile)));

				infile.close();
				outputfile.close();
				std::filesystem::remove(pathname);
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

			////out stream key to key.bin
			std::string keyHex;
			CryptoPP::HexEncoder keyEncode;
			keyEncode.Put(key, sizeof(key));
			keyEncode.MessageEnd();

			if (CryptoPP::word32 size = keyEncode.MaxRetrievable()) {
				keyHex.resize(size);
				keyEncode.Get((CryptoPP::byte*)keyHex.data(), keyHex.size());
			}

			std::ofstream(keyfilename).write(keyHex.c_str(), sizeof(keyHex));

			////iv out stream
			std::string ivHex;
			CryptoPP::HexEncoder ivEncode;
			ivEncode.Put(iv, sizeof(iv));
			ivEncode.MessageEnd();

			if (CryptoPP::word32 size = ivEncode.MaxRetrievable()) {
				ivHex.resize(size);
				ivEncode.Get((CryptoPP::byte*)ivHex.data(), ivHex.size());
			}
			std::ofstream(ivfilename).write(ivHex.c_str(), sizeof(ivHex));

		}
	}

}

void decrypt(std::filesystem::path pathname, bool isDecrypt) {
	std::string keyfilename = "key.bin";
	std::string ivfilename = "iv.bin";
	std::cout << "\tDecrypting";
	while (isDecrypt) {
		if (fileExist(keyfilename) && fileExist(ivfilename)) {
			//get the content of key
			CryptoPP::SecByteBlock key = contentBinParser(keyfilename, true);

			//get the content of iv
			CryptoPP::SecByteBlock iv = contentBinParser(ivfilename, false);

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
			std::filesystem::remove(pathname);
			std::cout << "\tOK" << std::endl;
			isDecrypt = false;

		}
		else {
			std::cout << "no key";
			exit(EXIT_FAILURE);
		}
	}
}