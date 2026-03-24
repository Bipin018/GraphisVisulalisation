#include "histogram.hpp"
#include <algorithm>
#include <cstdio>
#include <vector>

static int max_value(const std::vector<int>& values){
    int m = values[0];
    for(size_t i = 1; i < values.size(); i++){
        if(values[i] > m) m = values[i];
    }
    return m;
}

static int clamp_int(int v, int lo, int hi){
    if(v < lo) return lo;
    if(v > hi) return hi;
    return v;
}

void histogram(sf::Image& canvas, sf::RenderTexture& rt, int* samples, int sampleSize, int binCount){
    // Histogram plot area
    int left   = 80;
    int right  = 760;
    int top    = 50;
    int bottom = 550;

    // axes
    draw_line(canvas, left, top, left, bottom, sf::Color::Black);
    draw_line(canvas, left, bottom, right, bottom, sf::Color::Black);

    if(sampleSize <= 0) return;
    if(binCount <= 0) return;

    // min and max values of sample
    int minV = samples[0];
    int maxV = samples[0];
    for(int i = 1; i < sampleSize; i++){
        if(samples[i] < minV) minV = samples[i];
        if(samples[i] > maxV) maxV = samples[i];
    }

    // If all values in sample are same
    if(minV == maxV){
        maxV = minV + 1;
    }

   //limits max no of bins
    binCount = clamp_int(binCount, 1, 50);

    int range = maxV - minV;                
    int binWidth = (range + binCount - 1) / binCount;
    if(binWidth <= 0) binWidth = 1;

  
    int coveredMax = minV + binWidth * binCount;

    // Count how many samples fall into each bin
    std::vector<int> counts(binCount, 0);
    for(int i = 0; i < sampleSize; i++){
        int v = samples[i];
        int idx = (v - minV) / binWidth;
        if(idx < 0) idx = 0;
        if(idx >= binCount) idx = binCount - 1; 
        counts[idx]++;
    }

    int largestY = max_value(counts);
    if(largestY <= 0) largestY = 1;

    // scaling for Y axis based on largest Y
    int tensY = 1;
    float tempY = (float)largestY;
    while(tempY > 1){
        tempY /= 10;
        tensY *= 10;
    }
    tensY= largestY;
    // bar width in pixels 
    int plotW = right - left;
    int barW = plotW / binCount;
    if(barW < 3) barW = 3;

    sf::Text text(font);
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(11);

  
    for(int i = 0; i <= 10; i++){
        int yv = (tensY/10) * i;
        int py = bottom - (int)((float)yv / (float)tensY * (bottom - top));

        draw_line(canvas, left-3, py, left+3, py, sf::Color::Black);

        text.setString(std::to_string(yv));
        text.setPosition({20.0f, (float)py - 6});
        rt.draw(text);
    }

    // draw bins
    for(int i = 0; i < binCount; i++){
        float v = (float)counts[i];

        int barH = (int)(v / (float)tensY * (bottom - top));
        int x0 = left + i * barW;
        int y0 = bottom - barH;

        sf::Color col(120, 170, 240);
        draw_rect(canvas, x0, y0, barW, barH, col, true);
        draw_rect(canvas, x0, y0, barW, barH, sf::Color::Black, false);

        // generates bin label
        int binStart = minV + i * binWidth;
        int binEnd   = binStart + binWidth - 1;

        if(i == binCount - 1){
            binEnd = coveredMax - 1;
        }

        text.setString(std::to_string(binStart) + "-" + std::to_string(binEnd));
        text.setPosition({(float)x0 + 2, (float)bottom + 8});
        rt.draw(text);
    }

    rt.display();
}