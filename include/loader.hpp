#pragma once
#ifndef LOADER_HPP
#define LOADER_HPP

// Loads an LC-3 binary file into memory
int read_image(const char* image_path);

// Reads binary data from an open file into memory
void read_image_file(FILE* file);
#endif // LOADER_HPP
