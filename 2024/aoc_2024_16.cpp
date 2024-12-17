// https://adventofcode.com/2024/day/16

#include "shared.hpp"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <limits>
#include <set>
#include <stack>
#include <vector>

struct Pose {
  Vector2i location;
  std::size_t direction_index;
  int cost;
  std::vector<Vector2i> locations;
};

static constexpr auto COST_FORWARD = 1;
static constexpr auto COST_TURN = 1000;

static Pose walk_forward(const Pose& pose) {
  return {pose.location + Grid::CARDINAL_DIRECTIONS[pose.direction_index],
          pose.direction_index, pose.cost + COST_FORWARD, pose.locations};
}

static Pose walk_left(const Pose& pose) {
  auto direction_index = pose.direction_index == 0
                             ? Grid::CardinalDirection::END - 1
                             : pose.direction_index - 1;
  return {pose.location + Grid::CARDINAL_DIRECTIONS[direction_index],
          direction_index, pose.cost + COST_TURN + COST_FORWARD,
          pose.locations};
}

static Pose walk_right(const Pose& pose) {
  auto direction_index =
      (pose.direction_index + 1) % Grid::CardinalDirection::END;
  return {pose.location + Grid::CARDINAL_DIRECTIONS[direction_index],
          direction_index, pose.cost + COST_TURN + COST_FORWARD,
          pose.locations};
}

static bool is_wall(const Grid& grid, const Vector2i& location) {
  return grid.location(location) == '#';
}

static bool is_end(const Grid& grid, const Vector2i& location) {
  return grid.location(location) == 'E';
}

int main() {
  Grid grid{std::cin};
  std::stack<Pose> poses;
  poses.emplace(std::find(grid.cbegin(), grid.cend(), 'S'),
                Grid::CardinalDirection::EAST, 0);
  Matrix<int> min_costs{grid.size(), -1};
  auto min_cost = std::numeric_limits<int>::max();
  std::set<Vector2i> min_locations;
  while (!poses.empty()) {
    auto pose = poses.top();
    pose.locations.push_back(pose.location);
    poses.pop();

    if (is_wall(grid, pose.location)) {
      continue;
    }

    const auto is_costlier_route =
        pose.cost > min_cost ||
        (min_costs.location(pose.location) != -1 &&
         pose.cost - min_costs.location(pose.location) > COST_TURN);
    if (is_costlier_route) {
      continue;
    }
    min_costs.location(pose.location) = pose.cost;

    if (is_end(grid, pose.location)) {
      if (pose.cost < min_cost) {
        min_locations.clear();
        min_cost = pose.cost;
      }
      if (pose.cost == min_cost) {
        std::copy(pose.locations.cbegin(), pose.locations.cend(),
                  std::inserter(min_locations, min_locations.cend()));
      }
      continue;
    }

    for (const auto& walk : {walk_left, walk_right, walk_forward}) {
      poses.push(walk(pose));
    }
  }
  std::cout << min_cost << "\n";
  std::cout << min_locations.size() << "\n";
}
