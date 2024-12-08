#include <iostream>
#include <istream>
#include <iterator>
#include <string>
#include <vector>

struct Line {
  std::string line_string;
  friend std::istream& operator>>(std::istream& istream, Line& line) {
    return std::getline(istream, line.line_string);
  }
  operator std::string() const { return line_string; }
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

  bool operator==(const Vector2i&) const = default;

  bool operator<(const Vector2i& rhs) const {
    if (x != rhs.x) {
      return x < rhs.x;
    } else {
      return y < rhs.y;
    }
  }
};

std::ostream& operator<<(std::ostream& ostream, const Vector2i& v);

class Grid {
 public:
  class iterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = char;
    using difference_type = std::ptrdiff_t;
    using reference = Vector2i&;

    iterator(const Grid* const grid, const Vector2i& location = {})
        : grid{grid}, location{location} {}
    bool operator!=(const iterator& rhs) const {
      return grid != rhs.grid || location != rhs.location;
    }

    operator Vector2i() const { return location; }

    iterator& operator++() {
      if (++location.x == grid->size().x) {
        location.x = 0;
        location.y++;
      }
      return *this;
    }
    char operator*() const { return grid->location(location); }

   protected:
    const Grid* grid;
    Vector2i location;
  };

  iterator cbegin() const { return iterator(this, Vector2i{}); }
  iterator cend() const { return iterator(this, {0, size().y}); }

  Grid(std::istream& istream) {
    std::copy(std::istream_iterator<Line>{istream}, {},
              std::back_inserter(locations));
  }

  Vector2i size() const {
    return {static_cast<int>(locations[0].size()),
            static_cast<int>(locations.size())};
  }

  char location(const Vector2i& location) const {
    return locations[static_cast<std::size_t>(location.y)]
                    [static_cast<std::size_t>(location.x)];
  }

  char& location(const Vector2i& location) {
    return locations[static_cast<std::size_t>(location.y)]
                    [static_cast<std::size_t>(location.x)];
  }

  bool is_inside(const Vector2i& location) const {
    return location.x >= 0 && location.x < size().x && location.y >= 0 &&
           location.y < size().y;
  }

  friend std::ostream& operator<<(std::ostream& ostream, const Grid& grid);

 private:
  std::vector<std::string> locations;
};

std::ostream& operator<<(std::ostream& ostream, const Grid& grid);

template <typename T>
std::ostream& operator<<(std::ostream& ostream,
                         const std::vector<T>& container) {
  for (const auto& element : container) {
    ostream << element << " ";
  }
  return ostream;
}
