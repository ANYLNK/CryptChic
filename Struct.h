#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

constexpr int BLOCK_SIZE = 64;
constexpr int KEY_SIZE = 32;
constexpr int NONCE_SIZE = 12;


// ChaCha20 state
struct ChaCha20State {
    uint32_t state[16];
};