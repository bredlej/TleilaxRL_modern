#include <tleilax/tleilax.h>
#include <lua_utils/lua_utils.h>
#include <SDL.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
	print_version();
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window =
		SDL_CreateWindow("Galaxy", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, 1200, 600, 0);

	SDL_Renderer* renderer =
		SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);	

	SDL_RenderClear(renderer);
	randomize_seed_xy(30, 30);
	SDL_Rect r;
	r.x = 30;
	r.y = 30;
	r.w = 100 + rndInt(0, 100);
	r.h = 100 + rndInt(0, 100);
	SDL_SetRenderDrawColor(renderer, rndInt(0, 255), rndInt(0, 255), rndInt(0, 255), 255);
	SDL_RenderFillRect(renderer, &r);
	SDL_RenderPresent(renderer);
	SDL_Delay(1000);
	r.x = 130;
	r.y = 130;
	r.w = 100 + rndInt(0, 100);
	r.h = 100 + rndInt(0, 100);
	SDL_SetRenderDrawColor(renderer, rndInt(0, 255), rndInt(0, 255), rndInt(0, 255), rndInt(0, 255));
	SDL_RenderFillRect(renderer, &r);
	SDL_RenderPresent(renderer);
	SDL_Delay(1000);
	r.x = 230;
	r.y = 230;
	r.w = 100 + rndInt(0, 100);
	r.h = 100 + rndInt(0, 100);
	SDL_SetRenderDrawColor(renderer, rndInt(0, 255), rndInt(0, 255), rndInt(0, 255), rndInt(0, 255));
	SDL_RenderFillRect(renderer, &r);
	SDL_RenderPresent(renderer);
	SDL_Delay(1000);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
