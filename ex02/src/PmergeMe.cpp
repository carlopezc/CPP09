#include "PmergeMe.hpp"
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <utility>

PmergeMe::PmergeMe(int argc, char **argv) { parseInput(argc, argv); }

PmergeMe::PmergeMe(const PmergeMe &other) {
  _vec = other._vec;
  _deq = other._deq;
}

PmergeMe::~PmergeMe() {}

PmergeMe &PmergeMe::operator=(const PmergeMe &other) {
  if (this == &other) {
    return *this;
  }
  _vec = other._vec;
  _deq = other._deq;
  return *this;
}

void PmergeMe::parseInput(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    char *end;
    long num = std::strtol(argv[i], &end, 10);
    if (*end != '\0' || num <= 0 || num > INT_MAX) {
      throw std::runtime_error("Invalid input");
    }
    _vec.push_back(num);
    _deq.push_back(num);
  }
}

size_t binarySearch(const std::vector<int> &vec, int value, size_t high) {
  size_t left = 0;
  size_t right = high;

  while (left < right) {
    size_t mid = (left + right) / 2;
    if (value < vec[mid]) {
      right = mid;
    } else {
      left = mid + 1;
    }
  }
  return left;
}

size_t binarySearch(const std::deque<int> &deq, int value, size_t high) {
  size_t left = 0;
  size_t right = high;

  while (left < right) {
    size_t mid = (left + right) / 2;
    if (value < deq[mid]) {
      right = mid;
    } else {
      left = mid + 1;
    }
  }
  return left;
}

std::vector<std::pair<int, int> >
PmergeMe::generatePairs(std::vector<int> &vec) {
  std::vector<std::pair<int, int> > pairs;
  for (size_t i = 0; i < vec.size(); i += 2) {
    int bigger = std::max(vec[i], vec[i + 1]);
    int smaller = std::min(vec[i], vec[i + 1]);
    pairs.push_back(std::make_pair(bigger, smaller));
  }
  return pairs;
}

std::deque<std::pair<int, int> > PmergeMe::generatePairs(std::deque<int> &deq) {
  std::deque<std::pair<int, int> > pairs;
  for (size_t i = 0; i < deq.size(); i += 2) {
    int bigger = std::max(deq[i], deq[i + 1]);
    int smaller = std::min(deq[i], deq[i + 1]);
    pairs.push_back(std::make_pair(bigger, smaller));
  }
  return pairs;
}

std::vector<int>
PmergeMe::realignPend(const std::vector<int> &main_pairs,
                      const std::vector<std::pair<int, int> > &pairs) {
  std::vector<int> sorted_pend;
  for (size_t i = 0; i < main_pairs.size(); i++) {
    for (size_t j = 0; j < pairs.size(); j++) {
      if (pairs[j].first == main_pairs[i]) {
        sorted_pend.push_back(pairs[j].second);
        break;
      }
    }
  }
  return sorted_pend;
}

std::deque<int>
PmergeMe::realignPend(const std::deque<int> &main_pairs,
                      const std::deque<std::pair<int, int> > &pairs) {
  std::deque<int> sorted_pend;
  for (size_t i = 0; i < main_pairs.size(); i++) {
    for (size_t j = 0; j < pairs.size(); j++) {
      if (pairs[j].first == main_pairs[i]) {
        sorted_pend.push_back(pairs[j].second);
        break;
      }
    }
  }
  return sorted_pend;
}

std::vector<size_t> PmergeMe::generateJacobs(size_t limit) {
  std::vector<size_t> jacob;
  jacob.push_back(1);
  jacob.push_back(3);
  while (jacob.back() < limit) {
    size_t next = jacob[jacob.size() - 1] + 2 * jacob[jacob.size() - 2];
    jacob.push_back(next);
  }
  return jacob;
}

std::vector<size_t>
PmergeMe::generateInsertOrder(const std::vector<size_t> &jacob, size_t size) {
  std::vector<size_t> insert_order;
  size_t last = 1;
  for (size_t i = 1; i < jacob.size(); i++) {
    size_t target = jacob[i];
    if (target > size) {
      target = size;
    }
    for (size_t idx = target; idx > last; idx--) {
      insert_order.push_back(idx - 1);
    }
    last = target;
  }
  return insert_order;
}

void PmergeMe::insertPend(std::vector<int> &vec,
                          const std::vector<int> &main_pairs,
                          const std::vector<int> &sorted_pend,
                          const std::vector<size_t> &insert_order) {
  for (size_t i = 0; i < insert_order.size(); i++) {
    size_t idx = insert_order[i];
    int val = sorted_pend[idx];

    std::vector<int>::iterator it =
        std::find(vec.begin(), vec.end(), main_pairs[idx]);
    size_t limit = std::distance(vec.begin(), it);

    size_t pos = binarySearch(vec, val, limit);
    vec.insert(vec.begin() + pos, val);
  }
}

