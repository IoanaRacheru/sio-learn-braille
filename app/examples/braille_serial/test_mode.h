#ifndef TEST_MODE_H
#define TEST_MODE_H
#include <stdint.h>
struct TestStatistics {
  uint32_t total_answers;
  uint32_t correct_answers;
  uint32_t incorrect_answers;
  unsigned long last_response_ms;
};
void test_mode_begin();
bool test_mode_is_choosing_letter();
void test_mode_handle_line(const char* text, unsigned long submitted_at);
const TestStatistics& test_mode_statistics();
#endif
