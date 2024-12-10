// https://adventofcode.com/2024/day/10

#include "shared.hpp"

#include <iostream>
#include <set>
#include <stack>
#include <utility>

std::pair<int, int> score_trail(const Grid& grid,
                                const Vector2i& from_location) {
  const auto is_trailhead = grid.location(from_location) == '0';
  if (!is_trailhead) {
    return {};
  }

  std::set<Vector2i> trailtails;
  auto n_trails = 0;
  std::stack<Vector2i> location_to_walk{{from_location}};
  while (!location_to_walk.empty()) {
    const auto location = location_to_walk.top();
    location_to_walk.pop();

    const auto height = grid.location(location);
    const auto is_trailtail = height == '9';
    if (is_trailtail) {
      trailtails.insert(location);
      n_trails++;
    } else {
      for (const auto& direction : Grid::CARDINAL_DIRECTIONS) {
        const auto maybe_next_location = location + direction;
        const auto is_valid_next_location =
            grid.is_inside(maybe_next_location) &&
            grid.location(maybe_next_location) == height + 1;
        if (is_valid_next_location) {
          location_to_walk.push(maybe_next_location);
        }
      }
    }
  }
  return {trailtails.size(), n_trails};
}

int main() {
  const Grid grid{std::cin};
  auto tailtrail_score = 0;
  auto trail_score = 0;
  for (auto grid_it = grid.cbegin(); grid_it != grid.cend(); ++grid_it) {
    const auto [tts, ts] = score_trail(grid, grid_it);
    tailtrail_score += tts;
    trail_score += ts;
  }
  std::cout << tailtrail_score << "\n";
  std::cout << trail_score << "\n";
}
