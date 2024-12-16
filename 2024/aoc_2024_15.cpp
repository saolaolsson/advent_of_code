// https://adventofcode.com/2024/day/15

#include "shared.hpp"

#include <algorithm>
#include <iostream>
#include <optional>
#include <ranges>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

std::stringstream get_stream_to_empty_line(std::istream& istream) {
  std::stringstream string_ss;
  std::string string;
  while (std::getline(istream, string)) {
    if (string.empty()) {
      return string_ss;
    }
    string_ss << string << "\n";
  }
  return string_ss;
}

Vector2i char_to_direction(const char c) {
  static const auto directions_by_char = std::unordered_map<char, Vector2i>({
      {'^', Grid::CARDINAL_DIRECTIONS[0]},
      {'>', Grid::CARDINAL_DIRECTIONS[1]},
      {'v', Grid::CARDINAL_DIRECTIONS[2]},
      {'<', Grid::CARDINAL_DIRECTIONS[3]},
  });
  return directions_by_char.at(c);
}

std::optional<Vector2i> find_empty_location(const Grid& grid,
                                            const Vector2i& location,
                                            const Vector2i& direction) {
  auto l = location;
  while (true) {
    if (grid.location(l) == '.') {
      return l;
    } else if (grid.location(l) == '#') {
      return {};
    }
    l += direction;
  }
}

Grid tick(const Grid& grid, const Vector2i& location,
          const Vector2i& direction) {
  const auto empty_location = find_empty_location(grid, location, direction);
  if (!empty_location) {
    return grid;
  }
  auto new_grid = grid;
  auto l = *empty_location;
  while (l != location) {
    new_grid.location(l) = new_grid.location(l - direction);
    l -= direction;
  }
  new_grid.location(location) = '.';
  return new_grid;
}

Grid widen_grid(const Grid& grid) {
  Grid wide_grid({2 * grid.size().x, grid.size().y});
  for (auto y = 0; y < grid.size().y; y++) {
    for (auto x = 0; x < grid.size().x; x++) {
      const auto c = grid.location({x, y});
      char wc0;
      char wc1;
      if (c == 'O') {
        wc0 = '[';
        wc1 = ']';
      } else if (c == '@') {
        wc0 = c;
        wc1 = '.';
      } else {
        wc0 = c;
        wc1 = c;
      }
      wide_grid.location({2 * x + 0, y}) = wc0;
      wide_grid.location({2 * x + 1, y}) = wc1;
    }
  }
  return wide_grid;
}

// box location is (top) left corner of box
std::optional<Vector2i> wide_get_box_location(const Grid& grid,
                                              const Vector2i& location) {
  if (grid.location(location) == '[') {
    return location;
  } else if (grid.location(location) == ']') {
    return {{location.x - 1, location.y}};
  } else {
    return {};
  }
}

bool wide_is_blocked_direction(const Grid& grid, const Vector2i& box_location,
                               const Vector2i& direction) {
  if (direction.x == 0) {
    return grid.location({box_location.x + 0, box_location.y + direction.y}) ==
               '#' ||
           grid.location({box_location.x + 1, box_location.y + direction.y}) ==
               '#';
  } else if (direction.x == 1) {
    return grid.location({box_location.x + 1 + 1, box_location.y}) == '#';
  } else if (direction.x == -1) {
    return grid.location({box_location.x - 1, box_location.y}) == '#';
  }
  assert(false);
  return true;
}

std::optional<std::vector<Vector2i>> wide_find_pushed_adjacent_boxes(
    const Grid& grid, const Vector2i& box_location, const Vector2i& direction) {
  if (wide_is_blocked_direction(grid, box_location, direction)) {
    return {};
  }
  std::vector<Vector2i> box_locations;
  if (direction.x == 0) {
    const auto bl_left = wide_get_box_location(
        grid, {box_location.x + 0, box_location.y + direction.y});
    const auto bl_right = wide_get_box_location(
        grid, {box_location.x + 1, box_location.y + direction.y});
    if (bl_left && bl_right && *bl_left == *bl_right) {
      box_locations.push_back(*bl_left);
    }
    if (bl_left) {
      box_locations.push_back(*bl_left);
    }
    if (bl_right) {
      box_locations.push_back(*bl_right);
    }
  } else if (direction.x == 1) {
    const auto bl =
        wide_get_box_location(grid, {box_location.x + 1 + 1, box_location.y});
    if (bl) {
      box_locations.push_back(*bl);
    }
  } else if (direction.x == -1) {
    const auto bl =
        wide_get_box_location(grid, {box_location.x - 1, box_location.y});
    if (bl) {
      box_locations.push_back(*bl);
    }
  } else {
    assert(false);
  }
  return box_locations;
}

Grid wide_tick(const Grid& grid, const Vector2i& location,
               const Vector2i& direction) {
  auto l = location + direction;
  if (grid.location(l) == '#') {
    // movement direction is blocked
    return grid;
  } else if (grid.location(l) == '.') {
    // movement direction is empty
    auto new_grid = grid;
    std::swap(new_grid.location(l), new_grid.location(location));
    return new_grid;
  } else {
    // movement direction has box that may be movable
    auto box_location = *wide_get_box_location(grid, l);
    std::stack<Vector2i> box_locations_to_check;
    box_locations_to_check.push(box_location);
    std::vector<Vector2i> box_locations;
    while (!box_locations_to_check.empty()) {
      box_location = box_locations_to_check.top();
      box_locations_to_check.pop();
      box_locations.push_back(box_location);

      const auto maybe_box_locations =
          wide_find_pushed_adjacent_boxes(grid, box_location, direction);
      if (!maybe_box_locations) {
        // at least one box movement is blocked so all are blocked
        return grid;
      } else {
        for (const auto bl : *maybe_box_locations) {
          box_locations_to_check.push(bl);
        }
      }
    }

    // move boxes and robot
    auto new_grid = grid;
    for (const auto& bl : box_locations) {
      new_grid.location(bl) = '.';
      new_grid.location(bl + Vector2i{1, 0}) = '.';
    }
    for (const auto& bl : box_locations) {
      new_grid.location(bl + direction) = '[';
      new_grid.location(bl + direction + Vector2i{1, 0}) = ']';
    }
    std::swap(new_grid.location(location),
              new_grid.location(location + direction));

    return new_grid;
  }
}

int get_box_gps_sum(const Grid& grid) {
  auto gps_sum = 0;
  for (auto grid_it = grid.cbegin(); grid_it != grid.cend(); ++grid_it) {
    if (*grid_it == 'O' || *grid_it == '[') {
      const Vector2i location = grid_it;
      gps_sum += location.y * 100 + location.x;
    }
  }
  return gps_sum;
}

int main() {
  auto grid_stream = get_stream_to_empty_line(std::cin);
  Grid grid{grid_stream};
  auto wide_grid = widen_grid(grid);
  const auto directions = std::views::istream<char>(std::cin) |
                          std::views::transform(char_to_direction) |
                          std::ranges::to<std::vector<Vector2i>>();

  for (const auto& direction : directions) {
    const auto robot_location = std::find(grid.cbegin(), grid.cend(), '@');
    grid = tick(grid, robot_location, direction);
  }
  std::cout << get_box_gps_sum(grid) << "\n";

  for (const auto& direction : directions) {
    const auto robot_location =
        std::find(wide_grid.cbegin(), wide_grid.cend(), '@');
    wide_grid = wide_tick(wide_grid, robot_location, direction);
  }
  std::cout << get_box_gps_sum(wide_grid) << "\n";
}
