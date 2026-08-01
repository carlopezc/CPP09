#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <deque>
#include <vector>

class PmergeMe {
private:
  std::vector<int> _vec;
  std::deque<int> _deq;

  void sortVector(std::vector<int> &vec);
  void sortDeque(std::deque<int> &deq);
  void parseInput(int argc, char **argv);

  std::vector<std::pair<int, int> > generatePairs(std::vector<int> &vec);
  std::deque<std::pair<int, int> > generatePairs(std::deque<int> &deq);
  
  std::vector<int> realignPend(const std::vector<int> &main_pairs, const std::vector<std::pair<int, int> > &pairs);
  std::deque<int> realignPend(const std::deque<int> &main_pairs, const std::deque<std::pair<int, int> > &pairs);

  std::vector<size_t> generateJacobs(size_t limit);

  std::vector<size_t> generateInsertOrder(const std::vector<size_t> &jacob, size_t size);

  void insertPend(std::vector<int> &vec, const std::vector<int> &main_pairs, const std::vector<int> &sorted_pend, const std::vector<size_t> &insert_order);
  void insertPend(std::deque<int> &deq, const std::deque<int> &main_pairs, const std::deque<int> &sorted_pend, const std::vector<size_t> &insert_order);

public:
  PmergeMe(int argc, char **argv);
  ~PmergeMe();
  PmergeMe(const PmergeMe &other);
  PmergeMe &operator=(const PmergeMe &other);
  void run();
};

#endif