#pragma once
#include <SFML/Graphics.hpp>
#include "utils.hpp"
#include <string>

void bargraph(sf::Image& canvas, sf::RenderTexture& rt, int* values, std::string* labels, int sampleSize);