#pragma once
#include "stb_image.h"
#include <iostream>
#include "pgr.h"


/**
 * @brief Loads a texture from the specified file path.
 *
 * This function loads a texture from the given file path and returns the OpenGL ID of the texture.
 *
 * @param path The file path of the texture.
 * @param gamma Flag indicating whether gamma correction should be applied to the texture.
 * @return The OpenGL ID of the loaded texture.
 */
unsigned int TextureFromFile(const char* path, bool gamma = false);