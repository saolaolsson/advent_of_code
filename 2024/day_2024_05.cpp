// https://adventofcode.com/2024/day/5

#include "shared.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <map>
#include <optional>
#include <sstream>
#include <vector>

static std::tuple<std::map<int, std::vector<int>>,
                  std::vector<std::vector<int>>>
extract_ordering_updates(std::istream& istream) {
  std::map<int, std::vector<int>> ordering;
  std::vector<std::vector<int>> updates;
  for (auto line_it = std::istream_iterator<Line>{istream};
       line_it != std::istream_iterator<Line>{}; ++line_it) {
    std::string line;
    std::ranges::transform(
        std::string{*line_it}, std::back_inserter(line),
        [](const char c) { return std::isdigit(c) ? c : ' '; });
    std::stringstream line_ss{line};
    std::vector<int> values;
    std::copy(std::istream_iterator<int>{line_ss}, {},
              std::back_inserter(values));
    if (values.size() == 2) {
      ordering[values[0]].push_back(values[1]);
    } else if (values.size() > 2) {
      updates.push_back(values);
    }
  }
  return {ordering, updates};
}

static std::optional<std::pair<std::vector<int>::const_iterator,
                               std::vector<int>::const_iterator>>
find_bad_page_pair(const std::map<int, std::vector<int>>& ordering,
                   const std::vector<int>& update) {
  for (auto update_page_it = update.cbegin(); update_page_it != update.cend();
       ++update_page_it) {
    const auto ordering_it = ordering.find(*update_page_it);
    if (ordering_it == ordering.cend()) {
      // no page ordering rules for current page
      continue;
    }
    for (const auto after_page : ordering_it->second) {
      const auto update_page_after_it =
          std::find(update.cbegin(), update.cend(), after_page);
      if (update_page_after_it == update.cend()) {
        // ordering rule specifies page not in update
        continue;
      }
      if (update_page_after_it < update_page_it) {
        // ordering rule specifies page that is before current page in update
        return {{update_page_it, update_page_after_it}};
      }
    }
  }
  // no ordering rule violations found
  return {};
}

static bool validate_update(const std::map<int, std::vector<int>>& ordering,
                            const std::vector<int>& update) {
  return !find_bad_page_pair(ordering, update);
}

static std::vector<int> reorder_update(
    const std::map<int, std::vector<int>>& ordering,
    const std::vector<int>& bad_update) {
  auto update = bad_update;
  while (const auto& bad_page_pair = find_bad_page_pair(ordering, update)) {
    // can't use const_iterators to swap
    auto& first = *(update.begin() +
                    std::distance(update.cbegin(), bad_page_pair->first));
    auto& second = *(update.begin() +
                     std::distance(update.cbegin(), bad_page_pair->second));
    std::swap(first, second);
  }
  return update;
}

int main() {
  const auto [ordering, updates] = extract_ordering_updates(std::cin);

  auto mid_page_sum = 0;
  auto reordered_mid_page_sum = 0;
  for (const auto& update : updates) {
    if (validate_update(ordering, update)) {
      const auto mid_page = update[update.size() / 2];
      mid_page_sum += mid_page;
    } else {
      auto reordered_update = reorder_update(ordering, update);
      const auto mid_page = reordered_update[update.size() / 2];
      reordered_mid_page_sum += mid_page;
    }
  }
  std::cout << mid_page_sum << "\n";
  std::cout << reordered_mid_page_sum << "\n";
}
