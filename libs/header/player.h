#include <animation.h>
#include <raylib.h>

// NOTE: Player State
typedef enum {
  STATE_IDLE,
  STATE_RUN,
  STATE_JUMP,
  STATE_FALL,
  STATE_ATTACK1,
  STATE_ATTACK2,
  STATE_TAKEHIT,
  STATE_DEATH
} PlayerState;

// NOTE: PLAYER STATE
typedef struct Player {
  Vector2 pos;
  float speed;
  float velocity_y;
  bool is_grounded;
  bool facing_right;
  bool is_attacking;
  bool is_dead;
  bool is_taking_hit;
  int hp;
  int max_hp;
  float hit_cooldown;
  Rectangle body_rect;
  Rectangle attack_rect;
  PlayerState state;
  Animation anim;
} Player;

// NOTE: PLAYER TEXTURE STATE
typedef struct PlayerTextures {
  Texture2D idle;
  Texture2D run;
  Texture2D jump;
  Texture2D fall;
  Texture2D attack1;
  Texture2D attack2;
  Texture2D takehit;
  Texture2D death;
} PlayerTextures;

void player_set_state(Player *p, PlayerState new_state) {
  if (p->state == new_state)
    return;
  p->state = new_state;

  switch (new_state) {
  case STATE_IDLE:
    p->anim = (Animation){
        .first = 0,
        .last = 7,
        .cur = 0,
        .speed = 0.1f,
        .duration_left = 0.1f,
        .finished = false,
        .finished_stop = false,
    };
    break;
  case STATE_RUN:
    p->anim = (Animation){
        .first = 0,
        .last = 7,
        .cur = 0,
        .speed = 0.08f,
        .duration_left = 0.08f,
        .finished = false,
        .finished_stop = false,
    };
    break;
  case STATE_JUMP:
    p->anim = (Animation){
        .first = 0,
        .last = 1,
        .cur = 0,
        .speed = 0.1f,
        .duration_left = 0.1f,
        .finished = false,
        .finished_stop = false,
    };
    break;
  case STATE_FALL:
    p->anim = (Animation){
        .first = 0,
        .last = 1,
        .cur = 0,
        .speed = 0.12f,
        .duration_left = 0.12f,
        .finished = false,
        .finished_stop = false,
    };
    break;
  case STATE_ATTACK1:
    p->anim = (Animation){
        .first = 0,
        .last = 5,
        .cur = 0,
        .speed = 0.07f,
        .duration_left = 0.07f,
        .finished = false,
        .finished_stop = true,
    };
    break;
  case STATE_ATTACK2:
    p->anim = (Animation){
        .first = 0,
        .last = 5,
        .cur = 0,
        .speed = 0.07f,
        .duration_left = 0.07f,
        .finished = false,
        .finished_stop = true,
    };
    break;
  case STATE_TAKEHIT:
    p->anim = (Animation){
        .first = 0,
        .last = 3,
        .cur = 0,
        .speed = 0.07f,
        .duration_left = 0.07f,
        .finished = false,
        .finished_stop = true,
    };
    break;

  case STATE_DEATH:
    p->anim = (Animation){
        .first = 0,
        .last = 5,
        .cur = 0,
        .speed = 0.07f,
        .duration_left = 0.07f,
        .finished = false,
        .finished_stop = true,
    };
    break;
  }
}

// TODO: Get Texture
Texture2D *get_texture(PlayerState state, PlayerTextures *tex) {
  switch (state) {
  case STATE_RUN:
    return &tex->run;
  case STATE_JUMP:
    return &tex->jump;
  case STATE_FALL:
    return &tex->fall;
  case STATE_ATTACK1:
    return &tex->attack1;
  case STATE_ATTACK2:
    return &tex->attack2;
  case STATE_TAKEHIT:
    return &tex->takehit;
  case STATE_DEATH:
    return &tex->death;
  default:
    return &tex->idle;
  }
}

void player_draw(Player *p, PlayerTextures *tex) {
  Rectangle source = animation_frame(&p->anim, 8);
  if (!p->facing_right)
    source.width = -source.width;

  DrawTexturePro(*get_texture(p->state, tex), source,
                 {p->pos.x, p->pos.y, 500, 500}, {0, 0}, 0.0f, WHITE);
}

// TODO: Check textures
bool player_textures_valid(PlayerTextures *tex) {
  return tex->idle.id != 0 && tex->run.id != 0 && tex->jump.id != 0 &&
         tex->fall.id != 0 && tex->attack1.id != 0 && tex->attack2.id != 0;
}

// TODO: Hitbox Update
void player_update_rects(Player *p) {
  if (p->facing_right) {
    p->body_rect = (Rectangle){
        .x = p->pos.x + 190,
        .y = p->pos.y + 100,
        .width = 100,
        .height = 250,
    };
    p->attack_rect = (Rectangle){
        .x = p->pos.x + 290,
        .y = p->pos.y + 130,
        .width = 150,
        .height = 130,
    };
  } else {
    p->body_rect = (Rectangle){
        .x = p->pos.x + 210,
        .y = p->pos.y + 100,
        .width = 100,
        .height = 250,
    };
    p->attack_rect = (Rectangle){
        .x = p->pos.x + 60,
        .y = p->pos.y + 130,
        .width = 150,
        .height = 130,
    };
  }
}

// TODO: Check Hit
void check_hit(Player *attacker, Player *defender) {
  if (!attacker->is_attacking)
    return;
  if (defender->is_dead)
    return;
  if (defender->hit_cooldown > 0.0f)
    return;

  if (CheckCollisionRecs(attacker->attack_rect, defender->body_rect)) {
    defender->hp -= 10;
    defender->hit_cooldown = 0.6f;

    if (defender->hp <= 0) {
      defender->hp = 0;
      defender->is_dead = true;
      defender->is_attacking = false;
      player_set_state(defender, STATE_DEATH);
    } else {
      defender->is_taking_hit = true;
      defender->is_attacking = false;
      player_set_state(defender, STATE_TAKEHIT);
    }
  }
}

// TODO: HP Bar
void draw_hp_bar(int x, int y, int hp, int max_hp, const char *label,
                 bool flip) {
  int bar_widht = 350;
  int bar_height = 28;

  float pct = (float)hp / (float)max_hp;

  Color fill_color;
  if (pct > 0.6f)
    fill_color = GREEN;
  else if (pct > 0.3f)
    fill_color = YELLOW;
  else
    fill_color = RED;

  int fill_width = (int)(bar_widht * pct);

  DrawRectangle(x, y, bar_widht, bar_height, DARKGRAY);

  if (flip) {
    DrawRectangle(x + (bar_widht - fill_width), y, fill_width, bar_height,
                  fill_color);
  } else {
    DrawRectangle(x, y, fill_width, bar_height, fill_color);
  }
  DrawRectangleLines(x, y, bar_widht, bar_height, WHITE);
  DrawText(label, x, y - 22, 20, WHITE);
}
