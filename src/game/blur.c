#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>



void apply_blur(SDL_Surface* surface, int blurSize) {
    Uint32* pixels = (Uint32*)surface->pixels;
    int width = surface->w;
    int height = surface->h;

    for (int y = blurSize; y < height - blurSize; y++) {
        for (int x = blurSize; x < width - blurSize; x++) {
            Uint32 sumR = 0, sumG = 0, sumB = 0, sumA = 0;
            for (int ky = -blurSize; ky <= blurSize; ky++) {
                for (int kx = -blurSize; kx <= blurSize; kx++) {
                    Uint32 pixel = pixels[(y + ky) * width + (x + kx)];
                    sumR += (pixel >> 16) & 0xFF;
                    sumG += (pixel >> 8) & 0xFF;
                    sumB += pixel & 0xFF;
                    sumA += (pixel >> 24) & 0xFF;
                }
            }
            int area = (blurSize * 2 + 1) * (blurSize * 2 + 1);
            Uint32 avgR = sumR / area;
            Uint32 avgG = sumG / area;
            Uint32 avgB = sumB / area;
            Uint32 avgA = sumA / area;

            pixels[y * width + x] = (avgA << 24) | (avgR << 16) | (avgG << 8) | avgB;
        }
    }
}