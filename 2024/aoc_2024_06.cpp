// https://adventofcode.com/2024/day/6

#include "shared.hpp"

#include <algorithm>
#include <iostream>
#include <optional>
#include <set>

struct Pose {
  Vector2i location;
  std::size_t direction_index;

  bool operator<(const Pose& rhs) const {
    if (location != rhs.location) {
      return location < rhs.location;
    } else {
      return direction_index < rhs.direction_index;
    }
  }
};

std::optional<Pose> step(const Grid& grid, const Pose& guard_pose,
                         const std::optional<Vector2i>& blocked_location) {
  auto new_direction_index = guard_pose.direction_index;
  while (true) {
    const auto new_location =
        guard_pose.location + Grid::CARDINAL_DIRECTIONS[new_direction_index];
    if (!grid.is_inside(new_location)) {
      return {};
    } else if (grid.location(new_location) == '#' ||
               new_location == blocked_location) {
      new_direction_index =
          (new_direction_index + 1) % Grid::CARDINAL_DIRECTIONS.size();
    } else {
      return {{new_location, new_direction_index}};
    }
  }
}

std::set<Pose> walk(const Grid& grid, const Pose& pose,
                    const std::optional<Vector2i>& blocked_location) {
  std::set<Pose> walked_poses;
  std::optional<Pose> maybe_pose{pose};
  while (maybe_pose) {
    const auto is_loop = walked_poses.contains(*maybe_pose);
    if (is_loop) {
      return {};
    }
    walked_poses.insert(*maybe_pose);
    maybe_pose = step(grid, *maybe_pose, blocked_location);
  }
  return walked_poses;
}

int main() {
  const Grid grid{std::cin};
  const auto guard_pose = Pose{std::find(grid.cbegin(), grid.cend(), '^'), {}};

  const auto walked_poses = walk(grid, guard_pose, {});
  std::set<Vector2i> walked_locations;
  for (const auto& pose : walked_poses) {
    walked_locations.insert(pose.location);
  }
  std::cout << walked_locations.size() << "\n";

  std::set<Vector2i> looping_block_locations;
  for (const auto& blocked_location : walked_locations) {
    if (walk(grid, guard_pose, blocked_location).empty()) {
      looping_block_locations.insert(blocked_location);
    }
  }
  std::cout << looping_block_locations.size() << "\n";
}
