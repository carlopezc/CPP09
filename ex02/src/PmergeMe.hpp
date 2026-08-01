#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <deque>
#include <vector>

class PmergeMe {
private:
  std::vector<int> _vec; // Contenedor secundario vector para almacenar y ordenar
  std::deque<int> _deq;  // Contenedor secundario deque para almacenar y ordenar

  // Métodos principales de ordenación
  void sortVector(std::vector<int> &vec);
  void sortDeque(std::deque<int> &deq);

  // Valida y rellena los contenedores a partir de los argumentos de la línea de comandos
  void parseInput(int argc, char **argv);

  // Paso 1: Agrupa elementos adyacentes en parejas ordenadas (mayor primero)
  std::vector<std::pair<int, int> > generatePairs(std::vector<int> &vec);
  std::deque<std::pair<int, int> > generatePairs(std::deque<int> &deq);
  
  // Paso 3 (Preparativo): Realinea los elementos menores (pend) con la secuencia sorted del main_chain
  std::vector<int> realignPend(const std::vector<int> &main_pairs, const std::vector<std::pair<int, int> > &pairs);
  std::deque<int> realignPend(const std::deque<int> &main_pairs, const std::deque<std::pair<int, int> > &pairs);

  // Paso 3: Genera la sucesión de números de Jacobsthal necesarios para las inserciones
  std::vector<size_t> generateJacobs(size_t limit);

  // Paso 3: Define el orden exacto de inserción óptimo usando la sucesión de Jacobsthal
  std::vector<size_t> generateInsertOrder(const std::vector<size_t> &jacob, size_t size);

  // Paso 3: Inserta de manera binaria los elementos menores (pend) en la cadena principal
  void insertPend(std::vector<int> &vec, const std::vector<int> &main_pairs, const std::vector<int> &sorted_pend, const std::vector<size_t> &insert_order);
  void insertPend(std::deque<int> &deq, const std::deque<int> &main_pairs, const std::deque<int> &sorted_pend, const std::vector<size_t> &insert_order);

public:
  PmergeMe(int argc, char **argv);
  ~PmergeMe();
  PmergeMe(const PmergeMe &other);
  PmergeMe &operator=(const PmergeMe &other);
  
  // Imprime secuencias iniciales y finales, ejecuta la ordenación y mide tiempos
  void run();
};

#endif