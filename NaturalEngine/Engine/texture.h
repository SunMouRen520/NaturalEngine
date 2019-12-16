#pragma once
#include <stb_image.h>
#include <iostream>
#include <string>
#include <vector>
#include <glad/glad.h>

using namespace std;

unsigned int TextureFromFile(const char* path, const string &directory, bool gamma = false);
unsigned int LoadCubmap(vector<std::string>faces);
unsigned int GenerateTexture2D(int w, int h);
unsigned int GenerateTexture3D(int w, int h, int d);
void BindTexture2D(unsigned int tex, int unit = 0);
