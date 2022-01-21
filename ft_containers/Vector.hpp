/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/22 01:11:51 by kdustin           #+#    #+#             */
/*   Updated: 2022/01/20 22:21:13 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <limits>
#include "iterator.hpp"
#include "reverse_iterator.hpp"
#include "type_traits.hpp"
#include "Allocator.hpp"

namespace ft
{
	template <class T, class Alloc = Allocator<T> >
	class Vector
	{
	public:
		typedef T											value_type;
		typedef Alloc										allocator_type;
		typedef T&											reference;
		typedef const T&									const_reference;
		typedef T*											pointer;
		typedef const T*									const_pointer;
		typedef typename Alloc::difference_type				difference_type;
		typedef size_t										size_type;

		typedef pointer										iterator;
		typedef const_pointer								const_iterator;
		typedef ft::reverse_iterator<iterator>				reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>		const_reverse_iterator;

	private:
		allocator_type	_allocator;
		pointer			_start;
		pointer			_finish;
		pointer			_end_of_storage;

		void deleteArray()
		{
			if (_start)
			{
				size_type i = -1;
				while (++i < size())
					_allocator.destroy(_start + i);
				_allocator.deallocate(_start, capacity());
			}
		}

		void realloc(size_type new_max_size = 0)
		{
			if (new_max_size == 0)
			{
				if (size() == 0)
					new_max_size = 1;
				else
					new_max_size = 2 * size();
			}
			if (new_max_size > _allocator.max_size())
				return ;
			pointer new_start;
			try
			{
				new_start = _allocator.allocate(new_max_size);
			}
			catch(...)
			{
				throw;
			}
			pointer new_finish = new_start;
			pointer new_end_of_storage = new_start + new_max_size;
			for (size_type i = 0; i < size(); ++i)
			{
				_allocator.construct(new_finish, _start[i]);
				++new_finish;
			}
			deleteArray();
			_start = new_start;
			_finish = new_finish;
			_end_of_storage = new_end_of_storage;
		}

	public:
		explicit Vector (const allocator_type& alloc = allocator_type())
		 : _allocator(alloc), _start(NULL), _finish(NULL), _end_of_storage(NULL) {};

		explicit Vector (size_type n, const value_type& val = value_type(),
							const allocator_type& alloc = allocator_type())
		 : _allocator(alloc), _start(NULL), _finish(NULL), _end_of_storage(NULL) {
			if (n < _allocator.max_size())
			{
				pointer temp;
				try
				{
					temp = _allocator.allocate(n);
				}
				catch(...)
				{
					throw;
				}
				_start = temp;
				_finish = _start;
				_end_of_storage = _start + n;
				size_type i = -1;
				while (++i < n)
				{
					_allocator.construct(_finish, val);
					++_finish;
				}
			}
		};

		template <class InputIterator>
		Vector (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
				typename ft::enable_if< !ft::is_integral<InputIterator>::value >::type* = 0) {
			size_type len = last - first;

			_allocator = alloc;
			pointer temp;
			try
			{
				temp = _allocator.allocate(len);
			}
			catch(...)
			{
				throw;
			}
			_start = temp;
			_finish = _start;
			_end_of_storage = _start + (len);
			while (first != last)
			{
				_allocator.construct(_finish, *first);
				++_finish;
				++first;
			}
		};

		Vector (const Vector& x) {
			this->_allocator = x._allocator;
			pointer temp;
			try
			{
				temp = _allocator.allocate(x.size());
			}
			catch(...)
			{
				throw;
			}
			this->_start = temp;
			this->_finish = this->_start;
			this->_end_of_storage = this->_start + x.size();
			const_iterator it = x.begin();
			while (it != x.end())
			{
				this->_allocator.construct(this->_finish, *it);
				++it;
				++this->_finish;
			}
		};

		~Vector() {
			deleteArray();
		};

		Vector& operator= (const Vector& x) {
			pointer temp;
			try
			{
				temp = _allocator.allocate(x.size());
			}
			catch(...)
			{
				throw;
			}
			deleteArray();
			this->_allocator = x._allocator;
			this->_start = temp;
			this->_finish = this->_start;
			this->_end_of_storage = this->_start + x.size();
			const_iterator it = x.begin();
			while (it != x.end())
			{
				this->_allocator.construct(this->_finish, *it);
				++it;
				++this->_finish;
			}
			return (*this);
		};

		iterator begin() {
			return (_start);
		};

		const_iterator begin() const {
			return (_start);
		};

		iterator end() {
			return (_finish);
		};

		const_iterator end() const {
			return (_finish);
		};

		reverse_iterator rbegin() {
			reverse_iterator rit(_finish);
			return (rit);
		};

		const_reverse_iterator rbegin() const {
			reverse_iterator rit(_finish);
			return (rit);
		};

		reverse_iterator rend() {
			reverse_iterator rit(_start);
			return (rit);
		};

		const_reverse_iterator rend() const {
			reverse_iterator rit(_start);
			return (rit);
		};

		size_type size() const {
			return (_finish - _start);
		};

		size_type max_size() const{
			return (std::numeric_limits<size_type>::max() / sizeof(T));
		};

		void resize (size_type n, value_type val = value_type()) {
			if (n > size())
			{
				try
				{
					realloc(n);
				}
				catch(...)
				{
					throw;
				}
				while (n > size())
					push_back(val);
			}
			else
				while (n < size())
					pop_back();
		};

		size_type capacity() const {
			return (_end_of_storage - _start);
		};

		bool empty() const {
			if (size())
				return (false);
			return (true);
		};

		void reserve (size_type n) {
			if (n > size())
			{
				try
				{
					realloc(n);
				}
				catch(...)
				{
					throw;
				}
			}
		};

		reference operator[] (size_type n) {
			return (*(_start + n));
		};

		const_reference operator[] (size_type n) const {
			return (*(_start + n));
		};

		reference at (size_type n) {
			if (n >= size())
				throw std::out_of_range("vector");
			return (*(_start + n));
		};

		const_reference at (size_type n) const {
			if (n >= size())
				throw std::out_of_range("vector");
			return (*(_start + n));
		};

		reference front() {
			return (*_start);
		};

		const_reference front() const {
			return (*_start);
		};

		reference back() {
			pointer p = _finish;
			--p;
			return (*p);
		};

		const_reference back() const {
			const_pointer p = _finish;
			--p;
			return (*p);
		};

		template <class InputIterator>
		void assign (InputIterator first, InputIterator last,
			typename ft::enable_if< !ft::is_integral<InputIterator>::value >::type* = 0) {
			size_type len = last - first;
			pointer temp;
			try
			{
				temp = _allocator.allocate(len);
			}
			catch(...)
			{
				throw;
			}
			deleteArray();
			_start = temp;
			_finish = _start;
			_end_of_storage = _start + len;
			while (first != last)
			{
				push_back(*first);
				++first;
			}
		};

		void assign (size_type n, const value_type& val) {
			pointer temp;
			try
			{
				temp = _allocator.allocate(n);
			}
			catch(...)
			{
				throw;
			}
			deleteArray();
			_start = temp;
			_finish = _start;
			_end_of_storage = _start + n;
			for (size_type i = 0; i < n; ++i)
				push_back(val);
		};

		void push_back (const value_type& val) {
			if (_finish == _end_of_storage)
			{
				if (size() + 1 > _allocator.max_size())
					return ;
				try
				{
					realloc();
				}
				catch(...)
				{
					throw;
				}
			}
			_allocator.construct(_finish, val);
			++_finish;
		};

		void pop_back() {
			--_finish;
			_allocator.destroy(_finish);
		};

		iterator insert (iterator position, const value_type& val) {
			size_type index = position - _start;
			if (size() + 1 > capacity())
			{
				try
				{
					realloc();
				}
				catch(...)
				{
					throw;
				}
			}
			pointer p = _finish;
			while (p != _start + index)
			{
				if ((p - 1) >= _start)
				{
					_allocator.construct(p, *(p - 1));
					_allocator.destroy(p - 1);
				}
				--p;
			}
			_allocator.construct(p, val);
			++_finish;
			return (p);
		};

		void insert (iterator position, size_type n, const value_type& val) {
			if (n == 0)
				return ;
			size_type index = position - _start;
			size_type new_size = size() + n;
			if (new_size > capacity())
			{
				try
				{
					realloc(new_size);
				}
				catch(...)
				{
					throw;
				}
			}
			pointer p = _finish + n - 1;
			while (p - n >= _start + index)
			{
				_allocator.construct(p, *(p - n));
				_allocator.destroy(p - n);
				--p;
			}
			size_type i = -1;
			while (++i < n)
			{
				_allocator.construct(p - i, val);
				++_finish;
			}
		};

		template <class InputIterator>
		void insert (iterator position, InputIterator first, InputIterator last,
			typename ft::enable_if< !ft::is_integral<InputIterator>::value >::type* = 0) {
			size_type n = last - first;
			if (n == 0)
				return ;
			size_type index = position - _start;
			size_type new_size = size() + n;
			if (new_size > capacity())
			{
				try
				{
					realloc(new_size);
				}
				catch(...)
				{
					throw;
				}
			}
			pointer p = _finish + n - 1;
			while (p - n >= _start + index)
			{
				_allocator.construct(p, *(p - n));
				_allocator.destroy(p - n);
				--p;
			}
			size_type i = -1;
			while (++i < n)
			{
				--last;
				_allocator.construct(p - i, *last);
				++_finish;
			}
		};

		iterator erase (iterator position) {
			iterator result = position;
			_allocator.destroy(position);
			while (position + 1 != _finish)
			{
				_allocator.construct(position, position[1]);
				_allocator.destroy(position + 1);
				++position;
			}
			--_finish;
			return (result);
		};

		iterator erase (iterator first, iterator last) {
			size_type len = last - first;
			if (len == 0)
				return (last);
			iterator result = NULL;
			while (first != _finish)
			{
				if (first < last)
				{
					_allocator.destroy(first);
				}
				if (first + len < _finish)
				{
					_allocator.construct(first, first[len]);
					_allocator.destroy(first + len);
					if (result == NULL)
						result = first;
				}
				++first;
			}
			_finish -= len;
			if (result == NULL)
				return (end());
			return (result);
		};

		void swap (Vector& x) {
			pointer temp_start = _start;
			pointer temp_finish = _finish;
			pointer temp_end_of_storage = _end_of_storage;
			_start = x._start;
			_finish = x._finish;
			_end_of_storage = x._end_of_storage;
			x._start = temp_start;
			x._finish = temp_finish;
			x._end_of_storage = temp_end_of_storage;
		};

		void clear() {
			erase(begin(), end());
		};

		allocator_type get_allocator() const {
			return (_allocator);
		};
	};

	template <class T, class Alloc>
	bool operator== (const Vector<T,Alloc>& lhs, const Vector<T,Alloc>& rhs) {
		if (lhs.size() != rhs.size())
			return (false);
		typename ft::Vector<T>::const_iterator lit = lhs.begin();
		typename ft::Vector<T>::const_iterator rit = rhs.begin();
		while (lit != lhs.end())
		{
			if (rit == rhs.end() || *lit != *rit)
				return (false);
			++lit;
			++rit;
		}
		return (true);
	}

	template <class T, class Alloc>
	bool operator!= (const Vector<T,Alloc>& lhs, const Vector<T,Alloc>& rhs) {
		if (lhs == rhs)
			return (false);
		return (true);
	}

	template <class T, class Alloc>
	bool operator< (const Vector<T,Alloc>& lhs, const Vector<T,Alloc>& rhs) {
		typename ft::Vector<T>::const_iterator lit = lhs.begin();
		typename ft::Vector<T>::const_iterator rit = rhs.begin();
		while (lit != lhs.end())
		{
			if (rit == rhs.end() || *lit > *rit)
				return (false);
			else if (*lit < *rit)
				return (true);
			++lit;
			++rit;
		}
		if (rit == rhs.end() && lit == lhs.end())
			return (false);
		return (true);
	}

	template <class T, class Alloc>
	bool operator<= (const Vector<T,Alloc>& lhs, const Vector<T,Alloc>& rhs) {
		typename ft::Vector<T>::const_iterator lit = lhs.begin();
		typename ft::Vector<T>::const_iterator rit = rhs.begin();
		while (lit != lhs.end())
		{
			if (rit == rhs.end() || *lit > *rit)
				return (false);
			else if (*lit < *rit)
				return (true);
			++lit;
			++rit;
		}
		return (true);
	}

	template <class T, class Alloc>
	bool operator>  (const Vector<T,Alloc>& lhs, const Vector<T,Alloc>& rhs) {
		if (lhs <= rhs)
			return (false);
		return (true);
	}

	template <class T, class Alloc>
	bool operator>= (const Vector<T,Alloc>& lhs, const Vector<T,Alloc>& rhs) {
		if (lhs < rhs)
			return (false);
		return (true);
	}

	template <class T, class Alloc>
	void swap (Vector<T,Alloc>& x, Vector<T,Alloc>& y) {
		x.swap(y);
	}
}

#endif // !VECTOR_HPP
