#pragma once
#include <SFML/Graphics.hpp>
#include "utils.hpp"
#include <string>

// Histogram (computed):
// samples[] is raw data. histogram() computes bin ranges + frequencies internally.
// binCount controls how many bars (bins) to draw.
void histogram(sf::Image& canvas, sf::RenderTexture& rt, int* samples, int sampleSize, int binCount);