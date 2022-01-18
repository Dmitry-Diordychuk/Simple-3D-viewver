/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pair.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/25 13:11:33 by kdustin           #+#    #+#             */
/*   Updated: 2021/05/26 16:39:24 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PAIR_HPP
#define PAIR_HPP

namespace ft
{
	template <typename T1, typename T2>
	struct Pair
	{
		typedef T1	first_type;
		typedef T2	second_type;

		first_type	first;
		second_type	second;

		Pair() : first(T1()), second(T2()) {};
		template<class U, class V>
		Pair (const Pair<U, V>& pr) : first(pr.first), second(pr.second) {};
		Pair (const first_type& a, const second_type& b) : first(a), second(b) {};
		Pair& operator=(const Pair& pr) {
			if (this == &pr)
				return (*this);
			first = pr.first;
			second = pr.second;
			return (*this);
		};
	};

	template <class T1, class T2>
	bool operator== (const Pair<T1,T2>& lhs, const Pair<T1,T2>& rhs)
	{ return lhs.first==rhs.first && lhs.second==rhs.second; }

	template <class T1, class T2>
	bool operator!= (const Pair<T1,T2>& lhs, const Pair<T1,T2>& rhs)
	{ return !(lhs==rhs); }

	template <class T1, class T2>
	bool operator<  (const Pair<T1,T2>& lhs, const Pair<T1,T2>& rhs)
	{ return lhs.first<rhs.first || (!(rhs.first<lhs.first) && lhs.second<rhs.second); }

	template <class T1, class T2>
	bool operator<= (const Pair<T1,T2>& lhs, const Pair<T1,T2>& rhs)
	{ return !(rhs<lhs); }

	template <class T1, class T2>
	bool operator>  (const Pair<T1,T2>& lhs, const Pair<T1,T2>& rhs)
	{ return rhs<lhs; }

	template <class T1, class T2>
	bool operator>= (const Pair<T1,T2>& lhs, const Pair<T1,T2>& rhs)
	{ return !(lhs<rhs); }
}

#endif // !PAIR_HPP

