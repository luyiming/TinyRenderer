#include "Screen.h"
#include "model.h"
#include <cstdio>

using namespace std;

const int SCREEN_WIDTH = 640;
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

            screen->drawLine(10, 10, 100, 100);

            for (int i = 0; i< model.num_faces(); i++) {
                std::vector<int> face = model.get_face(i);
                for (int j = 0; j < 3; j++) {
                    vec4 v0 = model.get_vertex(face[j]);
                    vec4 v1 = model.get_vertex(face[(j + 1) % 3]);
                    int x0 = (v0.x + 1.0)*SCREEN_WIDTH / 2.0;
                    int y0 = (v0.y + 1.0)*SCREEN_HEIGHT / 2.0;
                    int x1 = (v1.x + 1.0)*SCREEN_WIDTH / 2.0;
                    int y1 = (v1.y + 1.0)*SCREEN_HEIGHT / 2.0;
                    screen->drawLine(x0, y0, x1, y1);
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
