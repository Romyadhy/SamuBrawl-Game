#include <player.h>

#define GROUND_Y 740.0f
#define GROUND_X 500.0f
#define GRAVITY 800.0f
#define JUMP_FORCE -300.0f
// #define VIRTUAL_W 640
// #define VIRTUAL_H 360
//
// NOTE: GAME STATE
typedef enum { GAME_COUNTDOWN, GAME_PLAYING, GAME_OVER } GameState;

// TODO: RESET GAME FUNCTIONS
void reset_game(Player *p1, Player *p2, GameState *gs, float *cd, bool *sf,
                float *ft) {
  p1->pos = {50, GROUND_Y};
  p1->speed = 200.f, p1->velocity_y = 0.0f, p1->is_grounded = true,
  p1->facing_right = true, p1->is_attacking = false, p1->is_dead = false,
  p1->is_taking_hit = false, p1->hp = 100, p1->max_hp = 100,
  p1->hit_cooldown = 0.0f, p1->body_rect = {0, 0, 0, 0},
  p1->attack_rect = {0, 0, 0, 0}, p1->state = STATE_IDLE,
  p1->anim = {.first = 0,
              .last = 7,
              .cur = 0,
              .speed = 0.1,
              .duration_left = 0.1,
              .finished = false};

  p2->pos = {650, GROUND_Y};
  p2->speed = 200.f, p2->velocity_y = 0.0f, p2->is_grounded = true,
  p2->facing_right = false, p2->is_attacking = false, p2->is_dead = false,
  p2->is_taking_hit = false, p2->hp = 100, p2->max_hp = 100,
  p2->hit_cooldown = 0.0f, p2->body_rect = {0, 0, 0, 0},
  p2->attack_rect = {0, 0, 0, 0}, p2->state = STATE_IDLE,
  p2->anim = {.first = 0,
              .last = 7,
              .cur = 0,
              .speed = 0.1,
              .duration_left = 0.1,
              .finished = false};

  *gs = GAME_COUNTDOWN;
  *cd = 3.0f;
  *sf = false;
  *ft = 0.0f;
}
