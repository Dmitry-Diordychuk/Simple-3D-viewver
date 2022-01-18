/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_traits.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 14:09:17 by kdustin           #+#    #+#             */
/*   Updated: 2021/05/29 18:02:40 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

namespace ft
{
	template<bool B, class T = void>
	struct enable_if {};

	template<class T>
	struct enable_if<true, T> { typedef T type; };

	template <typename T>
	class is_integral
	{
		public:
			static const bool value = false;
	};

	#define FT_IS_INTEGRAL(T)					\
		template<> class is_integral<T>			\
		{										\
			public:								\
				static const  bool value = true;\
		};

	FT_IS_INTEGRAL(bool)
	FT_IS_INTEGRAL(char)
	FT_IS_INTEGRAL(signed char)
	FT_IS_INTEGRAL(unsigned char)
	FT_IS_INTEGRAL(wchar_t)
	FT_IS_INTEGRAL(signed short)
	FT_IS_INTEGRAL(unsigned short)
	FT_IS_INTEGRAL(signed int)
	FT_IS_INTEGRAL(unsigned int)
	FT_IS_INTEGRAL(signed long)
	FT_IS_INTEGRAL(unsigned long)
	#undef FT_IS_INTEGRAL
} // namespace ft

#endif // !TYPE_TRAITS_HPP
