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

// Procesa y valida los argumentos de entrada de la línea de comandos
void PmergeMe::parseInput(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    char *end;
    long num = std::strtol(argv[i], &end, 10);
    // Valida que no queden caracteres residuales, que sea positivo y que no
    // supere INT_MAX
    if (*end != '\0' || num <= 0 || num > INT_MAX) {
      throw std::runtime_error("Invalid input");
    }
    _vec.push_back(num);
    _deq.push_back(num);
  }
}

// Búsqueda binaria clásica en std::vector
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

// Búsqueda binaria clásica en std::deque
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

// Genera parejas ordenadas a partir de un std::vector
std::vector<std::pair<int, int>>
PmergeMe::generatePairs(std::vector<int> &vec) {
  std::vector<std::pair<int, int>> pairs;
  for (size_t i = 0; i < vec.size(); i += 2) {
    int bigger = std::max(vec[i], vec[i + 1]);
    int smaller = std::min(vec[i], vec[i + 1]);
    pairs.push_back(std::make_pair(bigger, smaller));
  }
  return pairs;
}

// Genera parejas ordenadas a partir de un std::deque
std::deque<std::pair<int, int>> PmergeMe::generatePairs(std::deque<int> &deq) {
  std::deque<std::pair<int, int>> pairs;
  for (size_t i = 0; i < deq.size(); i += 2) {
    int bigger = std::max(deq[i], deq[i + 1]);
    int smaller = std::min(deq[i], deq[i + 1]);
    pairs.push_back(std::make_pair(bigger, smaller));
  }
  return pairs;
}

