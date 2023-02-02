/*
 * This file is part of OpenOrion2
 * Copyright (C) 2021 Martin Doucha
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef SDL_SCREEN_H_
#define SDL_SCREEN_H_

#include <cstdint>

// Logical screen size
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

class Screen{
    
    public:

        Screen(void);
		~Screen(void);
        void initScreen(void);
        void redrawScreen(void); // Refresh the screen using the last frame
        void updateScreen(void); // Finish drawing a frame and copy it to screen
        void shutdownScreen(void);

        // registerTexture() must return (nearly) consecutive texture IDs.
        // If the backend does not guarantee consecutive integers, the implementation
        /          / must maintain an internal translation table.
        unsigned registerTexture(unsigned width, unsigned height, const uint32_t *data);
        unsigned registerTexture(unsigned width, unsigned height, const uint8_t *data,
        	const uint8_t *palette, unsigned firstcolor, unsigned colors);
        void setTexturePalette(unsigned id, const uint8_t *palette,
        	unsigned firstcolor, unsigned colors);
        void freeTexture(unsigned id);

        // Draw whole texture
        void drawTexture(unsigned id, int x, int y);

        // Draw part of texture specified by rectangle (offsx,offsy)+(width,height)
        void drawTextureTile(unsigned id, int x, int y, int offsx, int offsy, unsigned width, unsigned height);

        void drawLine(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b);

        void drawRect(int x, int y, unsigned width, unsigned height, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, unsigned thickness = 1);
        void fillRect(int x, int y, unsigned width, unsigned height, uint8_t r = 0,uint8_t g = 0, uint8_t b = 0);
        void clearScreen(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0);

        // Set or remove clipping rectangle on the screen
        void setClipRegion(int x, int y, unsigned width, unsigned height);
        void unsetClipRegion(void);

    private:
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *framebuffer;
        SDL_Surface *drawbuffer;
        struct Texture *textures;
        size_t texture_count, texture_max;
        uint32_t amask, rmask, gmask, bmask;
};

 // Main event loop
        void main_loop(void);
#endif
