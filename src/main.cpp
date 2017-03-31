// Standard library
#include <cmath>
#include <complex>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Third party
#include "SDL.h"

// Our files
#include "window.h"

using namespace std::complex_literals;

Window *window = NULL;

int width = 1600;
int height = 900;
/*
float scaleFactor = 27;
float offsetX = -20;
float offsetY = 4;
*/

double scaleFactor = 1000;
double magnificationSpeed = 2;
double offsetX = -0.74364085;
double offsetY = 0.13182733;

int maxIteration = 1000;

bool init() {
	bool success = true;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL failed to initialize! Error: %s\n", SDL_GetError());
		success = false;
	}
	
	// Initialize the window
	window = new Window(width, height, true, "Mandelbrot Set");
	if (!window->isInitialized()) {
		printf("Window failed to initialize! Error: %s\n", SDL_GetError());
		success = false;
	}

	return success;
}

float scale(float num, float min, float max, float newMin, float newMax) {
	return (newMin - newMax) * (num - min) / (max - min) + newMax;
}

int convertColorChannelHSLtoRGB(double channel, double temp1, double temp2) {
	// Ensure input is between 0 and 1
	if (channel < 0) { channel++; }
	if (channel > 1) { channel--; }

	double converted = 0;
	if (6 * channel < 1) {
		converted = temp2 + (temp1 - temp2) * 6 * channel;
	} else if (2 * channel < 1) {
		converted = temp1;
	} else if (3 * channel < 2) {
		converted = temp2 + (temp1 - temp2) * (0.666 - channel) * 6;
	} else {
		converted = temp2;
	}
	return (int) floor(converted * 255.0);
}

void drawFractalAtPointC(SDL_Renderer *renderer, int x, int y) {
	int iteration = 0;

	double sx = scale((double) x, 0, width, -2.5 / scaleFactor + offsetX, 1 / scaleFactor + offsetX);
	double sy = scale((double) y, 0, height, -1 / scaleFactor + offsetY, 1 / scaleFactor + offsetY);

	// Mandelbrot Set-based Julia Set
	/*
	std::complex<double> c = -0.1011 + 0.9563 * 1i;
	std::complex<double> z = sx + sy * 1i;
	*/
	std::complex<double> c = sx + sy * 1i;
	std::complex<double> z = 0 + 0 * 1i;

	while (abs(z) < 2 && iteration < maxIteration) {
		z = z * z + c;
		iteration += 1;
	}

	double h = ((double) iteration / ((double) maxIteration)) * 30.0;
	double s = 0.75;
	double l = (double) iteration / ((double) maxIteration * 2);

	// HSL to RGB conversion algorithm derived from
	// http://www.niwa.nu/2013/05/math-behind-colorspace-conversions-rgb-hsl
	double temp1 = 0;
	double temp2 = 0;

	if (l < 0.5) {
		temp1 = l * (1.0 + s);
	} else {
		temp1 = l + s - l * s;
	}

	temp2 = 2 * l - temp1;
	h /= 360;

	int r = convertColorChannelHSLtoRGB(h + 0.333, temp1, temp2);
	int g = convertColorChannelHSLtoRGB(h, temp1, temp2);
	int b = convertColorChannelHSLtoRGB(h - 0.333, temp1, temp2);

	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderDrawPoint(renderer, x, height - y);
}

void drawFractalAtPoint(SDL_Renderer *renderer, int x, int y) {
	float x0 = scale(x, 0, width, (-2.5 + offsetX) / scaleFactor, (1 + offsetX) / scaleFactor);
	float y0 = scale(y, 0, height, (-1 + offsetY) / scaleFactor, (1 + offsetY) / scaleFactor);
	float tx = 0.0;
	float ty = 0.0;
	int iteration = 0;
	
	while (tx*tx + ty*ty < 4 && iteration < maxIteration) {
		float xtemp = tx*tx - ty*ty + x0;
		ty = 2*tx*ty + y0;
		tx = xtemp;
		iteration += 1;
	}

	int r = iteration % 215;
	int g = iteration % 100;
	int b = 50;

	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderDrawPoint(renderer, x, y);
}

// Main program loop
int main(int argc, char* args[]) {
	if (init() == false) {
		return -1;
	}

	SDL_Renderer *renderer = window->getRenderer();
	SDL_Event ev;

	bool pause = false;
	bool quit = false;

	int Py = 0;
	int Px = 0;

	while (!quit) {
		while (SDL_PollEvent(&ev) != 0) {
			if (ev.type == SDL_QUIT) { // The 'X' button
				quit = true;
			} else if (ev.type == SDL_KEYDOWN) {
				switch (ev.key.keysym.sym) {
					// TODO: real input handling
					case SDLK_ESCAPE: // Press escape to quit simulation
						quit = true;
						break;
					case SDLK_SPACE: // Press space to pause simulation
						pause = !pause;
						break;
				}
			}
		}

		if (!pause) {
			//window->clear();

			if (Px > width) {
				Px = 0;
				Py += 1;

				if (Py >= height) {
					scaleFactor *= magnificationSpeed;
					Px = 0;
					Py = 0;
				}
				window->update();
			}

			if (Py < height) {
				drawFractalAtPointC(renderer, Px, Py);
				Px += 1;
			}
		}
	}

	// Quit SDL subsystems
	SDL_Quit();

	return 0;
}
