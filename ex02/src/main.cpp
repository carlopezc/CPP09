#include "PmergeMe.hpp"
#include <iostream>

int main(int argc, char **argv) {
  // Se requiere al menos un argumento (la secuencia de enteros positivos)
  if (argc < 2) {
    std::cerr << "Error" << std::endl;
    return 1;
  }
  
  try {
    // Inicialización del objeto PmergeMe pasándole los argumentos.
    // Esto llamará a parseInput internamente para validar y almacenar los números.
    PmergeMe p(argc, argv);
    
    // Inicia el proceso de ordenación y medición de tiempos para ambos contenedores.
    p.run();
  } catch (const std::exception &e) {
    // Si se lanza alguna excepción de validación o desbordamiento, mostramos "Error"
    std::cerr << "Error" << std::endl;
    return 1;
  }
  return 0;
}
