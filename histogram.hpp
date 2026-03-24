#pragma once
#include <SFML/Graphics.hpp>
#include "utils.hpp"
#include <string>

void histogram(sf::Image& canvas, sf::RenderTexture& rt, int* samples, int sampleSize, int binCount);