#pragma once

#include <fstream>
#include <image.h>

class Sandpile {
public:
  friend class Parser;

  Sandpile();
  ~Sandpile();

  template<typename T>
  static T* ExpandArray(const T* array, int64_t size, int64_t new_size);

  void SetInitialState();
  void ExpandCanvas(bool to_left, bool to_right, bool to_up, bool to_down);
  void AddSand(int64_t x, int64_t y, uint64_t amount) const;
  bool IsStable() const;
  void Stabilize(int64_t x, int64_t y);
  void RunApp();

  [[nodiscard]] int64_t freq() const;
  [[nodiscard]] int64_t max_iteration() const;
  [[nodiscard]] std::string input_dir() const;
  [[nodiscard]] std::string output_dir() const;
  [[nodiscard]] int64_t grid_size() const;
  [[nodiscard]] int64_t* x_coordinates() const;
  [[nodiscard]] int64_t* y_coordinates() const;
  [[nodiscard]] uint64_t* sands() const;

private:
  int64_t freq_ = -1;
  int64_t max_iter_ = -1;
  std::string input_dir_;
  std::string output_dir_;

  int64_t grid_width_ = 0;
  int64_t grid_height_ = 0;
  int64_t current_iter_ = 0;
  int64_t grid_size_ = 0;

  uint64_t** grid_;
  int64_t* x_coordinates_;
  int64_t* y_coordinates_;
  uint64_t* sands_;

  void set_freq(int64_t freq);
  void set_max_iteration(int64_t max_iteration);
  void set_input_dir(const std::string& input_dir);
  void set_output_dir(const std::string& output_dir);
};
