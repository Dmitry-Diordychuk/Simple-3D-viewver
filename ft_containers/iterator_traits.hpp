/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator_traits.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 12:37:35 by kdustin           #+#    #+#             */
/*   Updated: 2021/05/29 18:40:27 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITERATOR_TRAITS
# define ITERATOR_TRAITS

# include <cstddef>
# include "iterator.hpp"

namespace ft
{
	template <typename Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
	};

	template <typename Iterator>
	struct iterator_traits<Iterator*>
	{
		typedef std::ptrdiff_t							difference_type;
		typedef Iterator								value_type;
		typedef Iterator*								pointer;
		typedef Iterator&								reference;
		typedef ft::random_access_iterator_tag			iterator_category;
	};

	template <typename Iterator>
	struct iterator_traits<const Iterator*>
	{
		typedef std::ptrdiff_t							difference_type;
		typedef Iterator								value_type;
		typedef const Iterator*							pointer;
		typedef const Iterator&							reference;
		typedef ft::random_access_iterator_tag			iterator_category;
	};
}

#endif
