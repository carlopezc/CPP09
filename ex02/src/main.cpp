/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 16:08:03 by carlopez          #+#    #+#             */
/*   Updated: 2026/08/01 16:18:20 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <iostream>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Error" << std::endl;
    return 1;
  }
  try {
    PmergeMe p(argc, argv);
    p.run();
  } catch (const std::exception &e) {
    std::cerr << "Error" << std::endl;
    return 1;
  }
  return 0;
}
