#include "aes.hpp"
#include <cstring>
#include<stdexcept>
#include<iostream>

using namespace std;

AES::AES(const vector<uint8_t>& key) {
    if (key.size() != 16) {
		cerr << "Invalid key size for AES-128!  Key size must be 128 bits." << endl;
		return;
	}
	
	Nk = 4;			// There are 4 32-bit words in AES-128 keys.
	Nr = 10;		// AES-128 always has 10 rounds!
    ExpandKey(key);	// The key is expanded to 176 bytes, up from 128 bits.
}

void AES::ExpandKey(const vector<uint8_t>& key) {
    const size_t keyWords = key.size() / 4;			// keyWords for AES-128 is always 4.
    const size_t expandedKeySize = Nb * (Nr + 1); 	// Expanded key size is 176 bytes for AES-128.

    // Copy the original key to the first 128 bits of the expanded key.
    for (size_t i = 0; i < keyWords; ++i) {
        expandedKey[4 * i + 0] = key[4 * i + 0];
        expandedKey[4 * i + 1] = key[4 * i + 1];
        expandedKey[4 * i + 2] = key[4 * i + 2];
        expandedKey[4 * i + 3] = key[4 * i + 3];
    }

	// Generate the next 4 bits of the expanded key until we can't no mo.
    for (size_t i = keyWords; i < expandedKeySize; ++i) {
	
		// Temp will be used to hold the current 4 bytes of the expanded key we are working on.
    	uint32_t temp = (expandedKey[4 * (i - 1) + 0] << 24) |
               (expandedKey[4 * (i - 1) + 1] << 16) |
               (expandedKey[4 * (i - 1) + 2] << 8) |
               (expandedKey[4 * (i - 1) + 3]);

        if (i % keyWords == 0) {
			// Rotate left by 8 bits.
            temp = (temp << 8) | (temp >> 24);
			
			// S-Box substitution formula; this scares me: 
            temp = (sbox[(temp >> 24) & 0xFF] << 24) |
                   (sbox[(temp >> 16) & 0xFF] << 16) |
                   (sbox[(temp >> 8) & 0xFF] << 8) |
                   (sbox[temp & 0xFF]);

            // XOR with round constant.
            temp ^= (uint32_t(rcon[i / keyWords]) << 24);
        }

        // XOR with the word Nk positions before the current word.
        temp ^= (expandedKey[4 * (i - keyWords) + 0] << 24) |
                (expandedKey[4 * (i - keyWords) + 1] << 16) |
                (expandedKey[4 * (i - keyWords) + 2] << 8) |
                (expandedKey[4 * (i - keyWords) + 3]);

		// Assign the values in temp to the expanded key.
        expandedKey[4 * i + 0] = temp >> 24;
        expandedKey[4 * i + 1] = (temp >> 16) & 0xFF;
        expandedKey[4 * i + 2] = (temp >> 8) & 0xFF;
        expandedKey[4 * i + 3] = temp & 0xFF;
    }
}

/** Round key generation is performed at the start of each round.  It is done by
	modifying the expanded key positionally based on the round index. */
void AES::AddRoundKey(array<uint8_t, 16>& block, int round) {

	// Start index into the expanded key to impact the round key.
    size_t start = round * Nb * 4;
	
	// Round keys are XOR'd into the expanded key, starting at the index of start.
    for (int i = 0; i < 16; ++i) {
        block[i] ^= expandedKey[start + i];
    }
}

/// S-Box substitution method.
void AES::SubstituteBytes(array<uint8_t, 16>& block) {
    for (int i = 0; i < 16; ++i) {
        block[i] = sbox[block[i]];
    }
}

/// Essentially a rotation of the current encryption block.
void AES::ShiftRows(array<uint8_t, 16>& block) {

    array<uint8_t, 16> temp = block;
    
	// Do not shift Row 0!

    // Row 1 shifts left by 1: 
    block[1] = temp[5];
    block[5] = temp[9];
    block[9] = temp[13];
    block[13] = temp[1];

    // Row 2 shifts left by 2: 
    block[2] = temp[10];
    block[6] = temp[14];
    block[10] = temp[2];
    block[14] = temp[6];

    // Row 3 shifts left by 3: 
    block[3] = temp[15];
    block[7] = temp[3];
    block[11] = temp[7];
    block[15] = temp[11];
}

