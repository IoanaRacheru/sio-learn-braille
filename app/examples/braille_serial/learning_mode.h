#ifndef LEARNING_MODE_H
#define LEARNING_MODE_H
#include <stdint.h>
struct LearningProgress {
  uint32_t correct_answers;
  uint32_t incorrect_attempts;
  uint32_t completed_letters; // Bit 0 = A, ..., bit 25 = Z.
};
void learning_mode_begin();
void learning_mode_handle_line(const char* text);
const LearningProgress& learning_mode_progress();
#endif
