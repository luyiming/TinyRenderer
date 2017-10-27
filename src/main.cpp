#include "Screen.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *args[]) {
  Screen *screen = new Screen(SCREEN_WIDTH, SCREEN_HEIGHT, "TinyRenderer");
  screen->Init();

  // Main loop flag
  bool quit = false;

  // Event handler
  SDL_Event e;

  // Enable text input
  SDL_StartTextInput();

  // While application is running
  while (!quit) {
    // Handle events on queue
    while (SDL_PollEvent(&e) != 0) {
      // User requests quit
      if (e.type == SDL_QUIT) {
        quit = true;
      }
      // Handle keypress with current mouse position
      else if (e.type == SDL_TEXTINPUT) {
        // SDL_GetMouseState(&x, &y);
        // handleKeys(e.text.text[0], x, y); 
      } else if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_q) {
          quit = true;
        }
      }
    }
    screen->drawPixel(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    screen->drawLine(1, 120, 160, 480);
    screen->drawLine(1, 120, 160, 420);
    screen->drawLine(1, 120, 160, 360);
    screen->drawLine(1, 120, 160, 300);
    screen->drawLine(1, 120, 160, 240);
    screen->drawLine(1, 120, 160, 180);
    screen->drawLine(1, 120, 160, 120);
    screen->drawLine(1, 120, 160, 60);
    screen->drawLine(1, 120, 160, 0);

    screen->drawLine(1, 360, 160, 480);
    screen->drawLine(1, 360, 160, 420);
    screen->drawLine(1, 360, 160, 360);
    screen->drawLine(1, 360, 160, 300);
    screen->drawLine(1, 360, 160, 240);
    screen->drawLine(1, 360, 160, 180);
    screen->drawLine(1, 360, 160, 120);
    screen->drawLine(1, 360, 160, 60);
    screen->drawLine(1, 360, 160, 0);
                     
    screen->drawLine(1, 120, 1, 360);

    screen->render();
  }

  // Disable text input
  SDL_StopTextInput();

  return 0;
}