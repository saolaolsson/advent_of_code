#include "shared.hpp"

#include <iostream>
#include <ostream>

std::ostream& operator<<(std::ostream& ostream, const Vector2i& v) {
  return ostream << "(" << v.x << ", " << v.y << ")";
}

std::ostream& operator<<(std::ostream& ostream, const Grid& grid) {
  for (const auto& s : grid.locations) {
    ostream << s << "\n";
  }
  return ostream;
}
