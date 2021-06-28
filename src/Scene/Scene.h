#ifndef SCENE_H
#define SCENE_H

#include "Physics/Interaction.h"
#include "Camera.h"
#include "SDL.h"

#include <thread>
#include <mutex>



/*
renders the scene to a bitmap, executing the raytracer
calculations
*/
namespace Scene
{
	template<typename ftype>
	void render(Camera<ftype>& camera, unsigned char n_threads)
	{
		const size_t n = camera.n_pixels();
		
		if (n_threads > 1)
		{
			std::thread** thread_ptrs = new std::thread * [n_threads];
			std::mutex guard;
			for (unsigned char i = 0; i < n_threads; i++)
			{
				std::cout << "\ninitialising thread " << int(i);
				//define a lambda that a bunch of threads can run...
				thread_ptrs[i] = new std::thread(
					[&camera, &guard, n, i, thread_ptrs]()
					{
						std::thread* current = thread_ptrs[i];
						for (;;)
						{
							size_t rel_mem = 0;
							guard.lock();
							RayInfo<ftype> my_ray = camera.spawn_ray(rel_mem);
							guard.unlock();

							if (rel_mem >= n)
							{
								return;
							}
							camera.write_to_canvas(find_ray_intensity(my_ray).get_data(), rel_mem);
						}
					});
			}
			//we need to ensure all threads are done

			for (unsigned char i = 0; i < n_threads; i++)
			{
				std::cout << "\njoining thread " << int(i);
				thread_ptrs[i]->join();
				delete thread_ptrs[i];
				std::cout << "\ndeleted thread " << int(i);
				thread_ptrs[i] = nullptr;
				//we need to see if all are joined

			}
			delete[] thread_ptrs;
			//we should delete all the thread objects
		}
		else
		{
			for(;;)
			{
				size_t rel_mem = 0;
				RayInfo<ftype> my_ray = camera.spawn_ray(rel_mem);
				if (rel_mem >= n)
				{
					return;
				}
				camera.write_to_canvas(find_ray_intensity(my_ray).get_data(), rel_mem);	
			}
		}
	}

	//makes an SDL window and displays the tting
	template<typename ftype>
	void display(const Camera<ftype>& camera)
	{
		/*SDL_Window* win = NULL;
		SDL_Renderer* renderer = NULL;
		SDL_Texture* bitmapTex = NULL;
		SDL_Surface* bitmapSurface = NULL;
		int posX = 100, posY = 100, width = 320, height = 240;

		SDL_Init(SDL_INIT_VIDEO);

		win = SDL_CreateWindow("Hello World", posX, posY, width, height, 0);

		renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

		bitmapSurface = SDL_LoadBMP("img/hello.bmp");
		bitmapTex = SDL_CreateTextureFromSurface(renderer, bitmapSurface);
		SDL_FreeSurface(bitmapSurface);

		while (1) {
			SDL_Event e;
			if (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) {
					break;
				}
			}

			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, bitmapTex, NULL, NULL);
			SDL_RenderPresent(renderer);
		}

		SDL_DestroyTexture(bitmapTex);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(win);

		SDL_Quit();*/

	}
};

#endif