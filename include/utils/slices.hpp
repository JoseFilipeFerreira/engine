#ifndef SLICES_H
#define SLICES_H

#include <vector>

template<class T>
class Slice {
  private:
    std::vector<T>& vec;
    size_t offset;

  public:
    Slice(size_t offset, std::vector<T>& vec): vec(vec), offset(offset) {}
    auto constexpr operator[](size_t i) const noexcept -> T const& {
        return vec[offset + i];
    }
};
template<class T, size_t cols>
struct SliceMatrix {
  private:
    std::vector<T>& vec;

  public:
    SliceMatrix(std::vector<T>& vec): vec(vec) {}
    auto constexpr operator[](size_t i) const noexcept -> Slice<T> {
        return Slice{i * cols, vec};
    }
};

#endif