/// Inverse S-Box substitution method.
void AES::InverseSubstituteBytes(array<uint8_t, 16>& block) {
    for (int i = 0; i < 16; ++i) {
        block[i] = inverse_sbox[block[i]];
    }
}

/// Shift the rows back to where they came from!
void AES::InverseShiftRows(array<uint8_t, 16>& block) {

    array<uint8_t, 16> temp = block;
    
	// Do not shift Row 0!

    // Row 1 shifts right by 1: 
    block[5] = temp[1];
    block[9] = temp[5];
    block[13] = temp[9];
    block[1] = temp[13];

    // Row 2 shifts right by 2: 
    block[10] = temp[2];
    block[14] = temp[6];
    block[2] = temp[10];
    block[6] = temp[14];

    // Row 3 shifts right by 3: 
    block[15] = temp[3];
    block[3] = temp[7];
    block[7] = temp[11];
    block[11] = temp[15];
}

/// Encrypt data using AES-128!
vector<uint8_t> AES::Encrypt(const vector<uint8_t>& plaintext) {
	
    // Create a space-padded vector for our encrypted data to live!
    size_t dataSize = plaintext.size();
	dataSize += 16 - (dataSize % 16);
    vector<uint8_t> encryptedData(dataSize, (uint8_t)' ');

	// Temporary block for use during encryption.
    array<uint8_t, 16> block;

    for (size_t i = 0; i < dataSize; i += 16) {
        // Copy the current block to our encrypted data block.
        copy(plaintext.begin() + i, plaintext.begin() + i + 16, block.begin());

		/* For each round, perform S-Box substitution, shift rows of our 
			encrypted data, and add a new round key.  Per professor request, 
			we have omitted MixColumns(), as the math is crazy for that and 
			this is not a math class.  Thank you Prof. Vickers!  :) */
        for (int round = 1; round < Nr; round++) {
            SubstituteBytes(block);
            ShiftRows(block);
            AddRoundKey(block, round);
        }

        // Copy the temporary block into the encrypted data block.
        copy(block.begin(), block.end(), encryptedData.begin() + i);
    }
	
	cout << endl << "Your original data is: " << endl;
	for(size_t i = 0; i < plaintext.size(); i++) {
		cout << plaintext[i];
	}
	cout << endl << endl;
	
	cout << "Your encrypted data is: " << endl;
	for(size_t i = 0; i < encryptedData.size(); i++) {
		cout << encryptedData[i];
	}
	cout << endl << endl;

    return encryptedData;
}

/// Encrypt data using AES-128!
vector<uint8_t> AES::Decrypt(const vector<uint8_t>& ciphertext) {
	
    // Create a vector for the decrypted data to live!
    size_t dataSize = ciphertext.size();
    vector<uint8_t> decryptedData(dataSize);

	// Temporary block for use during decryption.
    array<uint8_t, 16> block;

    for (size_t i = 0; i < dataSize; i += 16) {
        // Copy the current block to our decrypted data block.
        copy(ciphertext.begin() + i, ciphertext.begin() + i + 16, block.begin());

		/* Perform the opposite of encryption here xD.  Again note that the 
			omission of MixColumns() means we can condense our entire decryption
			pass into a single for loop! */
        for (int round = Nr - 1; round > 0; round--) {
			AddRoundKey(block, round);
			InverseShiftRows(block);
            InverseSubstituteBytes(block);
        }

        // Copy the temporary block into the decrypted data block.
        copy(block.begin(), block.end(), decryptedData.begin() + i);
    }
	
	cout << endl << "Your original data is: " << endl;
	for(size_t i = 0; i < ciphertext.size(); i++) {
		cout << ciphertext[i];
	}
	cout << endl << endl;
	
	cout << "Your decrypted data is: " << endl;
	for(size_t i = 0; i < decryptedData.size(); i++) {
		cout << decryptedData[i];
	}
	cout << endl << endl;

    return decryptedData;
}