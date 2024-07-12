#pragma once
#include <iostream>
#include "fstream"

class Image {
public:
  static constexpr int64_t kBitsPerByte = 8;
  static constexpr int64_t kBMPHeaderSize = 14;
  static constexpr int64_t kBMPInfoSize = 40;
  static constexpr int64_t kColorsAmount = 5;
  static constexpr int64_t kColorPalette = kColorsAmount * 4;

  enum color { white, green, yellow, violet, black };

  Image(int64_t x_size, int64_t y_size, uint64_t** grid)
    : x_size_(x_size)
    , y_size_(y_size)
    , grid_(grid) {
  }

  void CreateImage(const std::string& output_dir, int64_t iteration) const;

private:
  int64_t x_size_;
  int64_t y_size_;
  uint64_t** grid_;
};
