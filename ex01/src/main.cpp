/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlopez <carlopez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 18:00:35 by carlopez          #+#    #+#             */
/*   Updated: 2026/08/01 18:00:37 by carlopez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include <iostream>

int main(int ac, char **av) {
  if (ac != 2) {
    std::cerr << "Error: Invalid arguments" << std::endl;
    return 1;
  }
  try {
    RPN rpn;
    rpn.evaluate(av[1]);
  } catch (std::exception &e) {
    std::cerr << "Error" << std::endl;
    return 1;
  }
  return 0;
}