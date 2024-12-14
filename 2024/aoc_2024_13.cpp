// https://adventofcode.com/2024/day/13

#include "shared.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

struct Machine {
  int a_dx;
  int a_dy;
  int b_dx;
  int b_dy;
  int x;
  int y;
};

static std::int64_t find_min_cost(const std::vector<Machine>& machines,
                                  const std::int64_t offset) {
  std::int64_t cost = 0;
  for (const auto& machine : machines) {
    const auto x = machine.x + offset;
    const auto y = machine.y + offset;

    // solution is within period of a_init
    const auto period_x = std::max(machine.a_dx, machine.b_dx) /
                          std::gcd(machine.a_dx, machine.b_dx);
    const auto period_y = std::max(machine.a_dy, machine.b_dy) /
                          std::gcd(machine.a_dy, machine.b_dy);
    const auto period = std::min(period_x, period_y);
    const auto a_init =
        (x * machine.b_dy - y * machine.b_dx) /
        (machine.a_dx * machine.b_dy - machine.a_dy * machine.b_dx);

    for (auto a = a_init; a < a_init + period; a++) {
      const auto b = (x - a * machine.a_dx) / machine.b_dx;
      const auto valid_solution = a * machine.a_dx + b * machine.b_dx == x &&
                                  a * machine.a_dy + b * machine.b_dy == y;
      if (valid_solution) {
        static constexpr auto COST_A = 3;
        static constexpr auto COST_B = 1;
        cost += a * COST_A + b * COST_B;
        break;
      }
    }
  }
  return cost;
}

int main() {
  const auto values = extract_values(std::cin);
  std::vector<Machine> machines;
  for (const auto& v : values | std::views::chunk(6)) {
    machines.emplace_back(v[0], v[1], v[2], v[3], v[4], v[5]);
  }
  std::cout << find_min_cost(machines, 0) << "\n";
  std::cout << find_min_cost(machines, 10'000'000'000'000) << "\n";
}
