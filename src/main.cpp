#include "Screen.h"
#include "model.h"
#include "tgaimage.h"
#include <cstdio>

using namespace std;

//const int SCREEN_WIDTH = 640;
const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 512;

struct LightShader : public IShader {

    vec4 world_coords[3];

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


struct TextureShader : public IShader {
    Model &model;
    texcoord_t varying_uv[3];
    vec4 varying_norm[3];

    TextureShader(Model &model) :model(model) { }

    bool fragment(vec4 bar, color_t &color) override {
        float u = bar.x * varying_uv[0].u + bar.y * varying_uv[1].u + bar.z * varying_uv[2].u;
        float v = bar.x * varying_uv[0].v + bar.y * varying_uv[1].v + bar.z * varying_uv[2].v;
        color = model.get_texture(u, v);

        vec4 light_vec(0, 0, 1);

        vec4 norm_vec = bar.x * varying_norm[0] + bar.y * varying_norm[1] + bar.z * varying_norm[2];
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


    float theta = 0, radius = 5;

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
                    radius -= 0.2f;
                    printf("radius at %f\n", radius);
                    redraw = true;
                }
                else if (e.key.keysym.sym == SDLK_DOWN) {
                    radius += 3.1415926f / 18.0f;
                    printf("radius at %f\n", radius);
                    redraw = true;
                }
                else if (e.key.keysym.sym == SDLK_LEFT) {
                    theta -= 3.1415926f / 18.0f;
                    printf("theta at %f\n", theta);
                    redraw = true;
                }
                else if (e.key.keysym.sym == SDLK_RIGHT) {
                    theta += 0.2f;
                    printf("theta at %f\n", theta);
                    redraw = true;
                }
            }
        }

        if (redraw) {
            glClearColor(0.f, 0.f, 0.f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT);

            mat44 ModelView = lookat(vec4(radius * sinf(theta), 0, radius * cosf(theta)), vec4(0, 0, 0), vec4(0, 1, 0));
            mat44 Projection = mat44::identity();
            Projection.m[3][2] = -1.0f / radius;
            mat44 Viewport = viewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

            float *zbuffer = new float[SCREEN_HEIGHT * SCREEN_WIDTH];
            for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) zbuffer[i] = -std::numeric_limits<float>::max();

            for (int i = 0; i < model.num_faces(); i++) {
                std::vector<vertex_t> vertices = model.get_face(i);
                std::vector<vec4> screen_coords;
                for (int j = 0; j < (int)vertices.size(); j++) {
                    vec4 v = (Projection * ModelView * vertices[j].pos).homogenize();
                    if (clip_vertices(v) == true)
                        goto stop;
                    v = Viewport * v;
                    screen_coords.push_back(v);
                    shader.varying_norm[j] = vertices[j].norm;
                    shader.varying_uv[j] = vertices[j].tex;
                }

                screen->drawTriangle(screen_coords[0], screen_coords[1], screen_coords[2], shader, zbuffer);
            stop:;
            }

            screen->render();
            redraw = false;
        }
    }

    // Disable text input
    SDL_StopTextInput();

    return 0;
}
