#include "bargraph.hpp"
#include <cstdio>

static int max_value(int* values, int n){
    int m = values[0];
    for(int i = 1; i < n; i++){
        if(values[i] > m) m = values[i];
    }
    return m;
}

void bargraph(sf::Image& canvas, sf::RenderTexture& rt, int* values, std::string* labels, int sampleSize){
    // Plot area 
    int left   = 80;
    int right  = 760;
    int top    = 50;
    int bottom = 550;

    // axes
    draw_line(canvas, left, top, left, bottom, sf::Color::Black);
    draw_line(canvas, left, bottom, right, bottom, sf::Color::Black);

    if(sampleSize <= 0) return;

    int largestY = max_value(values, sampleSize);
    if(largestY <= 0) largestY = 1;

    // tens scaling 
    int tensY = 1;
    float tempY = (float)largestY;
    while(tempY > 1){
        tempY /= 10;
        tensY *= 10;
    }

    // bar layout
    int plotW = right - left;
    int barAreaW = plotW / sampleSize;
    int barW = (int)(barAreaW * 0.65f);
    if(barW < 5) barW = 5;

    sf::Text text(font);
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(12);

    // y-axis ticks + labels
    for(int i = 0; i <= 10; i++){
        int yv = (tensY/10) * i;
        int py = bottom - (int)((float)yv / (float)tensY * (bottom - top));

        draw_line(canvas, left-3, py, left+3, py, sf::Color::Black);

        text.setString(std::to_string(yv));
        text.setPosition({20.0f, (float)py - 6});
        rt.draw(text);
    }

    // bars + labels
    for(int i = 0; i < sampleSize; i++){
        float v = (float)values[i];
        if(v < 0) v = 0;

        int barH = (int)(v / (float)tensY * (bottom - top));
        int x0 = left + i * barAreaW + (barAreaW - barW)/2;
        int y0 = bottom - barH;

        // simple color cycle
        sf::Color col(80 + (i*40)%150, 120 + (i*60)%120, 220 - (i*30)%160);

        draw_rect(canvas, x0, y0, barW, barH, col, true);
        draw_rect(canvas, x0, y0, barW, barH, sf::Color::Black, false);

        // value above bar
        text.setFillColor(sf::Color::Black);
        text.setString(std::to_string(values[i]));
        text.setPosition({(float)x0, (float)y0 - 16});
        rt.draw(text);

        // category label under bar
        text.setString(labels[i]);
        text.setPosition({(float)x0, (float)bottom + 8});
        rt.draw(text);
    }

    rt.display();
}