#include "AES_encrypt.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    std::string filePath;
    std::cout << "Please enter the file path: ";
    std::getline(std::cin, filePath);

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << '\n';
        return 1;
    }

    std::vector<unsigned char> fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    std::vector<unsigned char> key(16, 'a');  // Ensure key is correct size for AES-128
    AES aes(key);  // Correct constructor usage

    try {
        std::vector<unsigned char> encryptedData = aes.EncryptECB(fileContents);  // Correct method usage
        fs::path p(filePath);
        std::string newFileName = "encrypted_" + p.stem().string() + ".txt";
        std::string newFilePath = p.parent_path().string() + "/" + newFileName;

        std::ofstream outFile(newFilePath, std::ios::out | std::ios::binary);
        if (!outFile.is_open()) {
            std::cerr << "Failed to create output file: " << newFilePath << '\n';
            return 1;
        }
        outFile.write(reinterpret_cast<const char*>(encryptedData.data()), encryptedData.size());
        outFile.close();

        std::cout << "Encryption successful. Output file created: " << newFileName << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Encryption failed: " << e.what() << '\n';
        return 1;
    }

    return 0;
}

