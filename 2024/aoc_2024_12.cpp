// https://adventofcode.com/2024/day/12

#include "shared.hpp"

#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <map>
#include <set>
#include <stack>

static std::set<Vector2i> find_connected_locations(
    const Grid& grid, const Vector2i& start_location) {
  std::stack<Vector2i> locations_to_visit;
  locations_to_visit.push(start_location);
  std::set<Vector2i> connected_locations;
  while (!locations_to_visit.empty()) {
    const auto location = locations_to_visit.top();
    locations_to_visit.pop();

    if (connected_locations.contains(location)) {
      continue;
    }
    connected_locations.insert(location);

    for (const auto delta : Grid::CARDINAL_DIRECTIONS) {
      const auto maybe_new_location = location + delta;
      const auto is_connected_location =
          grid.is_inside(maybe_new_location) &&
          grid.location(maybe_new_location) == grid.location(start_location);
      if (is_connected_location) {
        locations_to_visit.push(maybe_new_location);
      }
    }
  }
  return connected_locations;
}

std::bitset<4> get_patch(const std::set<Vector2i>& locations,
                         const Vector2i& top_left_corner_location) {
  const auto x = top_left_corner_location.x;
  const auto y = top_left_corner_location.y;
  return static_cast<unsigned long long>(
      locations.contains({x + 0, y + 0}) << 0 |
      locations.contains({x + 1, y + 0}) << 1 |
      locations.contains({x + 0, y + 1}) << 2 |
      locations.contains({x + 1, y + 1}) << 3);
}

static int calc_perimeter(const std::set<Vector2i>& locations) {
  auto internal_perimeter = 0;
  for (const auto& location : locations) {
    for (const auto delta : Grid::CARDINAL_DIRECTIONS) {
      internal_perimeter += locations.contains(location + delta);
    }
  }
  return 4 * static_cast<int>(locations.size()) - internal_perimeter;
}

static int count_double_edges(const std::set<Vector2i>& locations) {
  static constexpr auto DOUBLE_EDGE_PATCHES =
      std::to_array<std::bitset<4>>({0b1100, 0b0011, 0b1010, 0b0101});
  Vector2i location_min{std::numeric_limits<int>::max(),
                        std::numeric_limits<int>::max()};
  Vector2i location_max{std::numeric_limits<int>::min(),
                        std::numeric_limits<int>::min()};
  for (const auto& location : locations) {
    location_min = {std::min(location_min.x, location.x),
                    std::min(location_min.y, location.y)};
    location_max = {std::max(location_max.x, location.x),
                    std::max(location_max.y, location.y)};
  }
  auto count = 0;
  for (auto y = location_min.y - 1; y < location_max.y + 1; y++) {
    for (auto x = location_min.x - 1; x < location_max.x + 1; x++) {
      const auto patch = get_patch(locations, {x, y});
      count += std::ranges::count(DOUBLE_EDGE_PATCHES, patch);
    }
  }
  return count;
}

int main() {
  const Grid grid{std::cin};

  std::map<Vector2i, std::set<Vector2i>> regions;
  for (auto grid_it = grid.cbegin(); grid_it != grid.cend(); ++grid_it) {
    const auto locations = find_connected_locations(grid, grid_it);
    regions[std::ranges::min(locations)] = locations;
  }

  auto price = 0;
  auto discount_price = 0;
  for (const auto& region : regions) {
    const auto& locations = region.second;
    const auto area = static_cast<int>(locations.size());
    const auto perimeter = calc_perimeter(locations);
    const auto n_edges = perimeter - count_double_edges(locations);
    price += area * perimeter;
    discount_price += area * n_edges;
  }
  std::cout << price << "\n";
  std::cout << discount_price << "\n";
}
