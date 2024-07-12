#include "image.h"

void Image::CreateImage(const std::string& output_dir,
                        int64_t iteration) const {
  std::string iter_number = std::to_string(iteration);
  std::string filename = output_dir + iter_number + ".bmp";
  std::ofstream file;

  try {
    file.open(filename, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
      throw std::runtime_error("failed to create file " + filename);
    }

    const int64_t padding = (4 - (((x_size_ + 1) * 4) / kBitsPerByte) % 4) % 4;
    const int64_t fileSize = kBMPHeaderSize + kBMPInfoSize + x_size_ * y_size_ * 3 + padding * y_size_;
    unsigned char fileHeader[kBMPHeaderSize];
    // bmp signature
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    // file size
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;
    // reserved 1
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    // reserved 2
    fileHeader[8] = 0;
    fileHeader[9] = 0;
    // pixel data offset
    fileHeader[10] = kBMPHeaderSize + kColorPalette + kBMPInfoSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    unsigned char infoHeader[kBMPInfoSize];
    // header size
    infoHeader[0] = kBMPInfoSize;
    infoHeader[1] = 0;
    infoHeader[2] = 0;
    infoHeader[3] = 0;
    // image width
    infoHeader[4] = x_size_;
    infoHeader[5] = x_size_ >> 8;
    infoHeader[6] = x_size_ >> 16;
    infoHeader[7] = x_size_ >> 24;
    // image height
    infoHeader[8] = y_size_;
    infoHeader[9] = y_size_ >> 8;
    infoHeader[10] = y_size_ >> 16;
    infoHeader[11] = y_size_ >> 24;
    // planes
    infoHeader[12] = 1;
    infoHeader[13] = 0;
    // bits per pixel
    infoHeader[14] = 4;
    infoHeader[15] = 0;
    for (int i = 16; i < kBMPInfoSize; i++) {
      infoHeader[i] = 0;
    }

    // color palette (B,G,R)
    infoHeader[32] = kColorsAmount;
    unsigned char color_palette[kColorPalette] = {
      255, 255, 255, 0, // 0 white
      29, 206, 106, 0, // 1 green
      27, 242, 227, 0, // 2 yellow
      153, 51, 255, 0, // 3 pink
      25, 0, 51, 0 // 4 black
    };

    file.write(reinterpret_cast<char*>(fileHeader), kBMPHeaderSize);
    file.write(reinterpret_cast<char*>(infoHeader), kBMPInfoSize);
    file.write(reinterpret_cast<char*>(color_palette), kColorPalette);

    for (int64_t y = 0; y < y_size_; ++y) {
      for (int64_t x = 0; x < x_size_; x++) {
        unsigned char first_half_of_a_byte = (grid_[y][x] >= black)
            ? black
            : grid_[y][x];
        unsigned char second_half_of_a_byte = (grid_[y][++x] >= black)
            ? black
            : grid_[y][x];
        file << static_cast<char>(second_half_of_a_byte + (first_half_of_a_byte << (kBitsPerByte / 2)));
      }
      for (int k = 0; k < padding; ++k) {
        file << static_cast<unsigned char>(0);
      }
    }
  }
  catch (const std::exception& e) {
    std::cout << e.what();
  }
}
