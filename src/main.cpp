#include <game.h>
#include <raylib.h>

// NOTE: Game state variabel
GameState game_state = GAME_COUNTDOWN;
float countdown = 3.0f;
bool show_fight = false;
float fight_timer = 0.0f;

int main(void) {
  // TODO: Init
  const int screenWidth = 1200;
  const int screenHeigh = 900;
  const char *title = "SAMU-FIGHT";
  InitWindow(screenWidth, screenHeigh, title);

  // TODO: Load texture plyaer 1
  PlayerTextures p1_tex = {
      .idle = LoadTexture("src/assets/player1/Idle.png"),
      .run = LoadTexture("src/assets/player1/Run.png"),
      .jump = LoadTexture("src/assets/player1/Jump.png"),
      .fall = LoadTexture("src/assets/player1/Fall.png"),
      .attack1 = LoadTexture("src/assets/player1/Attack1.png"),
      .attack2 = LoadTexture("src/assets/player1/Attack2.png"),
      .takehit = LoadTexture("src/assets/player1/TakeHit.png"),
      .death = LoadTexture("src/assets/player1/Death.png"),
  };

  // TODO: Load texture player
  PlayerTextures p2_tex = {
      .idle = LoadTexture("src/assets/player2/Idle.png"),
      .run = LoadTexture("src/assets/player2/Run.png"),
      .jump = LoadTexture("src/assets/player2/Jump.png"),
      .fall = LoadTexture("src/assets/player2/Fall.png"),
      .attack1 = LoadTexture("src/assets/player2/Attack1.png"),
      .attack2 = LoadTexture("src/assets/player2/Attack2.png"),
      .takehit = LoadTexture("src/assets/player2/TakeHit.png"),
      .death = LoadTexture("src/assets/player2/Death.png"),
  };

  // TODO: Load Background Texture
  Texture2D bg = LoadTexture("src/assets/background/Forest2.png");

  Player player = {.pos = {50, GROUND_Y},
                   .speed = 200.f,
                   .velocity_y = 0.0f,
                   .is_grounded = true,
                   .facing_right = true,
                   .is_attacking = false,
                   .is_dead = false,
                   .is_taking_hit = false,
                   .hp = 100,
                   .max_hp = 100,
                   .hit_cooldown = 0.0f,
                   .body_rect = {0, 0, 0, 0},
                   .attack_rect = {0, 0, 0, 0},
                   .state = STATE_IDLE,
                   .anim = {.first = 0,
                            .last = 7,
                            .cur = 0,
                            .speed = 0.1,
                            .duration_left = 0.1,
                            .finished = false}

  };

  Player player2 = {.pos = {650, GROUND_Y},
                    .speed = 200.f,
                    .velocity_y = 0.0f,
                    .is_grounded = true,
                    .facing_right = false,
                    .is_attacking = false,
                    .is_dead = false,
                    .is_taking_hit = false,
                    .hp = 100,
                    .max_hp = 100,
                    .hit_cooldown = 0.0f,
                    .body_rect = {0, 0, 0, 0},
                    .attack_rect = {0, 0, 0, 0},
                    .state = STATE_IDLE,
                    .anim = {.first = 0,
                             .last = 7,
                             .cur = 0,
                             .speed = 0.1,
                             .duration_left = 0.1,
                             .finished = false}

  };

  while (!WindowShouldClose()) {

    // NOTE: Check Texture
    if (!player_textures_valid(&p1_tex)) {
      TraceLog(LOG_ERROR, "Player 1 failed to Load");
      CloseWindow();
      return 1;
    }
    if (!player_textures_valid(&p2_tex)) {
      TraceLog(LOG_ERROR, "Player 2 failed to Load");
      CloseWindow();
      return 1;
    }

    float dt = GetFrameTime();

    // NOTE: COUNTDOWN
    if (game_state == GAME_COUNTDOWN) {
      countdown -= dt;

      if (countdown <= 0.0f && !show_fight) {
        show_fight = true;
        fight_timer = 1.0f;
      }

      if (show_fight) {
        fight_timer -= dt;
        if (fight_timer <= 0.0f) {
          game_state = GAME_PLAYING;
        }
      }
    }

    // NOTE: HIT COOLDOWN
    if (player.hit_cooldown > 0.0f)
      player.hit_cooldown -= dt;
    if (player2.hit_cooldown > 0.0f)
      player2.hit_cooldown -= dt;

    // NOTE: MOVEMENT
    bool moving = false;
    bool p2_moving = false;
    if (game_state == GAME_PLAYING) {
      if (!player.is_attacking && !player.is_dead && !player.is_taking_hit) {
        if (IsKeyDown(KEY_D)) {
          // TODO: Move Right
          player.pos.x += player.speed * dt;
          player.facing_right = true;
          moving = true;
        }

        if (IsKeyDown(KEY_A)) {
          // TODO: Move Left
          player.pos.x -= player.speed * dt;
          player.facing_right = false;
          moving = true;
        }

        if (IsKeyPressed(KEY_W) && player.is_grounded) {
          // TODO: Jump
          player.velocity_y = JUMP_FORCE;
          player.is_grounded = false;
          player_set_state(&player, STATE_JUMP);
        }
      }
      // NOTE: ATTACK
      if (!player.is_attacking) {
        if (IsKeyPressed(KEY_J)) {
          player.is_attacking = true;
          player_set_state(&player, STATE_ATTACK1);
        } else if (IsKeyPressed(KEY_K)) {
          player.is_attacking = true;
          player_set_state(&player, STATE_ATTACK2);
        }
      }

      // NOTE: PLAYER 2
      if (!player2.is_attacking && !player2.is_dead && !player2.is_taking_hit) {
        if (IsKeyDown(KEY_RIGHT)) {
          player2.pos.x += player2.speed * dt;
          player2.facing_right = true;
          p2_moving = true;
        }
        if (IsKeyDown(KEY_LEFT)) {
          player2.pos.x -= player2.speed * dt;
          player2.facing_right = false;
          p2_moving = true;
        }
        if (IsKeyPressed(KEY_UP) && player2.is_grounded) {
          player2.velocity_y = JUMP_FORCE;
          player2.is_grounded = false;
          player_set_state(&player2, STATE_JUMP);
        }
        if (IsKeyPressed(KEY_X)) {
          player2.is_attacking = true;
          player_set_state(&player2, STATE_ATTACK1);
        } else if (IsKeyPressed(KEY_C)) {
          player2.is_attacking = true;
          player_set_state(&player2, STATE_ATTACK2);
        }
      }
    }

    // NOTE: GRAVITY P1
    if (!player.is_grounded) {
      player.velocity_y += GRAVITY * dt;
      player.pos.y += player.velocity_y * dt;

      if (player.velocity_y > 0 && !player.is_attacking) {
        player_set_state(&player, STATE_FALL);
      }

      if (player.pos.y >= GROUND_Y) {
        player.pos.y = GROUND_Y;
        player.velocity_y = 0.0f;
        player.is_grounded = true;
      }
    }

    // NOTE: GRAVITY P2
    if (!player2.is_grounded) {
      player2.velocity_y += GRAVITY * dt;
      player2.pos.y += player2.velocity_y * dt;
      if (player2.velocity_y > 0 && !player2.is_attacking)
        player_set_state(&player2, STATE_FALL);
      if (player2.pos.y >= GROUND_Y) {
        player2.pos.y = GROUND_Y;
        player2.velocity_y = 0.0f;
        player2.is_grounded = true;
      }
    }

    // NOTE: UPDATE HITBOX
    player_update_rects(&player);
    player_update_rects(&player2);

    // NOTE: CHECK HITBOX
    check_hit(&player, &player2);
    check_hit(&player2, &player);

    // NOTE: STATE LOGICS
    if (!player.is_dead && !player.is_taking_hit) {
      if (player.is_grounded && !player.is_attacking) {
        player_set_state(&player, moving ? STATE_RUN : STATE_IDLE);
      }
      if (player.is_attacking && player.anim.finished) {
        player.is_attacking = false;
        player_set_state(&player, moving ? STATE_RUN : STATE_IDLE);
      }
    }
    // take hit finish return idle
    if (player.is_taking_hit && player.anim.finished) {
      player.is_taking_hit = false;
      player_set_state(&player, STATE_IDLE);
    }

    // NOTE: STATE LOGIGS P2
    if (!player2.is_dead && !player2.is_taking_hit) {
      if (player2.is_grounded && !player2.is_attacking)
        player_set_state(&player2, p2_moving ? STATE_RUN : STATE_IDLE);
      if (player2.is_attacking && player2.anim.finished) {
        player2.is_attacking = false;
        player_set_state(&player2, p2_moving ? STATE_RUN : STATE_IDLE);
      }
    }
    // take hit fisnih return idle
    if (player2.is_taking_hit && player2.anim.finished) {
      player2.is_taking_hit = false;
      player_set_state(&player2, STATE_IDLE);
    }

    animation_update(&player.anim);
    animation_update(&player2.anim);

    BeginDrawing();
    ClearBackground(BLACK);
    // NOTE: Background
    float scaleX = (float)screenWidth / bg.width;
    float scaleY = (float)screenHeigh / bg.height;
    float scale = (scaleX > scaleY) ? scaleX : scaleY;

    float newWidth = bg.width * scale;
    float newHeight = bg.height * scale;

    DrawTexturePro(bg, (Rectangle){0, 0, (float)bg.width, (float)bg.height},
                   (Rectangle){(screenWidth - newWidth) / 2,
                               (screenHeigh - newHeight) / 2, newWidth,
                               newHeight},
                   (Vector2){0, 0}, 0.0f, WHITE);
    if (IsKeyPressed(KEY_F))
      ToggleFullscreen();

    // BUG: Debug Draw Hitbox
    if (IsKeyDown(KEY_B)) {
      DrawRectangleLinesEx(player.body_rect, 2, GREEN);
      DrawRectangleLinesEx(player.attack_rect, 2, RED);
      DrawRectangleLinesEx(player2.body_rect, 2, GREEN);
      DrawRectangleLinesEx(player2.attack_rect, 2, RED);
    }

    draw_hp_bar(20, 40, player.hp, player.max_hp, "PLAYER 1", false);
    draw_hp_bar(830, 40, player2.hp, player2.max_hp, "PLAYER 2", true);

    // NOTE: Draw Players
    player_draw(&player2, &p2_tex);
    player_draw(&player, &p1_tex);

    // NOTE: Draw Countdown
    if (game_state == GAME_COUNTDOWN) {
      // Blur effect while countdown
      DrawRectangle(0, 0, screenWidth, screenHeigh, {0, 0, 0, 200});

      if (!show_fight) {
        int number = (int)countdown + 1;
        const char *num_text = TextFormat("%d", number);

        int font_size = 200;
        int text_width = MeasureText(num_text, font_size);

        float fraction = countdown - (int)countdown;
        unsigned char alpha = (unsigned char)(255 * fraction + 80);

        DrawText(num_text, (screenWidth / 2) - (text_width / 2),
                 (screenHeigh / 2) - 100, font_size, {255, 255, 255, alpha});

      } else {
        const char *fight_text = "FIGHT!";
        int font_size = 120;
        int text_width = MeasureText(fight_text, font_size);

        unsigned char r = 255;
        unsigned char g = (unsigned char)(fight_timer * 255);
        unsigned char b = 0;

        DrawText(fight_text, (screenWidth / 2) - (text_width / 2),
                 (screenHeigh / 2) - 60, font_size, {r, g, b, 255});
      }
    }

    if (player.is_dead) {
      DrawText("PLAYER 2 WINS!", 400, 350, 50, RED);
      DrawText("Press R to restart", 450, 300, 30, GRAY);
      if (IsKeyPressed(KEY_R)) {
        // TODO: RESET THE GAME
        reset_game(&player, &player2, &game_state, &countdown, &show_fight,
                   &fight_timer);
      }
    }
    if (player2.is_dead) {
      DrawText("PLAYER 1 WINS!", 400, 350, 50, RED);
      DrawText("Press R to restart", 450, 300, 30, GRAY);
      if (IsKeyPressed(KEY_R)) {
        // TODO: RESET THE GAME
        reset_game(&player, &player2, &game_state, &countdown, &show_fight,
                   &fight_timer);
      }
    }
    EndDrawing();
  }

  UnloadTexture(p1_tex.idle);
  UnloadTexture(p2_tex.idle);
  UnloadTexture(p1_tex.run);
  UnloadTexture(p2_tex.run);
  UnloadTexture(p1_tex.jump);
  UnloadTexture(p2_tex.jump);
  UnloadTexture(p1_tex.fall);
  UnloadTexture(p2_tex.fall);
  UnloadTexture(p1_tex.attack1);
  UnloadTexture(p2_tex.attack1);
  UnloadTexture(p1_tex.attack2);
  UnloadTexture(p2_tex.attack2);
  UnloadTexture(p1_tex.takehit);
  UnloadTexture(p2_tex.takehit);
  UnloadTexture(p1_tex.death);
  UnloadTexture(p2_tex.death);
  UnloadTexture(bg);
  CloseWindow();

  return 0;
}
