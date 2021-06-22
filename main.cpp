#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_ttf.h>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

using namespace std;
using namespace std::chrono;

// check for errors
#define errcheck(e)                                                            \
  {                                                                            \
    if (e) {                                                                   \
      cout << SDL_GetError() << endl;                                          \
      SDL_Quit();                                                              \
      return -1;                                                               \
    }                                                                          \
  }

const int width = 800;
const int height = 800;
const char* BACKGROUND_PICTURE_PATH = "textures/Background_map.bmp";
const char* TARGET_PICTURE_PATH2 = "textures/target.png";
const char* ARCHER_PICTURE_PATH = "textures/archer.png";
const char* ARROW_PICTURE_PATH = "textures/arrow.png";
const char* FONT_PATH = "fonts/Roboto-Thin.ttf";
const SDL_Color Black = {0,0,0};

void reset_arrow(SDL_Rect &arrow_position, bool &arrow_in_air, bool &arrow_crash, int &acc_x, int &acc_y)
{
    arrow_position  = {60, 655, 30, 15};
    arrow_in_air = false;
    arrow_crash = true;
    acc_x = 0;
    acc_y = 0;
}

int main(int, char **)
{
    int score_number = 0;
    float gravity = 5;
    int acc_x = 0;
    int acc_y = 0;
    unsigned int speed = 15;
    double angle = 0;
    bool arrow_in_air = false;
    bool arrow_crash = false;

    errcheck(SDL_Init(SDL_INIT_VIDEO) != 0);
    errcheck(TTF_Init() != 0);

    SDL_Window *window = SDL_CreateWindow(
        "Archer", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    errcheck(window == nullptr);

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED); // SDL_RENDERER_PRESENTVSYNC
    errcheck(renderer == nullptr);
    
    TTF_Font* Roboto = TTF_OpenFont(FONT_PATH, 24);


    // draw background
    SDL_Surface* temp_background = IMG_Load(BACKGROUND_PICTURE_PATH);
    SDL_Texture* background_picture = SDL_CreateTextureFromSurface(renderer, temp_background);
    SDL_FreeSurface(temp_background);

    // draw target
    SDL_Surface* temp_target = IMG_Load(TARGET_PICTURE_PATH2);
    SDL_Texture* target_picture = SDL_CreateTextureFromSurface(renderer, temp_target);
    SDL_FreeSurface(temp_target);

    // draw archer
    SDL_Surface* temp_archer = IMG_Load(ARCHER_PICTURE_PATH);
    SDL_Texture* archer_picture = SDL_CreateTextureFromSurface(renderer, temp_archer);
    SDL_FreeSurface(temp_archer);

    // draw arrow
    SDL_Surface* temp_arrow = IMG_Load(ARROW_PICTURE_PATH);
    SDL_Texture* arrow_picture = SDL_CreateTextureFromSurface(renderer, temp_arrow);
    SDL_FreeSurface(temp_arrow);

    milliseconds dt(15);

    SDL_Rect target_1_position = {412, 600, 25, 25};
    SDL_Rect target_2_position = {500, 535, 25, 25};
    SDL_Rect target_3_position = {583, 470, 25, 25};
    SDL_Rect target_4_position = {675, 410, 25, 25};
    SDL_Rect target_5_position = {780, 345, 25, 25};
    SDL_Rect archer_position  = {25, 630, 50, 71};
    SDL_Rect arrow_position  = {60, 655, 30, 15};
    SDL_Rect score_position  = {750, 0, 40, 50};
    SDL_Rect speed_x_position  = {0, 0, 20, 25};
    SDL_Rect speed_y_position  = {30, 0, 20, 25};

    steady_clock::time_point current_time = steady_clock::now(); // remember current time
    for (bool game_active = true; game_active;)
    {
        // draw score
        SDL_Surface* surface_core = TTF_RenderText_Solid(Roboto, to_string(score_number).c_str(), Black);
        SDL_Texture* score_picture = SDL_CreateTextureFromSurface(renderer, surface_core);
        SDL_FreeSurface(surface_core);

        // draw speed on x axis
        SDL_Surface* speed_x_surface = TTF_RenderText_Solid(Roboto, to_string(acc_x).c_str(), Black);
        SDL_Texture* speed_x_picture = SDL_CreateTextureFromSurface(renderer, speed_x_surface);
        SDL_FreeSurface(speed_x_surface);

        // draw score on y axis
        SDL_Surface* speed_y_surface = TTF_RenderText_Solid(Roboto, to_string(acc_y* -1).c_str(), Black);
        SDL_Texture* speed_y_picture = SDL_CreateTextureFromSurface(renderer, speed_y_surface);
        SDL_FreeSurface(speed_y_surface);


        SDL_Event event;
        while (SDL_PollEvent(&event))
        { // check if there are some events
            // quit game
            if (event.type == SDL_QUIT)
            {
                game_active = false;
            }

            // keyboard events
            if (event.type == SDL_KEYDOWN &&
                arrow_in_air == false)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_w:
                    acc_y = acc_y - int(1);
                    cout << acc_y << endl;
                    break;
                case SDLK_s:
                    acc_y = acc_y + int(1);
                    cout << acc_y << endl;
                    break;
                case SDLK_a:
                    acc_x = acc_x - int(1);
                    cout << acc_x << endl;
                    break;
                case SDLK_d:
                    acc_x = acc_x + int(1);
                    cout << acc_x << endl;
                    break;
                case SDLK_SPACE:
                    cout << "SPACEBAR" << endl;
                    arrow_in_air = true;
                    break;
                }
            }
        }
        if (target_1_position.x <= arrow_position.x+int(30) &&
            target_1_position.x + int(25) >= arrow_position.x+int(30) &&
            target_1_position.y <= arrow_position.y+int(7) &&
            target_1_position.y + int(25) >= arrow_position.y+int(7))
        {
            cout << "HIT" << endl;
            score_number = score_number +int(10);
            reset_arrow(arrow_position, arrow_in_air, arrow_crash, acc_x, acc_y);
        }
        else if (target_2_position.x <= arrow_position.x+int(30) &&
                target_2_position.x + int(25) >= arrow_position.x+int(30) &&
                target_2_position.y <= arrow_position.y+int(7) &&
                target_2_position.y + int(25) >= arrow_position.y+int(7))
        {
            cout << "HIT" << endl;
            score_number = score_number +int(30);
            reset_arrow(arrow_position, arrow_in_air, arrow_crash, acc_x, acc_y);
        }
        else if (target_3_position.x <= arrow_position.x+int(30) &&
                target_3_position.x + int(25) >= arrow_position.x+int(30) &&
                target_3_position.y <= arrow_position.y+int(7) &&
                target_3_position.y + int(25) >= arrow_position.y+int(7))
        {
            cout << "HIT" << endl;
            score_number = score_number +int(50);
            reset_arrow(arrow_position, arrow_in_air, arrow_crash, acc_x, acc_y);
        }
        else if (target_4_position.x <= arrow_position.x+int(30) &&
                target_4_position.x + int(25) >= arrow_position.x+int(30) &&
                target_4_position.y <= arrow_position.y+int(7) &&
                target_4_position.y + int(25) >= arrow_position.y+int(7))
        {
            cout << "HIT" << endl;
            score_number = score_number +int(70);
            reset_arrow(arrow_position, arrow_in_air, arrow_crash, acc_x, acc_y);
        }
        else if (target_5_position.x <= arrow_position.x+int(30) &&
                target_5_position.x + int(25) >= arrow_position.x+int(30) &&
                target_5_position.y <= arrow_position.y+int(7) &&
                target_5_position.y + int(25) >= arrow_position.y+int(7))
        {
            cout << "HIT" << endl;
            score_number = score_number +int(100);
            reset_arrow(arrow_position, arrow_in_air, arrow_crash, acc_x, acc_y);
        }
        else if (arrow_position.x+int(30) > width+int(100) ||
                arrow_position.y < 0 ||
                arrow_position.y+int(15) > 700)
        {
            cout << "FAIL" << endl;
            score_number = score_number - int(10);
            reset_arrow(arrow_position, arrow_in_air, arrow_crash, acc_x, acc_y);
        }
        else if (arrow_in_air)
        {
            arrow_position.x = arrow_position.x + acc_x;
            arrow_position.y = arrow_position.y + acc_y + gravity;
            gravity += 0.2;
            angle = (acc_y + gravity)*2;
        }
        else if (arrow_crash)
        {
            gravity = 5;
            angle = 0;
        }
        
        if (score_number > 1000)
        {
            cout << "You Win!" << endl;
            score_number = 0;
        }
        else if (score_number < -100)
        {
            cout << "You Lose!" << endl;
            score_number = 0;
        }
        if (!arrow_in_air)
        {
            angle = (acc_y)*2;
        }
        SDL_RenderCopy(renderer, background_picture, NULL, NULL);
        SDL_RenderCopy(renderer, target_picture, NULL, &target_1_position);
        SDL_RenderCopy(renderer, target_picture, NULL, &target_2_position);
        SDL_RenderCopy(renderer, target_picture, NULL, &target_3_position);
        SDL_RenderCopy(renderer, target_picture, NULL, &target_4_position);
        SDL_RenderCopy(renderer, target_picture, NULL, &target_5_position);
        SDL_RenderCopy(renderer, archer_picture, NULL, &archer_position);
        SDL_RenderCopyEx(renderer,arrow_picture,NULL, &arrow_position, angle, NULL, SDL_FLIP_NONE);
        SDL_RenderCopy(renderer, score_picture, NULL, &score_position);
        SDL_RenderCopy(renderer, speed_x_picture, NULL, &speed_x_position);
        SDL_RenderCopy(renderer, speed_y_picture, NULL, &speed_y_position);
        SDL_RenderPresent(renderer); // draw frame to screen

        this_thread::sleep_until(current_time = current_time + dt);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}