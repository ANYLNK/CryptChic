#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>


namespace fs = std::filesystem;

void encrypt_folder(const fs::path& input_folder, const fs::path& output_folder, const uint8_t* key, const uint8_t* nonce);
void decrypt_folder(const fs::path& input_folder, const fs::path& output_folder, const uint8_t* key, const uint8_t* nonce);
fs::path GetUserProfilePath();
void CleanFolder(fs::path OriginalFilePath);