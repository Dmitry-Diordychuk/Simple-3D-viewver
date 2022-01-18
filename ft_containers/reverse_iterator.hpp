/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_iterator.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/24 18:57:02 by kdustin           #+#    #+#             */
/*   Updated: 2021/05/30 00:56:45 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REVERSE_ITERATOR_HPP
# define REVERSE_ITERATOR_HPP

# include "iterator_traits.hpp"

namespace ft
{
	template <class Iterator>
	class reverse_iterator
	{
		private:
			typedef Iterator												iterator_type;
			typedef typename iterator_traits<Iterator>::iterator_category	iterator_category;
			typedef typename iterator_traits<Iterator>::value_type			value_type;
			typedef typename iterator_traits<Iterator>::difference_type		difference_type;
			typedef typename iterator_traits<Iterator>::pointer				pointer;
			typedef typename iterator_traits<Iterator>::reference			reference;
			iterator_type	_base;

		public:
			reverse_iterator() {};
			explicit reverse_iterator(iterator_type it) : _base(it) {};

			template <class Iter>
			reverse_iterator(const reverse_iterator<Iter>& rev_it) : _base(rev_it.base()) {};

			iterator_type base() const{
				return (_base);
			};

			reference operator*() const{
				iterator_type temp = _base;
				--temp;
				return (*temp);
			};

			reverse_iterator operator+(difference_type n) const{
				return (reverse_iterator(_base - n));
			};

			reverse_iterator& operator++(){
				--_base;
				return (*this);
			};
			reverse_iterator operator++(int){
				--_base;
				return (*this);
			};
			reverse_iterator& operator+=(difference_type n) {
				_base -= n;
				return (*this);
			};
			reverse_iterator operator-(difference_type n) const {
				return (reverse_iterator(_base + n));
			};

			reverse_iterator& operator--(){
				++_base;
				return (*this);
			};

			reverse_iterator operator--(int){
				++_base;
				return (*this);
			};

			reverse_iterator& operator-=(difference_type n) {
				_base += n;
				return (*this);
			};

			pointer operator->() const {
				return (&(*_base));
			};

			reference operator[] (difference_type n) const {
				return (*(*this + n));
			};
	};
	template <class Iterator>
	bool operator==(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs){
		return (lhs.base() == rhs.base());
	}

	template <class IteratorL, class IteratorR>
	bool operator==(const reverse_iterator<IteratorL>& lhs, const reverse_iterator<IteratorR>& rhs){
		return (lhs.base() == rhs.base());
	}

	template <class Iterator>
	bool operator!=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs){
		return (lhs.base() != rhs.base());
	}

	template <class IteratorL, class IteratorR>
	bool operator!=(const reverse_iterator<IteratorL>& lhs, const reverse_iterator<IteratorR>& rhs){
		return (lhs.base() != rhs.base());
	}

	template <class Iterator>
	bool operator<(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs);
	template <class Iterator>
	bool operator<=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs);
	template <class Iterator>
	bool operator>(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs);
	template <class Iterator>
	bool operator>=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs);

	template <class Iterator>
	reverse_iterator<Iterator> operator+(typename reverse_iterator<Iterator>::difference_type n,
											const reverse_iterator<Iterator>& rev_it)
	{
		return (reverse_iterator<Iterator>(rev_it.base() - n));
	}

	template <class Iterator>
	typename reverse_iterator<Iterator>::difference_type operator-(const reverse_iterator<Iterator>& lhs,
																	const reverse_iterator<Iterator>& rhs)
	{
		return (rhs.base() - lhs.base());
	}
} // namespace ft

#endif // !REVERSE_ITERATOR_HPP
