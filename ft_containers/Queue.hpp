/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Queue.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 16:31:31 by kdustin           #+#    #+#             */
/*   Updated: 2021/05/29 16:39:48 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "List.hpp"

namespace ft
{
	template <class T, class Container = List<T> >
	class Queue
	{
	public:
		typedef	T				value_type;
		typedef Container		container_type;
		typedef size_t			size_type;

	protected:
		container_type _container;

	public:
		explicit Queue (const container_type& ctnr = container_type()) : _container(ctnr) {};
		bool empty() const {
			return (_container.empty());
		};

		size_type size() const {
			return (_container.size());
		};

		value_type& front() {
			return (_container.front());
		};
		const value_type& front() const {
			return (_container.front());
		};

		value_type& back() {
			return (_container.back());
		};
		const value_type& back() const {
			return (_container.back());
		};

		void push (const value_type& val) {
			_container.push_back(val);
		};

		void pop() {
			_container.pop_front();
		};

		template <class Tp, class Seq>
		friend bool operator== (const Queue<Tp, Seq>&, const Queue<Tp, Seq>&);
		template <class Tp, class Seq>
		friend bool operator!= (const Queue<Tp, Seq>&, const Queue<Tp, Seq>&);
		template <class Tp, class Seq>
		friend bool operator< (const Queue<Tp, Seq>&, const Queue<Tp, Seq>&);
		template <class Tp, class Seq>
		friend bool operator<= (const Queue<Tp, Seq>&, const Queue<Tp, Seq>&);
		template <class Tp, class Seq>
		friend bool operator> (const Queue<Tp, Seq>&, const Queue<Tp, Seq>&);
		template <class Tp, class Seq>
		friend bool operator>= (const Queue<Tp, Seq>&, const Queue<Tp, Seq>&);
	};

	template <class T, class Container>
  	bool operator== (const Queue<T, Container>& lhs, const Queue<T, Container>& rhs) {
		return (lhs._container == rhs._container);
	}

	template <class T, class Container>
  	bool operator!= (const Queue<T, Container>& lhs, const Queue<T, Container>& rhs) {
		return (lhs._container != rhs._container);
	}

	template <class T, class Container>
  	bool operator<  (const Queue<T, Container>& lhs, const Queue<T, Container>& rhs) {
		return (lhs._container < rhs._container);
	}

	template <class T, class Container>
  	bool operator<= (const Queue<T, Container>& lhs, const Queue<T, Container>& rhs) {
		return (lhs._container <= rhs._container);
	}

	template <class T, class Container>
  	bool operator>  (const Queue<T, Container>& lhs, const Queue<T, Container>& rhs) {
		return (lhs._container > rhs._container);
	}

	template <class T, class Container>
  	bool operator>= (const Queue<T, Container>& lhs, const Queue<T, Container>& rhs) {
		return (lhs._container >= rhs._container);
	}

} // namespace ft

#endif // !QUEUE_HPP
