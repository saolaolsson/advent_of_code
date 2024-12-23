#include <algorithm>
#include <cassert>
#include <iostream>
#include <istream>
#include <iterator>
#include <string>
#include <vector>

std::vector<int> extract_values(std::istream& istream);

struct Line {
  std::string line_string;
  friend std::istream& operator>>(std::istream& istream, Line& line) {
    return std::getline(istream, line.line_string);
  }
  operator std::string() const { return line_string; }
  operator std::vector<char>() const {
    return {line_string.cbegin(), line_string.cend()};
  }
};

struct Vector2i {
  int x;
  int y;

  Vector2i& operator+=(const Vector2i& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  Vector2i operator+(const Vector2i& rhs) const {
    auto v = *this;
    return v += rhs;
  }

  Vector2i& operator-=(const Vector2i& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }

  Vector2i operator-(const Vector2i& rhs) const {
    auto v = *this;
    return v -= rhs;
  }

  auto operator<=>(const Vector2i&) const = default;
};

Vector2i operator*(const int lhs, const Vector2i& rhs);

std::ostream& operator<<(std::ostream& ostream, const Vector2i& v);

template <typename T>
class Matrix {
 public:
  class iterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = Vector2i&;

    iterator(const Matrix* const matrix, const Vector2i& location = {})
        : matrix{matrix}, location{location} {}
    bool operator!=(const iterator& rhs) const {
      return matrix != rhs.matrix || location != rhs.location;
    }

    operator Vector2i() const { return location; }

    iterator& operator++() {
      if (++location.x == matrix->size().x) {
        location.x = 0;
        location.y++;
      }
      return *this;
    }
    T operator*() const { return matrix->location(location); }

   protected:
    const Matrix* matrix;
    Vector2i location;
  };

  iterator cbegin() const { return iterator(this, Vector2i{}); }
  iterator cend() const { return iterator(this, {0, size().y}); }

  static constexpr auto DIRECTIONS = std::to_array<Vector2i>(
      {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}});
  static constexpr auto CARDINAL_DIRECTIONS =
      std::to_array<Vector2i>({{0, -1}, {1, 0}, {0, 1}, {-1, 0}});
  enum CardinalDirection : int { NORTH, EAST, SOUTH, WEST, END };

  Matrix(std::istream& istream) {
    std::copy(std::istream_iterator<Line>{istream}, {},
              std::back_inserter(locations));
  }

  Matrix(const Vector2i& size, const T ch = {}) {
    std::fill_n(std::back_inserter(locations), size.y,
                std::vector(static_cast<std::size_t>(size.x), ch));
  }

  Vector2i size() const {
    return {static_cast<int>(locations[0].size()),
            static_cast<int>(locations.size())};
  }

  T location(const Vector2i& location) const {
    assert_valid_location(location);
    return locations[static_cast<std::size_t>(location.y)]
                    [static_cast<std::size_t>(location.x)];
  }

  T& location(const Vector2i& location) {
    assert_valid_location(location);
    return locations[static_cast<std::size_t>(location.y)]
                    [static_cast<std::size_t>(location.x)];
  }

  bool is_inside(const Vector2i& location) const {
    return location.x >= 0 && location.x < size().x && location.y >= 0 &&
           location.y < size().y;
  }

  friend std::ostream& operator<<(std::ostream& ostream,
                                  const Matrix<char>& matrix);

 private:
  void assert_valid_location([[maybe_unused]] const Vector2i& location) const {
    assert(location.x >= 0);
    assert(location.y >= 0);
    assert(static_cast<std::size_t>(location.y) < locations.size());
    assert(static_cast<std::size_t>(location.x) <
           locations[static_cast<std::size_t>(location.y)].size());
  }

  std::vector<std::vector<T>> locations;
};

std::ostream& operator<<(std::ostream& ostream, const Matrix<char>& matrix);

using Grid = Matrix<char>;

void print_grid(const Grid& grid, std::vector<Vector2i>& locations);

template <typename T>
std::ostream& operator<<(std::ostream& ostream,
                         const std::vector<T>& container) {
  for (const auto& element : container) {
    ostream << element << " ";
  }
  return ostream;
}
