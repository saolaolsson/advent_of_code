// https://adventofcode.com/2024/day/14

#include "shared.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <numeric>
#include <optional>
#include <ranges>
#include <vector>

int wrap(const int value, const int max) {
  const auto value_mod_max = value % max;
  return value_mod_max < 0 ? max + value_mod_max : value_mod_max;
}

struct Robot {
  Vector2i location;
  Vector2i velocity;
};

void print_robots(const std::vector<Robot>& robots, const Vector2i& size) {
  Grid grid{size, ' '};
  for (const auto& robot : robots) {
    grid.location(robot.location) = '#';
  }
  std::cout << grid << "\n";
}

std::vector<Robot> tick(const std::vector<Robot>& robots, const Vector2i& size,
                        const int ticks = 1) {
  std::vector<Robot> new_robots;
  for (const auto& robot : robots) {
    Robot new_robot{robot.location + ticks * robot.velocity, robot.velocity};
    new_robot.location.x = wrap(new_robot.location.x, size.x);
    new_robot.location.y = wrap(new_robot.location.y, size.y);
    new_robots.push_back(new_robot);
  }
  return new_robots;
}

std::optional<std::size_t> get_quadrant_index(const Vector2i& location,
                                              const Vector2i& size) {
  return location.x == size.x / 2 || location.y == size.y / 2
             ? std::optional<std::size_t>{}
             : (location.x > size.x / 2) + 2 * (location.y > size.y / 2);
}

int get_robot_safety_factor(const std::vector<Robot>& robots,
                            const Vector2i& size) {
  std::array<int, 4> quadrant_counts{};
  for (const auto& robot : robots) {
    if (const auto qi = get_quadrant_index(robot.location, size)) {
      quadrant_counts[*qi]++;
    }
  }
  return std::ranges::fold_left(quadrant_counts, 1, std::multiplies());
}

int get_ticks_to_christmas_tree(const std::vector<Robot>& robots,
                                const Vector2i& size,
                                const Vector2i& alignment_index) {
  const auto period_x = size.x / std::gcd(size.x, size.y);
  for (auto x = 0; x < period_x; x++) {
    const auto y = (alignment_index.x + x * size.x) / size.y;
    const auto is_solution =
        alignment_index.x + x * size.x == alignment_index.y + y * size.y;
    if (is_solution) {
      const auto ticks = alignment_index.x + x * size.x + 1;
      print_robots(tick(robots, size, ticks), size);
      return ticks;
    }
  }
  return -1;
}

int main() {
  std::vector<Robot> robots;
  for (const auto& v : extract_values(std::cin) | std::views::chunk(4)) {
    robots.emplace_back(Vector2i{v[0], v[1]}, Vector2i{v[2], v[3]});
  }
  Vector2i size{101, 103};
  std::cout << get_robot_safety_factor(tick(robots, size, 100), size) << "\n";
  const Vector2i alignment_index{67, 30};
  std::cout << get_ticks_to_christmas_tree(robots, size, alignment_index)
            << "\n";
}
