/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/15 17:35:37 by marvin            #+#    #+#             */
/*   Updated: 2021/04/15 17:35:37 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_HPP
#define LIST_HPP

#include <limits>
#include "iterator.hpp"
#include "reverse_iterator.hpp"
#include "type_traits.hpp"
#include "Allocator.hpp"

namespace ft
{
	/**
	 * @brief double-linked list
	 *
	 * @tparam T value type.
	 * @tparam Alloc allocator.
	 */
	template <class T, class Alloc = Allocator<T> >
	class List
	{
	public:
		typedef T										value_type;
		typedef Alloc									allocator_type;
		typedef T&										reference;
		typedef const T&								const_reference;
		typedef T*										pointer;
		typedef const T*								const_pointer;
		typedef typename Alloc::difference_type			difference_type;
		typedef size_t									size_type;

		template <typename Tp, typename Ref, typename Ptr>
		class _List_iterator;

		typedef _List_iterator<T, T&, T*>				iterator;
		typedef _List_iterator<T, const T&, const T*>	const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

	private:
		allocator_type		_allocator;
		size_type			_n;
		typedef struct		s_node
		{
			pointer		data;
			s_node		*next;
			s_node		*prev;
		}					t_node;
		t_node				*_begin;
		t_node				*_end;

		t_node *createNode(const value_type& val) {
			pointer p;
			t_node *node = NULL;
			try
			{
				p = _allocator.allocate(1);
			}
			catch(...)
			{
				throw;
			}
			_allocator.construct(p, val);
			try
			{
				node = new t_node();
			}
			catch(...)
			{
				_allocator.destroy(p);
				_allocator.deallocate(p, 1);
				throw;
			}
			if (node)
			{
				node->data = p;
				node->next = NULL;
				node->prev = NULL;
			}
			return (node);
		};

		void deleteNode(t_node *node)
		{
			_allocator.destroy(node->data);
			_allocator.deallocate(node->data, -1);
			delete node;
		}

		t_node *createEnd() {
			t_node *node = NULL;
			try
			{
				node = new t_node();
			}
			catch(...)
			{
				throw;
			}
			if (node)
			{
				try
				{
					node->data = new T();
				}
				catch(...)
				{
					delete node;
					throw;
				}
				node->next = NULL;
				node->prev = NULL;
			}
			return (node);
		};

		t_node *lastNode() {
			return (this->_end->prev);
		};

		void insertNode(t_node *new_node, t_node *prev, t_node *next) {
			new_node->next = next;
			if (new_node->next)
				new_node->next->prev = new_node;
			new_node->prev = prev;
			if (new_node->prev)
				new_node->prev->next = new_node;
			_n++;
			if (!new_node->prev)
				_begin = new_node;
		}

		void unlinkNode(t_node *node)
		{
			if (node->next)
				node->next->prev = NULL;
			node->next = NULL;
			if (node->prev)
				node->prev->next = NULL;
			node->prev = NULL;
		}

		template <class Compare>
		t_node* merge(t_node* left, size_type leftN,t_node* right, size_type rightN, Compare comp)
		{
			t_node* result = NULL;
			size_type totalLen = leftN + rightN;
			size_type resultN = 0;

			if (leftN == 0)
				return (left);
			if (rightN == 0)
				return (right);
			while (resultN < totalLen)
			{
				t_node* temp;
				if ((left && !right) || (left && right && comp(*left->data, *right->data)))
				{
					temp = left->next;
					unlinkNode(left);
					if (result)
					{
						result->next = left;
						left->prev = result;
						result = result->next;
					}
					else
						result = left;
					left = temp;
				}
				else
				{
					temp = right->next;
					unlinkNode(right);
					if (result)
					{
						result->next = right;
						right->prev = result;
						result = result->next;
					}
					else
						result = right;
					right = temp;
				}
				++resultN;
			}
			while (result->prev != NULL)
				result = result->prev;
			return (result);
		}

		t_node* splitList(t_node* list, size_type middle)
		{
			t_node* temp = list;
			size_type i = 1;
			while (i < middle){
				temp = temp->next;
				++i;
			}
			if (temp->next)
				temp->next->prev = NULL;
			t_node *right = temp->next;
			temp->next = NULL;
			return (right);
		}

		static bool defaultComp(const T& first, const T& second)
		{
			return (first < second);
		}

		t_node* mergeSort(t_node* list, size_type n)
		{
			if (n < 2)
				return (list);

			size_type middle;
			size_type reminder;
			if (list)
			{
				middle = n / 2;
				reminder = n % 2;
			}
			else
			{
				middle = 0;
				reminder = 0;
			}

			t_node* right = splitList(list, middle);
			t_node* left = mergeSort(list, middle, defaultComp);
			right = mergeSort(right, middle + reminder, defaultComp);

			return (merge(left, left ? middle : 0, right, right ? middle + reminder : 0, defaultComp));
		}

		template <class Compare>
		t_node* mergeSort(t_node* list, size_type n, Compare comp)
		{
			if (n < 2)
				return (list);

			size_type middle;
			size_type reminder;
			if (list)
			{
				middle = n / 2;
				reminder = n % 2;
			}
			else
			{
				middle = 0;
				reminder = 0;
			}

			t_node* right = splitList(list, middle);
			t_node* left = mergeSort(list, middle, comp);
			right = mergeSort(right, middle + reminder, comp);

			return (merge(left, left ? middle : 0, right, right ? middle + reminder : 0, comp));
		}

	public:
		// CONSTRUCTORS
		explicit List (const allocator_type& alloc = allocator_type())
			:	_allocator(alloc),
				_n(0)
		{
			try
			{
				this->_end = createEnd();
			}
			catch(...)
			{
				throw;
			}
			_begin = this->_end;
		};

		explicit List (size_type n, const value_type& val = value_type(),
			const allocator_type& alloc = allocator_type())
			:	_allocator(alloc),
				_n(0)
		{
			try
			{
				this->_end = createEnd();
			}
			catch(...)
			{
				throw;
			}
			_begin = this->_end;
			this->assign(n, val);
		};

		template <class InputIterator>
		List (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
			typename ft::enable_if< !ft::is_integral<InputIterator>::value >::type* = 0)
			:	_allocator(alloc),
				_n(0)
		{
			try
			{
				this->_end = createEnd();
			}
			catch(...)
			{
				throw;
			}
			_begin = this->_end;
			this->assign(first, last);
		};

		List (const List& x)
			:	_allocator(x._allocator),
				_n(0)
		{
			try
			{
				this->_end = createEnd();
			}
			catch(...)
			{
				throw;
			}
			_begin = this->_end;
			this->assign(x.begin(), x.end());
		};

		//DESTRUCTOR
		~List() {
			while (_begin != _end)
			{
				t_node *temp = _begin->next;
				_allocator.destroy(_begin->data);
				_allocator.deallocate(_begin->data, -1);
				delete _begin;
				_begin = temp;
			}
			delete _end->data;
			delete _end;
		};

		//Operator =
		List& operator= (const List& x) {
			this->_allocator = x._allocator;
			this->_begin = this->_end;
			this->assign(x.begin(), x.end());
			return (*this);
		};

		// Iterators
		iterator begin() {
			if (_n != 0)
			{
				iterator it(this->_begin);
				return (it);
			}
			iterator it(_end);
			return (it);
		};

		const_iterator begin() const {
			if (_n != 0)
			{
				const_iterator it(this->_begin);
				return (it);
			}
			const_iterator it(_end);
			return (it);
		};

		iterator end() {
			iterator it(_end);
			return (it);
		};

		const_iterator end() const {
			const_iterator it(_end);
			return (it);
		};

		reverse_iterator rbegin() {
			reverse_iterator rev_it(_end);
			return (rev_it);
		};

		const_reverse_iterator rbegin() const {
			reverse_iterator rev_it(_end);
			return (rev_it);
		};

		reverse_iterator rend() {
			reverse_iterator rev_it(_begin);
			return (rev_it);
		};

		const_reverse_iterator rend() const {
			reverse_iterator rev_it(_begin);
			return (rev_it);
		};

		// Capacity
		bool empty() const {
			if (_n == 0)
				return (true);
			return (false);
		};

		size_type size() const {
			return (_n);
		};

		size_type max_size() const {
			return (std::numeric_limits<size_type>::max());
		};

		// Element access
		reference front() {
			return (*(this->begin()));
		};

		const_reference front() const {
			return (*(this->begin()));
		};

		reference back() {
			iterator it = this->end();
			--it;
			return (*it);
		}

		const_reference back() const {
			const_iterator it = this->end();
			--it;
			return (*it);
		}

		// Modifiers
		template <class InputIterator>
		void assign (InputIterator first, InputIterator last,
			typename ft::enable_if< !ft::is_integral<InputIterator>::value >::type* = 0)
		{
			this->clear();
			for (; first != last; ++first)
				push_back(*first);
		}

		void assign (size_type n, const value_type& val) {
			this->clear();
			for (size_type i = 0; i < n; ++i)
				push_back(val);
		};

		void push_front (const value_type& val) {
			if (_n < max_size())
			{
				t_node *node = NULL;
				try
				{
					node = createNode(val);
				}
				catch(...)
				{
					throw;
				}
				if (node)
				{
					node->next = _begin;
					_begin->prev = node;
					_begin = node;
					++_n;
				}
			}
		};

		void pop_front() {
			if (_n != 0)
			{
				t_node *temp = _begin;
				_begin = _begin->next;
				_begin->prev = NULL;
				_allocator.destroy(temp->data);
				_allocator.deallocate(temp->data, -1);
				delete temp;
				--_n;
			}
		};

		void push_back (const value_type& val) {
			if (_n < max_size())
			{
				t_node *node = NULL;
				try
				{
					node = createNode(val);
				}
				catch(...)
				{
					throw;
				}
				t_node *last = lastNode();
				if (last != NULL)
				{
					node->prev = last;
					last->next = node;
				}
				else
					_begin = node;
				node->next = this->_end;
				this->_end->prev = node;
				++_n;
			}
		};

		void pop_back() {
			if (_n != 0)
			{
				t_node *last = lastNode();
				if (last->prev != NULL)
					last->prev->next = this->_end;
				else
					_begin = _end;
				this->_end->prev = last->prev;
				_allocator.destroy(last->data);
				_allocator.deallocate(last->data, -1);
				delete last;
				--_n;
			}
		};

		iterator insert (iterator position, const value_type& val) {
			t_node *new_node = NULL;
			try
			{
				new_node = createNode(val);
			}
			catch(...)
			{
				throw;
			}
			iterator	result;

			insertNode(new_node, position.getCurrent()->prev, position.getCurrent());
			result.setCurrent(new_node);
			return (result);
		};

		void insert (iterator position, size_type n, const value_type& val)
		{
			for (size_type i = 0; i < n; ++i)
				position = this->insert(position, val);
		};

		template <class InputIterator>
		void insert (iterator position, InputIterator first, InputIterator last,
			typename ft::enable_if< !ft::is_integral<InputIterator>::value >::type* = 0)
		{
			if (last == first)
				return ;
			--last;
			for (; last != first; --last)
				position = this->insert(position, *last);
			position = this->insert(position, *last);
		};

		iterator erase (iterator position) {
			t_node *next = position.getCurrent()->next;
			t_node *prev = position.getCurrent()->prev;
			iterator result;

			if (_n == 0 || position.getCurrent() == _end)
			{
				result.setCurrent(_end);
				return (result);
			}
			if (position.getCurrent() == _begin)
				_begin = position.getCurrent()->next;
			deleteNode(position.getCurrent());
			if (next)
				next->prev = prev;
			if (prev)
				prev->next = next;
			result.setCurrent(next);
			--this->_n;
			return (result);
		};

		iterator erase (iterator first, iterator last) {
			while (first != last)
				first = this->erase(first);
			return (last);
		};

		void swap (List& x) {
			t_node *temp_begin = this->_begin;
			t_node *temp_end = this->_end;
			size_type temp_n = this->_n;
			this->_begin = x._begin;
			this->_end = x._end;
			this->_n = x._n;
			x._begin = temp_begin;
			x._end = temp_end;
			x._n = temp_n;
		};

		void resize (size_type n, value_type val = value_type()) {
			while (_n > n)
				this->pop_back();
			while (_n < n)
				this->push_back(val);
		};

		void clear() {
			while (_n != 0)
				this->pop_back();
			_end->next = NULL;
			_end->prev = NULL;
		};

		// Operations
		void splice (iterator position, List& x) {
			splice(position, x, x.begin(), x.end());
		};

		void splice (iterator position, List& x, iterator i) {
			iterator first = i;
			i++;
			splice(position, x, first, i);
		};

		void splice (iterator position, List& x, iterator first, iterator last) {
			t_node *pos =   position.getCurrent()->prev;
			t_node *next = position.getCurrent();
			iterator it = first;
			size_type counter = 0;

			counter = 0;
			while (it != last)
			{
				counter++;
				++it;
			}
			if (x._n == 0)
				return ;
			t_node* temp = first.getCurrent()->prev;
			first.getCurrent()->prev = pos;
			if (pos)
				pos->next = first.getCurrent();
			else
				this->_begin = first.getCurrent();
			last.getCurrent()->prev->next = next;
			next->prev = last.getCurrent()->prev;
			last.getCurrent()->prev = temp;
			if (temp)
				temp->next = last.getCurrent();
			x._begin = x._end;
			while (x._begin->prev != NULL)
				x._begin = x._begin->prev;
			this->_n += counter;
			x._n -= counter;
		};

		void remove (const value_type& val) {
			t_node* node = _begin;
			while (node != _end)
			{
				if (*node->data == val)
				{
					t_node* temp = node->next;
					if (node->prev)
						node->prev->next = node->next;
					else
						_begin = node->next;
					node->next->prev = node->prev;
					deleteNode(node);
					--_n;
					node = temp;
				}
				else
					node = node->next;
			}
		};

		template <class Predicate>
		void remove_if (Predicate pred) {
			t_node* node = _begin;
			while (node != _end)
			{
				if (pred(*node->data))
				{
					t_node* temp = node->next;
					if (node->prev)
						node->prev->next = node->next;
					else
						_begin = node->next;
					node->next->prev = node->prev;
					deleteNode(node);
					--_n;
					node = temp;
				}
				else
					node = node->next;
			}
		};

		void unique() {
			t_node* node = _begin->next;
			while (node != NULL && node != _end)
			{
				if (*node->data == *node->prev->data)
				{
					t_node* temp = node->next;
					if (node->prev)
						node->prev->next = node->next;
					else
						_begin = node->next;
					node->next->prev = node->prev;
					deleteNode(node);
					--_n;
					node = temp;
				}
				else
					node = node->next;
			}
		};

		template <class BinaryPredicate>
		void unique (BinaryPredicate binary_pred) {
			t_node* node = _begin->next;
			while (node != NULL && node != _end)
			{
				if (binary_pred(*node->prev->data, *node->data))
				{
					t_node* temp = node->next;
					if (node->prev)
						node->prev->next = node->next;
					else
						_begin = node->next;
					node->next->prev = node->prev;
					deleteNode(node);
					--_n;
					node = temp;
				}
				else
					node = node->next;
			}
		};

		void merge (List& x) {
			t_node* rhs = x._begin;
			t_node* lhs = _begin;
			if (rhs == lhs)
				return ;
			while (rhs != x._end)
			{
				if (*rhs->data < *lhs->data || lhs == _end)
				{
					t_node* temp = rhs->next;
					insertNode(rhs, lhs->prev, lhs);
					rhs = temp;
				}
				else
					lhs = lhs->next;
			}
			x._begin = x._end;
			x._end->prev = NULL;
			x._n = 0;
		};

		template <class Compare>
		void merge (List& x, Compare comp) {
			t_node* rhs = x._begin;
			t_node* lhs = _begin;
			if (rhs == lhs)
				return ;
			while (rhs != x._end)
			{
				if (comp(*rhs->data, *lhs->data) || lhs == _end)
				{
					t_node* temp = rhs->next;
					insertNode(rhs, lhs->prev, lhs);
					rhs = temp;
				}
				else
					lhs = lhs->next;
			}
			x._begin = x._end;
			x._end->prev = NULL;
			x._n = 0;
		};

		void sort() {
			if (_n < 2)
				return ;
			_end->prev->next = NULL;
			t_node* result = mergeSort(_begin, _n);
			_begin = result;
			t_node* temp = result;
			while (temp->next != NULL)
				temp = temp->next;
			_end->prev = temp;
			temp->next = _end;
		};

		template <class Compare>
		void sort (Compare comp) {
			if (_n < 2)
				return ;
			_end->prev->next = NULL;
			t_node* result = mergeSort(_begin, _n, comp);
			_begin = result;
			t_node* temp = result;
			while (temp->next != NULL)
				temp = temp->next;
			_end->prev = temp;
			temp->next = _end;
		};

		void reverse() {
			t_node *temp;
			if (_end->prev)
				_end->prev->next = NULL;
			else
				return ;
			t_node *node = _begin;
			while (node)
			{
				temp = node->next;
				node->next = node->prev;
				node->prev = temp;
				node = node->prev;
			}
			temp = _begin;
			_begin = _end->prev;
			_end->prev = temp;
			temp->next = _end;
		};

		allocator_type get_allocator() const {
			return (_allocator);
		};

		class _List_iterator_base
		{
			private:
				t_node *_cur;
			public:
				_List_iterator_base() {};
				_List_iterator_base(t_node *node) : _cur(node) {};
				_List_iterator_base(const _List_iterator_base& x) : _cur(x._cur) {};
				~_List_iterator_base() {};
				_List_iterator_base& operator=(const _List_iterator_base& rhs){
					this->_cur = rhs._cur;
					return (*this);
				};

				t_node	*getCurrent() const {return (_cur);};
				void	setCurrent(t_node *cur) {_cur = cur;};

				bool operator== (const _List_iterator_base& other) const {return (this->_cur == other._cur);};
				bool operator!= (const _List_iterator_base& other) const {return (this->_cur != other._cur);};

				_List_iterator_base& increment () {
					if (_cur->next != NULL)
						_cur = _cur->next;
					return (*this);
				};

				_List_iterator_base& decrement () {
					if (_cur->prev != NULL)
						_cur = _cur->prev;
					return (*this);
				};
		};

		template <typename Tp, typename Ref, typename Ptr>
		class _List_iterator : public _List_iterator_base
		{
			public:
				typedef _List_iterator<Tp, Tp&, Tp*>				iterator;
				typedef _List_iterator<Tp, Ref, Ptr>				self;

				typedef bidirectional_iterator_tag					iterator_category;
				typedef Tp											value_type;
				typedef std::ptrdiff_t								difference_type;
				typedef Ptr											pointer;
				typedef Ref											reference;

				_List_iterator() {};
				_List_iterator(t_node *current) : _List_iterator_base(current) {};
				_List_iterator(const iterator& other) : _List_iterator_base(other) {};
				~_List_iterator() {};

				self& operator= (const iterator& rhs) {
					_List_iterator_base::operator=(rhs);
					return (*this);
				};
				self& operator++ () {
					this->increment();
					return (*this);
				};
				self& operator++ (int) {
					this->increment();
					return (*this);
				};
				self& operator-- () {
					this->decrement();
					return (*this);
				};
				self& operator-- (int) {
					this->decrement();
					return (*this);
				};
				bool operator== (const self& other) const {
					return (_List_iterator_base::operator==(other));
				};
				bool operator!= (const self& other) const {
					return (_List_iterator_base::operator!=(other));
				};

				reference operator*() const {
					return (*this->getCurrent()->data);
				};

				pointer operator->() const {
					return (this->getCurrent()->data);
				};
		};
	};

	//Non member overload
	template <class T, class Alloc>
	bool operator== (const List<T,Alloc>& lhs, const List<T,Alloc>& rhs){
		if (lhs.size() != rhs.size())
			return (false);
		typename ft::List<T>::const_iterator lit = lhs.begin();
		typename ft::List<T>::const_iterator rit = rhs.begin();
		while (lit != lhs.end())
		{
			if (*lit != *rit)
				return (false);
			lit++;
			rit++;
		}
		return (true);
	}

	template <class T, class Alloc>
	bool operator!= (const List<T,Alloc>& lhs, const List<T,Alloc>& rhs){
		if (lhs == rhs)
			return (false);
		return (true);
	}

	template <class T, class Alloc>
	bool operator< (const List<T,Alloc>& lhs, const List<T,Alloc>& rhs){
		typename ft::List<T>::const_iterator lit = lhs.begin();
		typename ft::List<T>::const_iterator rit = rhs.begin();
		while (lit != lhs.end())
		{
			if (rit == rhs.end() || *lit > *rit)
				return (false);
			else if (*lit < *rit)
				return (true);
			lit++;
			rit++;
		}
		if (lit == lhs.end() && rit == rhs.end())
			return (false);
		return (true);
	}

	template <class T, class Alloc>
	bool operator<= (const List<T,Alloc>& lhs, const List<T,Alloc>& rhs)
	{
		typename ft::List<T>::const_iterator lit = lhs.begin();
		typename ft::List<T>::const_iterator rit = rhs.begin();
		while (lit != lhs.end())
		{
			if (rit == rhs.end() || *lit > *rit)
				return (false);
			else if (*lit < *rit)
				return (true);
			lit++;
			rit++;
		}
		return (true);
	}

	template <class T, class Alloc>
	bool operator> (const List<T,Alloc>& lhs, const List<T,Alloc>& rhs){
		if (lhs <= rhs)
			return (false);
		return (true);
	}

	template <class T, class Alloc>
	bool operator>= (const List<T,Alloc>& lhs, const List<T,Alloc>& rhs){
		if (lhs < rhs)
			return (false);
		return (true);
	}

	template <class T, class Alloc>
	void swap (List<T,Alloc>& x, List<T,Alloc>& y){
		x.swap(y);
	}
}

#endif // !LIST_HPP
