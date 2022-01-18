/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Test.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/19 17:54:26 by marvin            #+#    #+#             */
/*   Updated: 2021/04/19 17:54:26 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_HPP
#define TEST_HPP

#include <iomanip>
#include <iostream>

#define OK "\033[1;32m Ok \033[0m"
#define FAILED "\033[1;31m Failed \033[0m"

/**
 * @brief A result class. This class contains the result information about a test.
 */
class Result
{
private:
	Result();
	bool			_is_passed;
	unsigned long	_line;
	std::string		_file;
	std::string		_code;

	Result(Result& x);
	Result& operator= (Result& rhs);
public:
	/**
	 * @brief Construct a new Result object
	 *
	 * @param is_passed true if test has been passed.
	 * @param line number of the line where test presented.
	 * @param file file where test presented.
	 * @param code code associated with test.
	 */
	Result(bool is_passed, unsigned int line, std::string file, std::string code);
	~Result();

	void setIsPassed(bool is_passed);
	bool getIsPassed();
	void setLine(unsigned int line);
	void setFile(std::string file);
	void setCode(std::string code);

	/**
	 * @brief print test result information.
	 */
	void print();
};

#define ASSERT(condition)	Test(condition, __FILE__, __LINE__, #condition)
#define ASSERT_EQ(A, B)		ASSERT(isEq(A, B));
#define ASSERT_TRUE(A)		ASSERT(isTrue(A));
#define ASSERT_FALSE(A)		ASSERT(isFalse(A));

class Test
{
private:
	Test();
public:
	~Test() {};

	/**
	 * @brief Construct a new Test object and print result.
	 *
	 * @param eval if this paramer is true the test will be passed.
	 * @param file file where test presented. __FILE__
	 * @param line number of the line where test presented. __LINE__
	 * @param code code associated with test. #eval
	 */
	Test(bool eval, const char* file, unsigned long line, const char* code);
};

template <typename T>
bool isEq(T a, T b)
{
	std::cout << a << " ?= " << b << " ";
	if (a == b)
		return (true);
	return (false);
}

template <typename T>
bool isTrue(T a)
{
	if (a == true)
		return (true);
	return (false);
}

template <typename T>
bool isFalse(T a)
{
	if (a == false)
		return (true);
	return (false);
}

#endif // !TEST_HPP
