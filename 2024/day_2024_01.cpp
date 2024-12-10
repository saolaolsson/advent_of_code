// https://adventofcode.com/2024/day/1

#include <algorithm>
#include <array>
#include <iostream>
#include <ranges>
#include <vector>

// Back insert iterator to an array (of size N_ARRAYS) whose elements are
// vectors of T. Assigned values are added to the vectors in array0, array1,
// ..., arrayN_ARRAYS, array0, array1, ..., in sequence. Partial but working
// iterator implementation.
template <typename T, std::size_t N_ARRAYS>
struct ArrayOfVectorsBackInserter {
  std::array<std::vector<T>, N_ARRAYS>& value_vectors;
  std::size_t array_index = 0;

  ArrayOfVectorsBackInserter& operator=(const int i) {
    array_index = (array_index + 1) % N_ARRAYS;
    value_vectors[array_index].push_back(i);
    return *this;
  }
  ArrayOfVectorsBackInserter& operator++() { return *this; }
  ArrayOfVectorsBackInserter& operator*() { return *this; }
};

static int calc_distance_sum(std::vector<int> left_values,
                             std::vector<int> right_values) {
  std::ranges::sort(left_values);
  std::ranges::sort(right_values);
  const auto sum_tuple_distance = [](auto sum,
                                     const std::tuple<int&, int&>& ints) {
    const auto distance = std::abs(std::get<1>(ints) - std::get<0>(ints));
    return sum + distance;
  };
  return std::ranges::fold_left(
      std::ranges::zip_view(left_values, right_values), 0, sum_tuple_distance);
}

static int calc_similiarity_score(const std::vector<int>& left_values,
                                  const std::vector<int>& right_values) {
  auto score = 0;
  for (const auto left_value : left_values) {
    score += left_value * std::ranges::count(right_values, left_value);
  }
  return score;
}

int main() {
  std::array<std::vector<int>, 2> values;
  std::copy(std::istream_iterator<int>{std::cin}, {},
            ArrayOfVectorsBackInserter{values});
  std::cout << calc_distance_sum(values[0], values[1]) << "\n";
  std::cout << calc_similiarity_score(values[0], values[1]) << "\n";
}
