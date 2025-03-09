#include "raylib.h"

int main() {
  // Window dimensions
  int width = 800;
  int height = 800;

  // Window name
  char name[] = "Kote's Window";

  // Circle coords
  int circle_x = 100;
  int circle_y = 100;
  int radius = 30;

  int l_circle_x = circle_x - radius;
  int r_circle_x = circle_x + radius;
  int b_circle_y = circle_y + radius;
  int u_circle_y = circle_y - radius;

  // Edge coordinates for circle
  int left_edge = radius;
  int top_edge = radius;
  int right_edge = width - radius;
  int bottom_edge = height - radius;

  // Rectangle coords
  int rect_x = width / 2;
  int rect_y = 0;
  int rect_width = 50;
  int rect_height = 50;

  int l_rect_x = rect_x;
  int r_rect_x = rect_x + rect_height;
  int b_rect_y = rect_y;
  int u_rect_y = rect_y + rect_width;

  int direction = 10;

  bool collision_with_rect = 
      (b_rect_y >= u_circle_y) && 
      (u_rect_y <= b_circle_y) && 
      (l_rect_x <= r_circle_x) && 
      (r_rect_x >= l_circle_x);

  InitWindow(width, height, name);

  SetTargetFPS(30);

  while (WindowShouldClose() == false) {
    BeginDrawing();
    ClearBackground(WHITE);

    // Game logic start

    if (collision_with_rect) {
      DrawText("Game Over!", width / 2, height / 2, 20, RED);
    } else {
      // Circle coords update
      l_circle_x = circle_x - radius;
      r_circle_x = circle_x + radius;
      b_circle_y = circle_y + radius;
      u_circle_y = circle_y - radius;

      // Rect coords update
      l_rect_x = rect_x;
      r_rect_x = rect_x + rect_height;
      b_rect_y = rect_y;
      u_rect_y = rect_y + rect_width;

      // Update collision value
      collision_with_rect = 
        (b_rect_y >= u_circle_y) && 
        (u_rect_y <= b_circle_y) && 
        (l_rect_x <= r_circle_x) && 
        (r_rect_x >= l_circle_x);

      DrawCircle(circle_x, circle_y, radius, RED);
      DrawRectangle(rect_x, rect_y, rect_width, rect_height, BLUE);

      rect_y += direction;

      if (rect_y > height - rect_height || rect_y < 0) {
        direction = -direction;
      }

      if (IsKeyDown(KEY_D) && circle_x < right_edge) {
        circle_x += 10;
      }

      if (IsKeyDown(KEY_A) && circle_x > left_edge) {
        circle_x -= 10;
      }

      if (IsKeyDown(KEY_W) && circle_y > top_edge) {
        circle_y -= 10;
      }

      if (IsKeyDown(KEY_S) && circle_y < bottom_edge) {
        circle_y += 10;
      }
    }

    // Game logic end

    EndDrawing();
  }
}

