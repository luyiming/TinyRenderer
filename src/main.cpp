#include "Screen.h"
#include "model.h"
#include <cstdio>

using namespace std;

//const int SCREEN_WIDTH = 640;
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *args[]) {
    Screen *screen = new Screen(SCREEN_WIDTH, SCREEN_HEIGHT, "TinyRenderer");
    screen->Init();
    
    Model model("D:/Projects/TinyRenderer/src/model/african_head.obj");

    // Main loop flag
    bool quit = false;
    bool redraw = true;

    // Event handler
    SDL_Event e;

    // Enable text input
    SDL_StartTextInput();

    // While application is running
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_q) {
                    quit = true;
                }
            }
        }

        if (redraw) {
            glClearColor(0.f, 0.f, 0.f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT);

            float *zbuffer = new float[SCREEN_HEIGHT * SCREEN_WIDTH];
            for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) zbuffer[i] = -std::numeric_limits<float>::max();

            for (int i = 0; i< model.num_faces(); i++) {
                std::vector<int> face = model.get_face(i);
                vec4 screen_coords[3];
                vec4 world_coords[3];
                for (int j = 0; j < 3; j++) {
                    world_coords[j] = model.get_vertex(face[j]);
                    screen_coords[j] = screen->world2screen(model.get_vertex(face[j]));
                }

                vec4 light_vec(0, 0, -1);
                    
                vec4 norm_vec = cross_product(world_coords[2] - world_coords[0], world_coords[1] - world_coords[0]);
                norm_vec.normalize();
                float intensity = dot_product(norm_vec, light_vec);

                if (intensity > 0) {
                    screen->drawTriangle(screen_coords[0], screen_coords[1], screen_coords[2], zbuffer, color_t(1.0, 1.0, 1.0)*intensity);
                    //screen->drawTriangle(vec4(x0, y0, v0.z), vec4(x1, y1, v1.z), vec4(x2, y2, v2.z), zbuffer, color_t((rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0));
                }
            }

            screen->render();
            redraw = false;
        }
    }

    // Disable text input
    SDL_StopTextInput();

    return 0;
}
