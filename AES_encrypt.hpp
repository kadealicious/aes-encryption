#ifndef AES_encrypt_HPP
#define AES_encrypt_HPP

#include <array>
#include <vector>

class AES {
public:
    AES(const std::vector<uint8_t>& key); // Constructor to initialize the AES object with a key
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext);
    std::vector<uint8_t> EncryptECB(const std::vector<uint8_t>& data);
private:
    static constexpr int Nb = 4; // Number of columns (32-bit words) comprising the State. For AES, Nb is always 4.
    int Nk; // Number of 32-bit words comprising the Cipher Key.
    int Nr; // Number of rounds, which is a function of Nk and Nb (which is fixed).
    std::array<uint8_t, 176> expandedKey; // Expanded key

    void keyExpansion(const std::vector<uint8_t>& key);
    void addRoundKey(std::array<uint8_t, 16>& state, int round);
    void subBytes(std::array<uint8_t, 16>& state);
    void shiftRows(std::array<uint8_t, 16>& state);
    void mixColumns(std::array<uint8_t, 16>& state);
    void invSubBytes(std::array<uint8_t, 16>& state);
    void invShiftRows(std::array<uint8_t, 16>& state);
    void invMixColumns(std::array<uint8_t, 16>& state);
    uint8_t gmul(uint8_t a, uint8_t b);  

    static const std::array<uint8_t, 256> sbox;
    static const std::array<uint8_t, 256> invSbox;
    static const std::array<uint8_t, 11> rcon;
};

#endif // AES_HPP

