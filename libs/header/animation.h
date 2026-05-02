#include <raylib.h>

typedef struct Animation {
  int first;
  int last;
  int cur;
  float speed;
  float duration_left;
  bool finished;
  bool finished_stop;
} Animation;

void animation_update(Animation *self) {
  if (self->finished)
    return;
  float dt = GetFrameTime();
  self->duration_left -= dt;

  if (self->duration_left <= 0.0) {
    self->duration_left = self->speed;
    self->cur++;

    if (self->cur > self->last) {
      if (self->finished_stop) {
        self->cur = self->last;
        self->finished = true;
      } else {
        self->cur = self->first;
      }
    }
  }
}

Rectangle animation_frame(Animation *self, int num_frames_per_row) {
  int x = (self->cur % num_frames_per_row) * 200.0;
  int y = (self->cur / num_frames_per_row) * 200.0;
  return (Rectangle){
      .x = (float)x, .y = (float)y, .width = 200.0, .height = 200.0};
}