void PmergeMe::insertPend(std::deque<int> &deq,
                          const std::deque<int> &main_pairs,
                          const std::deque<int> &sorted_pend,
                          const std::vector<size_t> &insert_order) {
  for (size_t i = 0; i < insert_order.size(); i++) {
    size_t idx = insert_order[i];
    int val = sorted_pend[idx];

    std::deque<int>::iterator it =
        std::find(deq.begin(), deq.end(), main_pairs[idx]);
    size_t limit = std::distance(deq.begin(), it);

    size_t pos = binarySearch(deq, val, limit);
    deq.insert(deq.begin() + pos, val);
  }
}

void PmergeMe::sortVector(std::vector<int> &vec) {
  if (vec.size() <= 1) {
    return;
  }

  int leftoverVal = -1;
  bool hasLeftover = false;
  if (vec.size() % 2 != 0) {
    leftoverVal = vec.back();
    vec.pop_back();
    hasLeftover = true;
  }

  std::vector<std::pair<int, int> > pairs = generatePairs(vec);

  std::vector<int> main_pairs;
  for (size_t i = 0; i < pairs.size(); i++) {
    main_pairs.push_back(pairs[i].first);
  }

  sortVector(main_pairs);

  std::vector<int> sorted_pend = realignPend(main_pairs, pairs);

  vec.clear();
  vec.push_back(sorted_pend[0]);
  for (size_t i = 0; i < main_pairs.size(); i++) {
    vec.push_back(main_pairs[i]);
  }

  std::vector<size_t> jacob = generateJacobs(sorted_pend.size());
  std::vector<size_t> insert_order =
      generateInsertOrder(jacob, sorted_pend.size());

  insertPend(vec, main_pairs, sorted_pend, insert_order);

  if (hasLeftover) {
    size_t pos = binarySearch(vec, leftoverVal, vec.size());
    vec.insert(vec.begin() + pos, leftoverVal);
  }
}

void PmergeMe::sortDeque(std::deque<int> &deq) {
  if (deq.size() <= 1) {
    return;
  }

  int leftoverVal = -1;
  bool hasLeftover = false;
  if (deq.size() % 2 != 0) {
    leftoverVal = deq.back();
    deq.pop_back();
    hasLeftover = true;
  }

  std::deque<std::pair<int, int> > pairs = generatePairs(deq);

  std::deque<int> main_pairs;
  for (size_t i = 0; i < pairs.size(); i++) {
    main_pairs.push_back(pairs[i].first);
  }

  sortDeque(main_pairs);

  std::deque<int> sorted_pend = realignPend(main_pairs, pairs);

  deq.clear();
  deq.push_back(sorted_pend[0]);
  for (size_t i = 0; i < main_pairs.size(); i++) {
    deq.push_back(main_pairs[i]);
  }

  std::vector<size_t> jacob = generateJacobs(sorted_pend.size());
  std::vector<size_t> insert_order =
      generateInsertOrder(jacob, sorted_pend.size());

  insertPend(deq, main_pairs, sorted_pend, insert_order);

  if (hasLeftover) {
    size_t pos = binarySearch(deq, leftoverVal, deq.size());
    deq.insert(deq.begin() + pos, leftoverVal);
  }
}

void PmergeMe::run() {
  std::cout << "Before: ";
  for (size_t i = 0; i < _vec.size(); i++) {
    std::cout << _vec[i] << (i + 1 == _vec.size() ? "" : " ");
  }
  std::cout << std::endl;

  std::clock_t startVec = std::clock();
  sortVector(_vec);
  std::clock_t endVec = std::clock();
  double timeVec =
      static_cast<double>(endVec - startVec) / CLOCKS_PER_SEC * 1e6;

  std::clock_t startDeq = std::clock();
  sortDeque(_deq);
  std::clock_t endDeq = std::clock();
  double timeDeq =
      static_cast<double>(endDeq - startDeq) / CLOCKS_PER_SEC * 1e6;

  std::cout << "After:  ";
  for (size_t i = 0; i < _vec.size(); i++) {
    std::cout << _vec[i] << (i + 1 == _vec.size() ? "" : " ");
  }
  std::cout << std::endl;

  std::cout << "Time to process a range of " << _vec.size()
            << " elements with std::vector : " << timeVec << " us" << std::endl;
  std::cout << "Time to process a range of " << _deq.size()
            << " elements with std::deque  : " << timeDeq << " us" << std::endl;
}
