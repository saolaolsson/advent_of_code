// https://adventofcode.com/2024/day/17

#include "shared.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <iostream>
#include <ranges>
#include <vector>

struct State {
  std::vector<std::uint64_t> registers;
  std::size_t instruction_pointer{0};
};

enum Register { A, B, C, END };

struct Result {
  State state;
  std::optional<std::uint64_t> output;
};

static std::uint64_t decode_combo_operand(const State& state,
                                          const std::uint64_t operand) {
  switch (operand) {
    case 0:
    case 1:
    case 2:
    case 3:
      return operand;
    case 4:
    case 5:
    case 6:
      return state.registers[static_cast<std::size_t>(operand - 4)];
  }
  return 0;
}

static std::uint64_t denominator(const std::uint64_t decoded_operand) {
  return decoded_operand == 0 ? std::uint64_t{1}
                              : std::uint64_t{2} << (decoded_operand - 1);
}

static Result opcode_adv(const State& state, const std::uint64_t operand) {
  auto new_state = state;
  new_state.registers[Register::A] =
      state.registers[Register::A] /
      denominator(decode_combo_operand(state, operand));
  new_state.instruction_pointer += 2;
  return {new_state, {}};
}

static Result opcode_bxl(const State& state, const std::uint64_t operand) {
  auto new_state = state;
  new_state.registers[Register::B] = state.registers[Register::B] ^ operand;
  new_state.instruction_pointer += 2;
  return {new_state, {}};
}

static Result opcode_bst(const State& state, const std::uint64_t operand) {
  auto new_state = state;
  new_state.registers[Register::B] = decode_combo_operand(state, operand) % 8;
  new_state.instruction_pointer += 2;
  return {new_state, {}};
}

static Result opcode_jnz(const State& state, const std::uint64_t operand) {
  auto new_state = state;
  if (state.registers[Register::A] == 0) {
    new_state.instruction_pointer += 2;
    return {new_state, {}};
  }
  new_state.instruction_pointer = static_cast<std::size_t>(operand);
  return {new_state, {}};
}

static Result opcode_bxc(const State& state, const std::uint64_t) {
  auto new_state = state;
  new_state.registers[Register::B] =
      state.registers[Register::B] ^ state.registers[Register::C];
  new_state.instruction_pointer += 2;
  return {new_state, {}};
}

static Result opcode_out(const State& state, const std::uint64_t operand) {
  auto new_state = state;
  new_state.instruction_pointer += 2;
  return {new_state, decode_combo_operand(state, operand) % 8};
}

static Result opcode_bdv(const State& state, const std::uint64_t operand) {
  auto new_state = state;
  new_state.registers[Register::B] =
      state.registers[Register::A] /
      denominator(decode_combo_operand(state, operand));
  new_state.instruction_pointer += 2;
  return {new_state, {}};
}

Result opcode_cdv(const State& state, const std::uint64_t operand) {
  auto new_state = state;
  new_state.registers[Register::C] =
      state.registers[Register::A] /
      denominator(decode_combo_operand(state, operand));
  new_state.instruction_pointer += 2;
  return {new_state, {}};
}

std::vector<std::uint64_t> run_program(
    const State& initial_state, const std::vector<std::uint64_t>& program) {
  static const auto opcode_functions =
      std::to_array<std::function<Result(const State&, std::uint64_t)>>({
          opcode_adv,
          opcode_bxl,
          opcode_bst,
          opcode_jnz,
          opcode_bxc,
          opcode_out,
          opcode_bdv,
          opcode_cdv,
      });
  std::vector<std::uint64_t> outputs;
  auto state = initial_state;
  while (state.instruction_pointer < program.size()) {
    const auto opcode = program[state.instruction_pointer + 0];
    const auto operand = program[state.instruction_pointer + 1];
    const auto [new_state, output] =
        opcode_functions[static_cast<std::size_t>(opcode)](state, operand);
    state = new_state;
    if (output) {
      outputs.push_back(*output);
    }
  }
  return outputs;
}

void find_output_program(const State& initial_state,
                         const std::vector<std::uint64_t>& program,
                         const std::uint64_t initial_a,
                         const std::uint64_t delta_a,
                         const std::size_t n_outputs_check) {
  auto state = initial_state;
  state.registers[Register::A] = initial_a;
  while (true) {
    const auto outputs = run_program(state, program);

    const auto first_n_outputs_equal =
        (outputs | std::views::take(n_outputs_check) |
         std::ranges::to<std::vector<std::uint64_t>>()) ==
        (program | std::views::take(n_outputs_check) |
         std::ranges::to<std::vector<std::uint64_t>>());
    if (first_n_outputs_equal) {
      static auto last_register_a = state.registers[Register::A];
      std::cout << state.registers[Register::A] << " "
                << state.registers[Register::A] - last_register_a << "\n";
      last_register_a = state.registers[Register::A];
    }
    if (outputs == program) {
      std::cout << state.registers[Register::A] << "\n";
      break;
    }
    state.registers[Register::A] += delta_a;
  }
}

int main() {
  const auto values = extract_values(std::cin);
  const State state{values | std::views::take(Register::END) |
                    std::ranges::to<std::vector<std::uint64_t>>()};
  const auto program = values | std::views::drop(Register::END) |
                       std::ranges::to<std::vector<std::uint64_t>>();

  const auto outputs = run_program(state, program);
  std::ranges::copy(outputs, std::ostream_iterator<int>(std::cout, ","));
  std::cout << "\n";

  /*
  output to match program: 2,4,1,7,7,5,4,1,1,4,5,5,0,3,3,0

  2,4: bst A: B = A % 8
  1,7: bxl 7: B = B ^ 7
  7,5: cdv B: C = A / B
  4,1: bxc -: B = B ^ C
  1,4: bxl 4: B = B ^ 4
  5,5: out B: print B % 8
  0,3: adv 3: A = A / 8
  3,0: jnz 0: goto start

  B = (A % 8) ^ 7
  B = (B ^ (A / B)) ^ 4
  B = ((A % 8) ^ 7 ^ (A / (A % 8) ^ 7)) ^ 4
  D = (((A % 8) ^ 7 ^ (A / (A % 8) ^ 7)) ^ 4) % 8
  */

  // brute forced part 2 solution (three tests, about ~1 minute total time)

  // minimum A register content for 16 integer output
  // find_output_program(state, program, std::uint64_t{1} << (15 * 3), 1, 6);

  // result suggests new start and delta for register A
  // find_output_program(state, program, 35184375080160, 4194304, 7);

  // result suggests new start and delta for register A
  find_output_program(state, program, 35184391857376, 33554432, 16);
}
