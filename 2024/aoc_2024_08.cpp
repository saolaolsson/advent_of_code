// https://adventofcode.com/2024/day/8

#include "shared.hpp"

#include <cctype>
#include <iostream>
#include <map>
#include <ranges>
#include <set>
#include <vector>

int main() {
  const Grid grid{std::cin};
  std::map<char, std::vector<Vector2i>> all_freq_antennae;
  for (auto grid_it = grid.cbegin(); grid_it != grid.cend(); ++grid_it) {
    if (std::isalnum(*grid_it)) {
      all_freq_antennae[*grid_it].push_back(grid_it);
    }
  }

  std::set<Vector2i> antinodes;
  std::set<Vector2i> multi_antinodes;
  for (const auto& single_freq_antennae : all_freq_antennae) {
    const auto& anntennae_locations = single_freq_antennae.second;
    for (const auto& [antenna_location_a, antenna_location_b] :
         std::views::cartesian_product(anntennae_locations,
                                       anntennae_locations)) {
      if (antenna_location_a == antenna_location_b) {
        continue;
      }
      auto antinode_location = antenna_location_a;
      const auto antenna_location_delta =
          antenna_location_b - antenna_location_a;

      if (grid.is_inside(antinode_location - antenna_location_delta)) {
        antinodes.insert(antinode_location - antenna_location_delta);
      }

      while (grid.is_inside(antinode_location)) {
        multi_antinodes.insert(antinode_location);
        antinode_location -= antenna_location_delta;
      }
    }
  }
  std::cout << antinodes.size() << "\n";
  std::cout << multi_antinodes.size() << "\n";
}
