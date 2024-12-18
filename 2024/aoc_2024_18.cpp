// https://adventofcode.com/2024/day/18

#include "shared.hpp"

#include <iostream>
#include <limits>
#include <ranges>
#include <span>
#include <stack>
#include <vector>

static int find_path(const Vector2i& grid_size,
                     const std::span<Vector2i>& bytes,
                     const Vector2i& start_location,
                     const Vector2i& goal_location, const bool find_shortest) {
  Grid grid{grid_size, ' '};
  struct Location {
    Vector2i location;
    int cost;
  };
  auto min_cost = std::numeric_limits<int>::max();
  Matrix<int> min_costs{grid.size(), std::numeric_limits<int>::max()};
  std::stack<Location> locations;
  locations.push({start_location, 0});
  for (const auto& byte : bytes) {
    grid.location(byte) = '#';
  }

  while (!locations.empty()) {
    const auto location = locations.top();
    locations.pop();

    if (!grid.is_inside(location.location)) {
      continue;
    }

    if (grid.location(location.location) == '#') {
      continue;
    }

    const auto is_costlier_route =
        location.cost > min_cost ||
        location.cost >= min_costs.location(location.location);
    if (is_costlier_route) {
      continue;
    }
    min_costs.location(location.location) = location.cost;

    if (location.location == goal_location) {
      min_cost = std::min(min_cost, location.cost);
      if (find_shortest) {
        continue;
      } else {
        break;
      }
    }

    for (const auto& direction : Grid::CARDINAL_DIRECTIONS) {
      locations.push({location.location + direction, location.cost + 1});
    }
  }
  return min_cost;
}

int main() {
  std::vector<Vector2i> bytes;
  for (const auto v : extract_values(std::cin) | std::views::chunk(2)) {
    bytes.emplace_back(v[0], v[1]);
  }

  const Vector2i grid_size{71, 71};
  const Vector2i start_location{0, 0};
  const auto goal_location = grid_size - Vector2i{1, 1};

  constexpr auto min_bytes = 1024uz;
  const auto cost = find_path(grid_size, bytes | std::views::take(min_bytes),
                              start_location, goal_location, true);
  std::cout << cost << "\n";

  for (const auto n_bytes : std::views::iota(min_bytes, bytes.size())) {
    const auto has_path =
        find_path(grid_size, bytes | std::views::take(n_bytes), start_location,
                  goal_location, false) != std::numeric_limits<int>::max();
    if (!has_path) {
      std::cout << bytes[n_bytes - 1] << "\n";
      break;
    }
  }
}
