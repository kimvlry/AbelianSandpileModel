#include "sandpile.h"

Sandpile::Sandpile() {
  x_coordinates_ = new int64_t;
  y_coordinates_ = new int64_t;
  grid_ = nullptr;
  sands_ = new uint64_t;
}

Sandpile::~Sandpile() {
  delete[] x_coordinates_;
  delete[] y_coordinates_;
  delete[] sands_;
  for (int64_t i = 0; i < grid_height_; ++i) {
    delete[] grid_[i];
  }
  delete[]grid_;
}

void Sandpile::SetInitialState() {
  std::ifstream tcv_file(input_dir(), std::ios::binary);

  try {
    if (!tcv_file.is_open()) {
      throw std::runtime_error("failed to open " + input_dir());
    }

    int64_t x;
    int64_t y;
    int64_t x_edge_ = 0;
    int64_t y_edge_ = 0;
    int64_t sand_amount;

    while (tcv_file >> x >> y >> sand_amount) {
      x_edge_ = std::max(x_edge_, x);
      y_edge_ = std::max(y_edge_, y);
      x_coordinates_ = ExpandArray(x_coordinates_, grid_size_, grid_size_ + 1);
      y_coordinates_ = ExpandArray(y_coordinates_, grid_size_, grid_size_ + 1);
      sands_ = ExpandArray(sands_, grid_size_, grid_size_ + 1);
      x_coordinates_[grid_size_] = x;
      y_coordinates_[grid_size_] = y;
      sands_[grid_size_] = sand_amount;
      ++grid_size_;
    }
    grid_width_ = x_edge_ + 1;
    grid_height_ = y_edge_ + 1;

    grid_ = new uint64_t*[grid_height_];
    for (int64_t i = 0; i < grid_height_; ++i) {
      grid_[i] = new uint64_t[grid_width_]{0};
    }
    for (int64_t i = 0; i < grid_size_; ++i) {
      AddSand(x_coordinates_[i], y_coordinates_[i], sands_[i]);
    }
  }
  catch (const std::exception& e) {
    std::cerr << e.what();
  }
}

template<typename T>
T* Sandpile::ExpandArray(const T* array, int64_t size, int64_t new_size) {
  T* expanded_arr = nullptr;
  try {
    expanded_arr = new T[new_size];
    for (int64_t i = 0; i < size; ++i) {
      expanded_arr[i] = array[i];
    }
    for (int64_t i = size; i < new_size; ++i) {
      expanded_arr[i] = 0;
    }
    return expanded_arr;
  }
  catch (const std::exception& e) {
    delete[] expanded_arr;
    std::cerr << e.what();
    exit(1);
  }
}

void Sandpile::ExpandCanvas(bool to_left,
                            bool to_right,
                            bool to_up,
                            bool to_down) {
  uint64_t** temp = nullptr;
  int64_t x = grid_width_ + to_left + to_right;
  int64_t y = grid_height_ + to_up + to_down;

  try {
    temp = new uint64_t*[y];

    for (int64_t i = 0; i < y; ++i) {
      temp[i] = new uint64_t[x];
      for (int64_t j = 0; j < x; ++j) {
        temp[i][j] = 0;
      }
    }
    for (int64_t i = 0; i < grid_height_; ++i) {
      for (int64_t j = 0; j < grid_width_; ++j) {
        temp[i + to_up][j + to_left] = grid_[i][j];
      }
      delete[] grid_[i];
    }
    delete[] grid_;

    grid_width_ = x;
    grid_height_ = y;
    grid_ = temp;
  }
  catch (const std::exception& e) {
    if (temp) {
      for (int64_t i = 0; i < y; ++i) {
        delete[] temp[i];
      }
      delete[] temp;
    }
    std::cerr << e.what();
  }
}

void Sandpile::AddSand(int64_t x, int64_t y, uint64_t amount) const {
  grid_[y][x] += amount;
}

bool Sandpile::IsStable() const {
  for (int64_t x = 0; x < grid_width_; ++x) {
    for (int64_t y = 0; y < grid_height_; ++y) {
      if (grid_[x][y] >= Image::black) {
        return false;
      }
    }
  }
  return true;
}

void Sandpile::Stabilize(int64_t x, int64_t y) {
  while (max_iter_ > 0 && grid_[y][x] >= Image::black) {
    const bool L_border_ = x == 0;
    const bool U_border_ = y == 0;
    const bool R_boarder_ = x == grid_width_ - 1;
    const bool D_border_ = y == grid_height_ - 1;

    if (L_border_ + U_border_ + R_boarder_ + D_border_ != 0) {
      ExpandCanvas(L_border_, R_boarder_, U_border_, D_border_);
    }
    y += U_border_;
    x += L_border_;
    grid_[y][x] -= Image::black;
    AddSand(x + 1, y, 1);
    AddSand(x, y + 1, 1);
    AddSand(x - 1, y, 1);
    AddSand(x, y - 1, 1);
    ++current_iter_;
    --max_iter_;

    if (freq_ != 0 && current_iter_ % freq_ == 0) {
      Image BMP(grid_width_, grid_height_, grid_);
      BMP.CreateImage(output_dir_, current_iter_);
    }
  }
}

void Sandpile::RunApp() {
  while (max_iter_ > 0 && !IsStable()) {
    for (int64_t y = 0; y < grid_height_; ++y) {
      for (int64_t x = 0; x < grid_width_; ++x) {
        if (grid_[y][x] >= Image::black) {
          Stabilize(x, y);
        }
      }
    }
  }
  Image final_BMP(grid_width_, grid_height_, grid_);
  final_BMP.CreateImage(output_dir_, current_iter_);
}

[[nodiscard]] int64_t Sandpile::freq() const {
  return freq_;
}

[[nodiscard]] int64_t Sandpile::max_iteration() const {
  return max_iter_;
}

[[nodiscard]] std::string Sandpile::input_dir() const {
  return input_dir_;
}

[[nodiscard]] std::string Sandpile::output_dir() const {
  return output_dir_;
}

[[nodiscard]] int64_t Sandpile::grid_size() const {
  return grid_size_;
}

void Sandpile::set_freq(const int64_t freq) {
  freq_ = freq;
}

void Sandpile::set_max_iteration(const int64_t max_iteration) {
  max_iter_ = max_iteration;
}

void Sandpile::set_input_dir(const std::string& input_dir) {
  input_dir_ = input_dir;
}

void Sandpile::set_output_dir(const std::string& output_dir) {
  output_dir_ = output_dir;
}

int64_t* Sandpile::x_coordinates() const {
  return x_coordinates_;
}

int64_t* Sandpile::y_coordinates() const {
  return y_coordinates_;
}

uint64_t* Sandpile::sands() const {
  return sands_;
}
