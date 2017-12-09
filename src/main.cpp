#include "Screen.h"
#include "model.h"
#include "tgaimage.h"
#include <cstdio>

using namespace std;

//const int SCREEN_WIDTH = 640;
const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 512;

vec4 world2screen(vec4 v)
{
    return vec4((v.x + 1.0f)*SCREEN_WIDTH / 2.0f, (v.y + 1.0f)*SCREEN_HEIGHT / 2.0f, v.z);
}

struct LightShader : public IShader {
    Model &model;
    color_t color;
    vec4 screen_coords[3];
    vec4 world_coords[3];

    LightShader(Model &model) :model(model) {}

    vec4 vertex(int iface, int nthvert) {
        world_coords[nthvert] = model.get_vertex(model.get_face(iface)[nthvert]);
        screen_coords[nthvert] = world2screen(world_coords[nthvert]);

        color = color_t((rand() % 255) / 255.0f, (rand() % 255) / 255.0f, (rand() % 255) / 255.0f);
        return screen_coords[nthvert];
    }

    bool fragment(vec4 bar, color_t &color) override {
        vec4 light_vec(0, 0, -1);

        vec4 norm_vec = cross_product(world_coords[2] - world_coords[0], world_coords[1] - world_coords[0]);
        norm_vec.normalize();
        float intensity = dot_product(norm_vec, light_vec);

        color = color_t(1.0, 1.0, 1.0)*intensity;

        return intensity < 0;
    }

    ~LightShader() {}
};

float camera = 5.0f;

struct TextureShader : public IShader {
    Model &model;
    color_t color;
    vec4 screen_coords[3];
    vec4 world_coords[3];
    texcoord_t varying_uv[3];
    vec4 varying_norm[3];

    mat44 ViewPort;
    mat44 Projection;

    TextureShader(Model &model) :model(model) {
        ViewPort = viewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    vec4 vertex(int iface, int nthvert) {
        world_coords[nthvert] = model.get_vertex(model.get_face(iface)[nthvert]);

        Projection.set_identity();
        Projection.m[3][2] = -1.0f / camera;
        world_coords[nthvert] = Projection * world_coords[nthvert];
        world_coords[nthvert].homogenize();

        screen_coords[nthvert] = ViewPort * world_coords[nthvert];

        varying_uv[nthvert] = model.uv(iface, nthvert);
        varying_norm[nthvert] = model.norm(iface, nthvert);

        color = color_t((rand() % 255) / 255.0f, (rand() % 255) / 255.0f, (rand() % 255) / 255.0f);
        return screen_coords[nthvert];
    }

    bool fragment(vec4 bar, color_t &color) override {
        float u = bar.x * varying_uv[0].u + bar.y * varying_uv[1].u + bar.z * varying_uv[2].u;
        float v = bar.x * varying_uv[0].v + bar.y * varying_uv[1].v + bar.z * varying_uv[2].v;
        color = model.get_texture(u, v);

        vec4 light_vec(0, 0, 1);

        vec4 norm_vec = bar.x * varying_norm[0] + bar.y * varying_norm[1] + bar.z * varying_norm[2];
        //vec4 norm_vec = cross_product(world_coords[2] - world_coords[0], world_coords[1] - world_coords[0]);
        norm_vec.normalize();
        float intensity = dot_product(norm_vec, light_vec);

        if (intensity < 0)
            intensity = 0;

        color *= intensity;

        return false;
    }

    ~TextureShader() {}
};

int main(int argc, char *args[]) {
    Screen *screen = new Screen(SCREEN_WIDTH, SCREEN_HEIGHT, "TinyRenderer");
    screen->Init();

    Model model("D:/Projects/TinyRenderer/src/model/african_head.obj",
        "D:/Projects/TinyRenderer/src/model/african_head_diffuse.tga");

    //LightShader shader(model);
    TextureShader shader(model);

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
                else if (e.key.keysym.sym == SDLK_UP) {
                    camera -= 0.2f;
                    printf("camera at %f\n", camera);
                    redraw = true;
                }
                else if (e.key.keysym.sym == SDLK_DOWN) {
                    camera += 0.2f;
                    printf("camera at %f\n", camera);
                    redraw = true;
                }
            }
        }

        if (redraw) {
            glClearColor(0.f, 0.f, 0.f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT);

            float *zbuffer = new float[SCREEN_HEIGHT * SCREEN_WIDTH];
            for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) zbuffer[i] = -std::numeric_limits<float>::max();

            for (int i = 0; i < model.num_faces(); i++) {
                std::vector<int> face = model.get_face(i);
                for (int j = 0; j < 3; j++) {
                    shader.vertex(i, j);
                }

                screen->drawTriangle(shader.screen_coords[0], shader.screen_coords[1], shader.screen_coords[2], shader, zbuffer);
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
