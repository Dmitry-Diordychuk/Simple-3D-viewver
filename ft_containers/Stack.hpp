/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 13:56:51 by kdustin           #+#    #+#             */
/*   Updated: 2021/05/29 16:38:34 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_HPP
#define STACK_HPP

#include "Vector.hpp"

namespace ft
{
	template <class T, class Container = Vector<T> >
	class Stack
	{
	public:
		typedef	T				value_type;
		typedef Container		container_type;
		typedef size_t			size_type;

	protected:
		container_type _container;

	public:
		explicit Stack (const container_type& ctnr = container_type()) : _container(ctnr) {};
		bool empty() const {
			return (_container.empty());
		};

		size_type size() const {
			return (_container.size());
		};

		value_type& top() {
			return (_container.back());
		};

		const value_type& top() const {
			return (_container.back());
		};

		void push (const value_type& val) {
			return (_container.push_back(val));
		};

		void pop() {
			return (_container.pop_back());
		};

		template <class Tp, class Seq>
		friend bool operator== (const Stack<Tp, Seq>&, const Stack<Tp, Seq>&);
		template <class Tp, class Seq>
		friend bool operator!= (const Stack<Tp, Seq>&, const Stack<Tp, Seq>&);
		template <class Tp, class Seq>
		friend bool operator< (const Stack<Tp, Seq>&, const Stack<Tp, Seq>&);
		template <class Tp, class Seq>
		friend bool operator<= (const Stack<Tp, Seq>&, const Stack<Tp, Seq>&);
		template <class Tp, class Seq>
		friend bool operator> (const Stack<Tp, Seq>&, const Stack<Tp, Seq>&);
		template <class Tp, class Seq>
		friend bool operator>= (const Stack<Tp, Seq>&, const Stack<Tp, Seq>&);
	};

	template <class T, class Container>
  	bool operator== (const Stack<T, Container>& lhs, const Stack<T, Container>& rhs) {
		return (lhs._container == rhs._container);
	}

	template <class T, class Container>
  	bool operator!= (const Stack<T, Container>& lhs, const Stack<T, Container>& rhs) {
		return (lhs._container != rhs._container);
	}

	template <class T, class Container>
  	bool operator<  (const Stack<T, Container>& lhs, const Stack<T, Container>& rhs) {
		return (lhs._container < rhs._container);
	}

	template <class T, class Container>
  	bool operator<= (const Stack<T, Container>& lhs, const Stack<T, Container>& rhs) {
		return (lhs._container <= rhs._container);
	}

	template <class T, class Container>
  	bool operator>  (const Stack<T, Container>& lhs, const Stack<T, Container>& rhs) {
		return (lhs._container > rhs._container);
	}

	template <class T, class Container>
  	bool operator>= (const Stack<T, Container>& lhs, const Stack<T, Container>& rhs) {
		return (lhs._container >= rhs._container);
	}
}

#endif // !STACK_HPP
