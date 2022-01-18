/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestMiscellanea.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/16 14:40:57 by kdustin           #+#    #+#             */
/*   Updated: 2021/12/04 02:44:26 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_MISCELLANEA_HPP

#include "Test.hpp"
#include "List.hpp"
#include "Vector.hpp"
#include "Map.hpp"
#include "Stack.hpp"
#include "Queue.hpp"
#include "String.hpp"

#include <string>
#include <list>
#include <vector>
#include <map>
#include <stack>
#include <queue>
#include <iostream>
#include <cassert>

int test_listgroup();
int test_vectorgroup();
int test_mapgroup();
int test_stackgroup();
int test_queuegroup();
int test_stringgroup();

class TestClass
{
private:
	int			_a;
	std::string	_b;
public:
	TestClass();
	TestClass(const TestClass& other);
	~TestClass();

	TestClass& operator=(TestClass& rhs);

	int getA();
	void setA(int a);
	std::string getB();
	void setB(std::string b);
};

bool single_digit(const int& value);

struct is_odd{
	bool operator() (const int& value);
};

bool same_integral_part(double first, double second);

struct is_near
{
	bool operator() (double first, double second);
};

bool mycomp(double first, double second);


bool descendingComp(const std::string first, const std::string second);

#endif // !TEST_MISCELLANEA_HPP