// Realinea la cadena pend (elementos menores) para que coincida con el orden de
// la cadena principal ordenada
std::vector<int>
PmergeMe::realignPend(const std::vector<int> &main_pairs,
                      const std::vector<std::pair<int, int>> &pairs) {
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

// Realinea la cadena pend (elementos menores) para std::deque
std::deque<int>
PmergeMe::realignPend(const std::deque<int> &main_pairs,
                      const std::deque<std::pair<int, int>> &pairs) {
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

// Genera la secuencia de Jacobsthal: J(n) = J(n-1) + 2 * J(n-2)
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

// Determina el orden exacto de inserción a partir de los números de Jacobsthal.
// Por ejemplo, para el grupo 3, insertará desde el índice 3 hacia abajo hasta
// el anterior + 1.
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

// Realiza las inserciones binarias de la secuencia pend en la cadena final vec
// (std::vector)
void PmergeMe::insertPend(std::vector<int> &vec,
                          const std::vector<int> &main_pairs,
                          const std::vector<int> &sorted_pend,
                          const std::vector<size_t> &insert_order) {
  for (size_t i = 0; i < insert_order.size(); i++) {
    size_t idx = insert_order[i];
    int val = sorted_pend[idx];

    // Limita la búsqueda binaria hasta la posición actual de su pareja en el
    // main chain
    std::vector<int>::iterator it =
        std::find(vec.begin(), vec.end(), main_pairs[idx]);
    size_t limit = std::distance(vec.begin(), it);

    size_t pos = binarySearch(vec, val, limit);
    vec.insert(vec.begin() + pos, val);
  }
}

// Realiza las inserciones binarias de la secuencia pend en la cadena final deq
// (std::deque)
void PmergeMe::insertPend(std::deque<int> &deq,
                          const std::deque<int> &main_pairs,
                          const std::deque<int> &sorted_pend,
                          const std::vector<size_t> &insert_order) {
  for (size_t i = 0; i < insert_order.size(); i++) {
    size_t idx = insert_order[i];
    int val = sorted_pend[idx];

    // Limita la búsqueda binaria hasta la posición actual de su pareja en el
    // main chain
    std::deque<int>::iterator it =
        std::find(deq.begin(), deq.end(), main_pairs[idx]);
    size_t limit = std::distance(deq.begin(), it);

    size_t pos = binarySearch(deq, val, limit);
    deq.insert(deq.begin() + pos, val);
  }
}

// Algoritmo Ford-Johnson (Merge-Insertion Sort) para std::vector
void PmergeMe::sortVector(std::vector<int> &vec) {
  if (vec.size() <= 1) {
    return;
  }

  // Si el tamaño es impar, extraemos el último elemento temporalmente
  // (leftover)
  int leftoverVal = -1;
  bool hasLeftover = false;
  if (vec.size() % 2 != 0) {
    leftoverVal = vec.back();
    vec.pop_back();
    hasLeftover = true;
  }

  // Paso 1: Agrupar y ordenar parejas
  std::vector<std::pair<int, int>> pairs = generatePairs(vec);

  // Extraer cadena principal (mayores de cada pareja)
  std::vector<int> main_pairs;
  for (size_t i = 0; i < pairs.size(); i++) {
    main_pairs.push_back(pairs[i].first);
  }

  // Paso 2: Llamada recursiva sobre la cadena principal
  sortVector(main_pairs);

  // Paso 3 (Preparativo): Alinear los menores con los mayores ordenados
  std::vector<int> sorted_pend = realignPend(main_pairs, pairs);

  // Inicializar vec final con el primer elemento menor y la cadena principal
  // ordenada
  vec.clear();
  vec.push_back(sorted_pend[0]);
  for (size_t i = 0; i < main_pairs.size(); i++) {
    vec.push_back(main_pairs[i]);
  }

  // Generar secuencia de Jacobsthal y definir orden de inserciones
  std::vector<size_t> jacob = generateJacobs(sorted_pend.size());
  std::vector<size_t> insert_order =
      generateInsertOrder(jacob, sorted_pend.size());

  // Insertar de manera binaria optimizada
  insertPend(vec, main_pairs, sorted_pend, insert_order);

  // Paso final: Insertar el elemento sobrante (leftover) de forma binaria si
  // existía
  if (hasLeftover) {
    size_t pos = binarySearch(vec, leftoverVal, vec.size());
    vec.insert(vec.begin() + pos, leftoverVal);
  }
}

// Algoritmo Ford-Johnson (Merge-Insertion Sort) para std::deque
void PmergeMe::sortDeque(std::deque<int> &deq) {
  if (deq.size() <= 1) {
    return;
  }

  // Si el tamaño es impar, extraemos el último elemento temporalmente
  // (leftover)
  int leftoverVal = -1;
  bool hasLeftover = false;
  if (deq.size() % 2 != 0) {
    leftoverVal = deq.back();
    deq.pop_back();
    hasLeftover = true;
  }

  // Paso 1: Agrupar y ordenar parejas
  std::deque<std::pair<int, int>> pairs = generatePairs(deq);

  // Extraer cadena principal (mayores de cada pareja)
  std::deque<int> main_pairs;
  for (size_t i = 0; i < pairs.size(); i++) {
    main_pairs.push_back(pairs[i].first);
  }

  // Paso 2: Llamada recursiva sobre la cadena principal
  sortDeque(main_pairs);

  // Paso 3 (Preparativo): Alinear los menores con los mayores ordenados
  std::deque<int> sorted_pend = realignPend(main_pairs, pairs);

  // Inicializar deq final con el primer elemento menor y la cadena principal
  // ordenada
  deq.clear();
  deq.push_back(sorted_pend[0]);
  for (size_t i = 0; i < main_pairs.size(); i++) {
    deq.push_back(main_pairs[i]);
  }

  // Generar secuencia de Jacobsthal y definir orden de inserciones
  std::vector<size_t> jacob = generateJacobs(sorted_pend.size());
  std::vector<size_t> insert_order =
      generateInsertOrder(jacob, sorted_pend.size());

  // Insertar de manera binaria optimizada
  insertPend(deq, main_pairs, sorted_pend, insert_order);

  // Paso final: Insertar el elemento sobrante (leftover) de forma binaria si
  // existía
  if (hasLeftover) {
    size_t pos = binarySearch(deq, leftoverVal, deq.size());
    deq.insert(deq.begin() + pos, leftoverVal);
  }
}

void printCont(int type, const std::vector<int> &vec) {
  if (type == 1) {
    std::cout << "Before: ";
  } else {
    std::cout << "After:  ";
  }
  for (size_t i = 0; i < vec.size(); i++) {
    std::cout << vec[i] << (i + 1 == vec.size() ? "" : " ");
  }
  std::cout << std::endl;
}

// Controla la visualización del flujo completo, las salidas ordenadas y las
// mediciones de tiempo.
void PmergeMe::run() {
  printCont(1, this->_vec);

  // Medir ordenación con std::vector
  // std::clock_T devuelve cuantos clock ticks ha consumido nuestro proceso
  // actual
  std::clock_t startVec = std::clock();
  sortVector(_vec);
  std::clock_t endVec = std::clock();
  // CLOCKS_PER_SEC es una macro del sistema que indica cuantos ticks de CPU
  // equivalen a un segundo
  //  * 1e6 lo convertimos a milisegudos
  // lo guardamos en double para no descartar los decimales
  double timeVec =
      static_cast<double>(endVec - startVec) / CLOCKS_PER_SEC * 1e6;

  // Medir ordenación con std::deque
  std::clock_t startDeq = std::clock();
  sortDeque(_deq);
  std::clock_t endDeq = std::clock();
  double timeDeq =
      static_cast<double>(endDeq - startDeq) / CLOCKS_PER_SEC * 1e6;

  printCont(2, this->_vec);

  // Imprime los resultados con el formato idéntico al enunciado del subject
  std::cout << "Time to process a range of " << _vec.size()
            << " elements with std::vector : " << timeVec << " us" << std::endl;
  std::cout << "Time to process a range of " << _deq.size()
            << " elements with std::deque  : " << timeDeq << " us" << std::endl;
}
