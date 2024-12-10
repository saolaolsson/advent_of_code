// https://adventofcode.com/2024/day/9

#include "shared.hpp"

#include <algorithm>
#include <iostream>
#include <istream>
#include <iterator>
#include <vector>

static std::vector<int> extract_disk_map(std::istream& istream) {
  std::vector<int> disk_map;
  auto file_id = 0;
  auto char_it = std::istream_iterator<char>{istream};
  bool is_file = true;
  while (char_it != std::istream_iterator<char>{}) {
    const auto count = *char_it++ - '0';
    const auto value = is_file ? file_id++ : -1;
    std::fill_n(std::back_inserter(disk_map), count, value);
    is_file = !is_file;
  }
  return disk_map;
}

static bool is_free_block(const int file_id) { return file_id == -1; };

static std::vector<int> defragment_free_blocks(
    const std::vector<int>& disk_map) {
  std::vector<int> defragged_disk_map(disk_map.size(), -1);
  auto block_it = disk_map.cbegin();
  auto defrag_block_it = disk_map.crbegin();
  auto defragged_block_it = defragged_disk_map.begin();
  while (std::vector<int>::const_reverse_iterator{block_it} > defrag_block_it) {
    const auto file_id = *block_it++;
    if (is_free_block(file_id)) {
      defrag_block_it =
          std::find_if_not(defrag_block_it, disk_map.crend(), is_free_block);
      *defragged_block_it++ = *defrag_block_it++;
    } else {
      *defragged_block_it++ = file_id;
    }
  }
  return defragged_disk_map;
}

static std::optional<std::pair<std::vector<int>::reverse_iterator,
                               std::vector<int>::reverse_iterator>>
find_defrag_file(const std::vector<int>::reverse_iterator& rbegin,
                 const std::vector<int>::reverse_iterator& rend) {
  const auto file_rbegin = std::find_if_not(rbegin, rend, is_free_block);
  const auto block_file_id = *file_rbegin;
  const auto file_rend = std::find_if(
      file_rbegin, rend,
      [block_file_id](const auto file_id) { return file_id != block_file_id; });
  if (file_rbegin == file_rend) {
    return {};
  } else {
    return {{file_rbegin, file_rend}};
  }
}

static std::vector<int>::iterator find_first_free_block(
    const std::vector<int>::iterator& begin,
    const std::vector<int>::iterator& end, const int min_size) {
  auto free_begin = begin;
  while ((free_begin = std::find_if(free_begin, end, is_free_block)) != end) {
    const auto free_end = std::find_if_not(free_begin, end, is_free_block);
    if (std::distance(free_begin, free_end) >= min_size) {
      return free_begin;
    }
    free_begin = free_end;
  }
  return end;
}

static std::vector<int> defragment_files(std::vector<int> disk_map) {
  auto defrag_file_it = disk_map.rbegin();
  while (const auto defrag_file =
             find_defrag_file(defrag_file_it, disk_map.rend())) {
    const auto [defrag_file_rbegin, defrag_file_rend] = *defrag_file;
    const auto file_size = std::distance(defrag_file_rbegin, defrag_file_rend);
    const auto free_block_it =
        find_first_free_block(disk_map.begin(), disk_map.end(), file_size);
    const auto found_free_space_left_of_file =
        free_block_it != disk_map.end() &&
        std::vector<int>::const_reverse_iterator{free_block_it} >
            defrag_file_rbegin;
    if (found_free_space_left_of_file) {
      const auto file_id = *defrag_file_rbegin;
      std::fill_n(free_block_it, file_size, file_id);
      std::fill_n(defrag_file_rbegin, file_size, -1);
    }
    defrag_file_it = defrag_file_rend;
  }
  return disk_map;
}

static std::int64_t calc_checksum(const std::vector<int>& disk_map) {
  auto block_index = 0;
  const auto checksum = [&block_index](auto sum, const int id) {
    const auto block_checksum = id >= 0 ? block_index * id : 0;
    block_index++;
    return sum + block_checksum;
  };
  return std::ranges::fold_left(disk_map, std::int64_t{}, checksum);
}

int main() {
  const auto disk_map = extract_disk_map(std::cin);
  std::cout << calc_checksum(defragment_free_blocks(disk_map)) << "\n";
  std::cout << calc_checksum(defragment_files(disk_map)) << "\n";
}
