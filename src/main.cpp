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

            //screen->drawTriangle(10, 10, 100, 50, 50, 100);
            //screen->drawTriangle(10, 10, 100, 25, 100, 40, { 1.0f ,0.0f, 0.0f });
            //screen->drawTriangle(10, 10, 40, 100, 25, 100, { 0.0f ,1.0f, 0.0f });

            float *zbuffer = new float[SCREEN_HEIGHT * SCREEN_WIDTH];
            for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) zbuffer[i] = -std::numeric_limits<float>::max();

            for (int i = 0; i< model.num_faces(); i++) {
                std::vector<int> face = model.get_face(i);
                vec4 v0 = model.get_vertex(face[0]);
                vec4 v1 = model.get_vertex(face[1]);
                vec4 v2 = model.get_vertex(face[2]);

                vec4 n = cross_product(v2 - v0, v1 - v0);
                n.normalize();
                float light = dot_product(n, vec4(0, 0, -1));

                int x0 = int((v0.x + 1.0)*SCREEN_WIDTH / 2.0);
                int y0 = int((v0.y + 1.0)*SCREEN_HEIGHT / 2.0);
                int x1 = int((v1.x + 1.0)*SCREEN_WIDTH / 2.0);
                int y1 = int((v1.y + 1.0)*SCREEN_HEIGHT / 2.0);
                int x2 = int((v2.x + 1.0)*SCREEN_WIDTH / 2.0);
                int y2 = int((v2.y + 1.0)*SCREEN_HEIGHT / 2.0);
                if(light > 0)
                    screen->drawTriangle(vec4(x0, y0, v0.z), vec4(x1, y1, v1.z), vec4(x2, y2, v2.z), zbuffer, color_t(1.0, 1.0, 1.0)*light);
                    //screen->drawTriangle(vec4(x0, y0, v0.z), vec4(x1, y1, v1.z), vec4(x2, y2, v2.z), zbuffer, color_t((rand() % 255) / 255.0, (rand() % 255) / 255.0, (rand() % 255) / 255.0));
            }

            screen->render();
            redraw = false;
        }
    }

    // Disable text input
    SDL_StopTextInput();

    return 0;
}
