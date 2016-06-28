#include "general.h"

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

int main(int argc, char *argv[])
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture, *texture1;
	SDL_Event event;
	bool quit = false;
	bool flag = 1;
	float offset_x = 1;

	//��ʼ��SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
	}

	window = SDL_CreateWindow("SDL_RenderWithTexture",
							  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
							  SCREEN_WIDTH, SCREEN_HEIGHT,
							  SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	if (window == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't CreateWindow: %s", SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, -1,
								  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Couldn't CreateRenderer: %s", SDL_GetError());
	}

	texture = SDL_CreateTexture(renderer,
								SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
								SCREEN_WIDTH, SCREEN_HEIGHT);
	if (texture == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Couldn't CreateTexture: %s", SDL_GetError());
	}
	//��ѭ��
	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			PrintEvent(&event);
			if (event.type == SDL_QUIT) {
				quit = true;
			}
			if (event.type == SDL_WINDOWEVENT) {
				switch (event.window.event) {
					case SDL_WINDOWEVENT_SIZE_CHANGED: {
						//�����ڴ�С�仯ʱ�������ڿ���
						SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
						//δ�仯��������,���ٳ�����
						texture = SDL_CreateTexture(renderer,
													SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
													SCREEN_WIDTH, SCREEN_HEIGHT);
						if (texture == NULL) {
							SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Couldn't CreateTexture: %s", SDL_GetError());
						}
						break;
					}
				}
			}
		}
		//������ȾĿ��
		if (SDL_SetRenderTarget(renderer, texture)) {
			SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Couldn't SetRenderTarget: %s", SDL_GetError());
		}
		if (SDL_SetRenderDrawColor(renderer, 50, 50, 50, 50)) {
			SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Couldn't SetRenderDrawColor: %s", SDL_GetError());
		}
		if (SDL_RenderClear(renderer)) {
			SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Couldn't RenderClear: %s", SDL_GetError());
		}
		if (SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255)) {
			SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Couldn't SetRenderDrawColor: %s", SDL_GetError());
		}

		RenderDrawCoordinate(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, 0.025);

		//ˮƽ�˶�����(��������任)
		if (flag == 1 && 0 <= offset_x && SCREEN_WIDTH > offset_x) {
			offset_x += 1;
		}
		else if (flag == 1 && SCREEN_WIDTH <= offset_x) {
			flag = 0;
			offset_x -= 1;
		}

		if (flag == 0 && 0 <= offset_x && SCREEN_WIDTH >= offset_x) {
			offset_x -= 1;
		}
		else if (flag == 0 && 0 >= offset_x) {
			flag = 1;
			offset_x += 1;
		}

		RenderFillCircle(renderer, offset_x, SCREEN_HEIGHT*0.5, SCREEN_WIDTH*0.1);
		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
	return 0;
}