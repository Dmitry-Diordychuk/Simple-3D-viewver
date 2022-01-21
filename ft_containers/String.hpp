#ifndef FT_STRING_HPP
#define FT_STRING_HPP

#include "Allocator.hpp"
#include "reverse_iterator.hpp"
#include "type_traits.hpp"
#include <iostream>
#include <exception>

namespace ft
{
	template<class charT, class Alloc = Allocator<charT> >
	class BasicString
	{
	public:
		typedef charT									value_type;
		typedef Alloc									allocator_type;
		typedef charT&									reference;
		typedef const charT&							const_reference;
		typedef charT*									pointer;
		typedef const charT*							const_pointer;
		typedef pointer									iterator;
		typedef const_pointer							const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;
		typedef ptrdiff_t								difference_type;
		typedef size_t									size_type;

		static const size_t npos = -1;

	private:
		size_type		len;
		value_type		*c_string;
		allocator_type	allocator;
		value_type		*c_end;

		size_type	strlen(const_pointer str) const
		{
			pointer tail = const_cast<pointer>(str);
			while (*tail != '\0')
				tail++;
			return (tail - str);
		}

		void deleteBasicString() {
			if (this->c_string)
			{
				destroyCString(this->c_string);
				this->allocator.deallocate(this->c_string, 0);
				this->len = -1;
				this->c_end = NULL;
			}
		}

		void destroyCString(pointer start) {
			if (*start == '\0')
				return;
			size_type i = 0;
			while (start[i] != '\0') {
				allocator.destroy(start + i);
				++i;
			}
			allocator.destroy(this->c_string + i); // delete \0
		}

		void copyCString(const_pointer s) {
			copyCString(s, this->strlen(s));
		}

		void copyCString(const_pointer s, size_type n) {
			this->len = n;
			this->c_string = this->allocator.allocate(this->len + 1);
			pointer dst = this->c_string;
			pointer src = const_cast<pointer>(s);
			size_type i = 0;
			while (i < this->len) {
				this->allocator.construct(dst + i, src[i]);
				++i;
			}
			this->allocator.construct(dst + i, '\0');
			this->c_end = dst + i;
		}

		void copyNChars(size_type n, charT c) {
			this->c_string = this->allocator.allocate(n + 1);
			this->len = n;
			pointer dst = this->c_string;
			size_type i = 0;
			while (i < n) {
				this->allocator.construct(dst + i, c);
				++i;
			}
			this->allocator.construct(dst + i, '\0');
			this->c_end = dst + i;
		}

		void eraseFromStartToFinish(pointer start, pointer finish) {
			if (start == finish)
				return;

			this->memmove(start, finish, strlen(finish) + 1);
			this->len = strlen(this->c_string);
			this->c_end = this->c_string + this->len;

			destroyCString(this->c_end + 1);
		}

		void resizeToGreater(size_type n) {
			size_type new_len = n;
			pointer new_str = allocator.allocate(this->len + n + 1);
			size_type i = 0;
			while (i < this->len) {
				*(new_str + i) = *(this->c_string + i);
				++i;
			}
			while (i < new_len) {
				*(new_str + i) = '\0';
				++i;
			}
			*(new_str + i) = '\0';
			deleteBasicString();
			this->c_string = new_str;
			this->len = new_len;
			this->c_end = c_string + this->len;
		}

		void initEmptyString() {
			this->len = 0;
			this->c_string = this->allocator.allocate(1);
			this->allocator.construct(this->c_string, '\0');
			this->c_end = this->c_string;
		};

		void pushChar(charT c) {
			resizeToGreater(this->len + 1);
			this->c_string[this->len - 1] = c;
		};

		void pushStr(const charT *str) {
			pushStr(str, this->strlen(str));
		}

		void pushStr(const charT *substr, size_type sublen) {
			size_type first_len = this->len;
			size_type second_len = sublen;
			size_type sum_len = first_len + second_len;
			resizeToGreater(sum_len);
			size_type i = first_len;
			while (i < sum_len) {
				this->c_string[i] = *substr;
				++substr;
				++i;
			}
		}

		void pushNChars(size_type n, const charT c) {
			size_type first_len = this->len;
			size_type second_len = n;
			size_type sum_len = first_len + second_len;
			resizeToGreater(sum_len);
			size_type i = first_len;
			while (i < sum_len) {
				this->c_string[i] = c;
				++i;
			}
		}

		void	*memmove(charT *dest, const charT *src, size_t n)
		{
			size_t	i;
			if (n == 0)
				return (dest);
			i = 0;
			if (dest == src)
				return (dest);
			else if (dest < src)
			{
				i = 0;
				while (i < n)
				{
					dest[i] = src[i];
					i++;
				}
			}
			else
			{
				i = n - 1;
				while (i > 0)
				{
					dest[i] = src[i];
					i--;
				}
				dest[i] = src[i];
			}
			return (dest);
		}

		void insertCString(size_type pos, const charT *s, size_type n) {
			resizeToGreater(this->len + n);
			this->memmove(this->c_string + pos + n, this->c_string + pos, strlen(this->c_string + pos));
			size_type i = pos;
			size_type j = 0;
			while (j < n) {
				this->c_string[i] = s[j];
				++i;
				++j;
			}
		}

		void insertNChars(size_type pos, const charT c, size_type n) {
			resizeToGreater(this->len + n);
			this->memmove(this->c_string + pos + n, this->c_string + pos, strlen(this->c_string + pos));
			size_type i = pos;
			size_type j = 0;
			while (j < n) {
				this->c_string[i] = c;
				++i;
				++j;
			}
		}

		void replaceCString(size_type pos, size_type len, const charT *s, size_type n) {
			this->eraseFromStartToFinish(this->c_string + pos, this->c_string + pos + len);
			this->insertCString(pos, s, n);
		}

		void replaceNChars(size_type pos, size_type len, size_type n, charT c) {
			this->eraseFromStartToFinish(this->c_string + pos, this->c_string + pos + len);
			this->insertNChars(pos, c, n);
		}

		size_type	strncmp(const charT *s1, const charT *s2, size_type n1, size_type n2) const
		{
			int n = n1 > n2 ? n1 : n2;
			for (size_type i = 0; i < n; ++i)
				if (s1[i] != s2[i])
					return (s1[i] - s2[i]);
			return (0);
		}


		pointer	strnstr(const_pointer s1, const_pointer s2, size_type ns1, size_type ns2) const
		{
			if (s2[0] == '\0')
				return (const_cast<pointer>(s1));
			for (size_type i = 0; i < ns1; ++i)
				if (!strncmp(s2, s1 + i, ns2, strlen(s1 + i)))
					return (const_cast<pointer>(s1 + i));
			return (NULL);
		}

		charT *rstrnstr(const_pointer s1, const_pointer s2, size_type ns1, size_type ns2) const {
			if (s2[0] == '\0')
				return (const_cast<pointer>(s1));
			size_type i = ns1 - 1;
			for (; i > 0; --i)
				if (!strncmp(s2, s1 + i, ns2, strlen(s1 + i)))
					return (const_cast<pointer>(s1 + i));
			if (!strncmp(s2, s1 + i, ns2, strlen(s1 + i)))
				return (const_cast<pointer>(s1 + i));
			return (NULL);
		}

		charT	*strchr(const charT *s, charT c, size_type n) const
		{
			for (size_type i = 0; i < n; ++i)
				if (s[i] == c)
					return (const_cast<char*>(s + i));
			return (NULL);
		}

		charT	*notstrchr(const charT *s, charT c, size_type n) const
		{
			for (size_type i = 0; i < n; ++i)
				if (s[i] != c)
					return (const_cast<char*>(s + i));
			return (NULL);
		}

		charT	*strrchr(const charT *s, charT c, size_type n) const
		{
			size_type i = n - 1;
			for (; i > 0; --i)
				if (s[i] == c)
					return (const_cast<char*>(s + i));
			if (s[i] == c)
				return (const_cast<char*>(s + i));
			return (NULL);
		}

		charT	*notstrrchr(const charT *s, charT c, size_type n) const
		{
			size_type i = n - 1;
			for (; i > 0; --i)
				if (s[i] != c)
					return (const_cast<char*>(s + i));
			if (s[i] != c)
				return (const_cast<char*>(s + i));
			return (NULL);
		}

		charT	*strpbrk(const charT *s, const charT *charset, size_type n1, size_type n2) const
		{
			for (size_type i = 0; i < n1; ++i)
				for (size_type j = 0; j < n2; ++j)
					if (s[i] == charset[j])
						return (const_cast<charT*>(s + i));
			return (NULL);
		}

		charT	*rstrpbrk(const charT *s, const charT *charset, size_type n1, size_type n2) const
		{
			size_type i = n1 - 1;
			for (; i > 0; --i)
				for (size_type j = 0; j < n2; ++j)
					if (s[i] == charset[j])
						return (const_cast<charT*>(s + i));
			for (size_type j = 0; j < n2; ++j)
				if (s[i] == charset[j])
					return (const_cast<charT*>(s + i));
			return (NULL);
		}


		charT	*notstrpbrk(const charT *s, const charT *charset, size_type n1, size_type n2) const
		{
			for (size_type i = 0; i < n1; ++i)
				if (!strchr(charset, s[i], n2))
					return (const_cast<charT*>(s + i));
			return (NULL);
		}


		charT	*notrstrpbrk(const charT *s, const charT *charset, size_type n1, size_type n2) const
		{
			size_type i = n1 - 1;
			for (; i > 0; --i)
				if (!strrchr(charset, s[i], n2))
					return (const_cast<charT*>(s + i));
			if (!strrchr(charset, s[i], n2))
				return (const_cast<charT*>(s + i));
			return (NULL);
		}

	public:
		BasicString() {
			initEmptyString();
		};

		BasicString(const BasicString& str) {
			copyCString(str.c_string, str.length());
		};

		BasicString(const BasicString& str, size_t pos, size_t len = npos) {
			if (pos > str.len)
				throw std::out_of_range("pos > str length");
			if (pos + len > str.len)
				this->len = len - pos;
			else
				this->len = len;
			this->c_string = this->allocator.allocate(this->len + 1);
			pointer dst = this->c_string;
			pointer src = const_cast<pointer>(str.c_string + pos);
			size_type i = 0;
			while (i < this->len) {
				this->allocator.construct(dst + i, *(src + i));
				++i;
			}
			this->allocator.construct(dst + i, '\0');
			this->c_end = dst + i;
		};

		BasicString(const_pointer s) {
			copyCString(s);
		};

		BasicString(const charT* s, size_t n) {
			this->c_string = this->allocator.allocate(n + 1);
			this->len = n;
			pointer dst = this->c_string;
			pointer src = const_cast<pointer>(s);
			size_type i = 0;
			while (i < n) {
				this->allocator.construct(dst + i, *(src + i));
				++i;
			}
			this->allocator.construct(dst + i, '\0');
			this->c_end = dst + i;
		};

		BasicString(size_t n, charT c) {
			copyNChars(n, c);
		};

		template <class InputIterator>
		BasicString(
			InputIterator first,
			InputIterator last,
			typename ft::enable_if< !ft::is_integral<InputIterator>::value >::type* = 0
		) {
			this->len = last - first;
			this->c_string = this->allocator.allocate(this->len + 1);
			pointer dst = this->c_string;
			while (first != last) {
				this->allocator.construct(dst, *first);
				++first;
				++dst;
			}
			this->allocator.construct(dst, '\0');
			this->c_end = dst;
		}

		~BasicString() {
			deleteBasicString();
		};

		BasicString& operator= (const BasicString& str) {
			// if (*this == str)
			// 	return *this;
			deleteBasicString();
			copyCString(str.c_string, str.length());
			return *this;
		};

		BasicString& operator= (const charT* s) {
			deleteBasicString();
			copyCString(s);
			return *this;
		};

		BasicString& operator= (charT c) {
			deleteBasicString();
			copyNChars(1, c);
			return *this;
		};

		iterator begin() {
			return (this->c_string);
		};

		const_iterator begin() const {
			return (this->c_string);
		};

		iterator end() {
			return (this->c_end);
		};

		const_iterator end() const {
			return (this->c_end);
		};

		reverse_iterator rbegin() {
			reverse_iterator rit(this->end);
			return (rit);
		};

		const_reverse_iterator rbegin() const {
			reverse_iterator rit(this->c_end);
			return (rit);
		};

		reverse_iterator rend() {
			reverse_iterator rit(this->c_string);
			return (rit);
		};

		const_reverse_iterator rend() const {
			reverse_iterator rit(this->c_string);
			return (rit);
		};

		size_t size() const {
			return (this->len);
		};

		size_t length() const {
			return (this->len);
		};

		size_type max_size() const {
			return (allocator.max_size());
		};

		void resize (size_t n) {
			resize(n, '\0');
		};

		void resize (size_t n, char c) {
			if (n < this->len) {
				eraseFromStartToFinish(this->c_string + n, this->c_end);
			} else if (n > this->len) {
				size_type old_len = this->len;
				resizeToGreater(n);
				size_type i = old_len;
				while (i < this->len) {
					this->c_string[i] = c;
					++i;
				}
			}
		};

		size_type capacity() const {
			return this->len;
		};

		void reserve(size_type) {
			return;
		};

		void clear() {
			deleteBasicString();
			initEmptyString();
		};

		bool empty() const {
			return (!this->len);
		};

		reference operator[] (size_type pos) {
			return (this->c_string[pos]);
		};

		const_reference operator[] (size_type pos) const {
			return (this->c_string[pos]);
		};

		reference at (size_type pos) {
			if (this->c_string + pos < this->c_string || pos > this->len - 1) {
				throw std::out_of_range("pos > str length");
			}
			return (this->c_string[pos]);
		};

		const_reference at (size_type pos) const {
			if (pos > this->len - 1) {
				throw std::out_of_range("pos > str length");
			}
			return (this->c_string[pos]);
		};

		BasicString& operator+= (const BasicString& str) {
			pushStr(str.c_string);
			return (*this);
		};

		BasicString& operator+= (const charT* s) {
			pushStr(s);
			return (*this);
		};

		BasicString& operator+= (const charT c) {
			pushChar(c);
			return (*this);
		};

		BasicString& append(const BasicString& str) {
			pushStr(str.c_string);
			return (*this);
		};

		BasicString& append (const BasicString& str, size_type subpos, size_type sublen) {
			if (this->strlen(str.c_string + subpos) < sublen || sublen == this->npos)
				pushStr(str.c_string + subpos);
			else
				pushStr(str.c_string + subpos, sublen);
			return (*this);
		};

		BasicString& append (const charT* s) {
			pushStr(s);
			return (*this);
		};

		BasicString& append (const charT* s, size_type n){
			pushStr(s, n);
			return (*this);
		};

		BasicString& append (size_type n, charT c) {
			pushNChars(n, c);
			return (*this);
		};

		template <class InputIterator>
  		BasicString& append (InputIterator first, InputIterator last) {
			pushStr(first, last - first);
			return (*this);
		};

		void push_back(charT c) {
			pushChar(c);
		}

		BasicString& assign (const BasicString& str) {
			deleteBasicString();
			copyCString(str.c_string);
			return (*this);
		};

		BasicString& assign (const BasicString& str, size_type subpos, size_type sublen) {
			deleteBasicString();
			if (str.length() < sublen - subpos || sublen == this->npos)
				copyCString(str.c_string + subpos);
			else
				copyCString(str.c_string + subpos, sublen);
			return (*this);
		};

		BasicString& assign (const charT* s) {
			deleteBasicString();
			copyCString(s);
			return (*this);
		};

		BasicString& assign (const charT* s, size_type n) {
			deleteBasicString();
			copyCString(s, n);
			return (*this);
		};

		BasicString& assign (size_type n, charT c) {
			deleteBasicString();
			copyNChars(n, c);
			return (*this);
		};

		template <class InputIterator>
   		BasicString& assign (InputIterator first, InputIterator last) {
			deleteBasicString();
			copyCString(first, last - first);
			return (*this);
		};

		BasicString& insert (size_type pos, const BasicString& str) {
			insertCString(pos, str.c_string, str.length());
			return (*this);
		};

		BasicString& insert (size_type pos, const BasicString& str, size_type subpos, size_type sublen) {
			insertCString(pos, str.c_string + subpos, sublen);
			return (*this);
		};

		BasicString& insert (size_type pos, const charT* s) {
			insertCString(pos, s, this->strlen(s));
			return (*this);
		};

		BasicString& insert (size_type pos, const charT* s, size_type n) {
			insertCString(pos, s, n);
			return (*this);
		};

		BasicString& insert (size_type pos, size_type n, charT c) {
			insertNChars(pos, c, n);
			return (*this);
		};
        void insert (iterator p, size_type n, charT c) {
			insertNChars(p - this->c_string, c, n);
		};

		iterator insert (iterator p, charT c) {
			size_type pos = p - this->c_string;
			insertNChars(pos, c, 1);
			return (this->c_string + pos);
		};

		template <class InputIterator>
		void insert (iterator p, InputIterator first, InputIterator last) {
			insertCString(p - this->c_string, first, last - first);
		};

		BasicString& erase (size_type pos = 0, size_type len = npos) {
			if (len == npos) {
				eraseFromStartToFinish(this->c_string + pos, this->c_string + pos + strlen(c_string + pos));
			} else {
				eraseFromStartToFinish(this->c_string + pos, this->c_string + pos + len);
			}
			return (*this);
		};
		iterator erase (iterator p) {
			eraseFromStartToFinish(p, p + 1);
			return (p);
		};
		iterator erase (iterator first, iterator last) {
			eraseFromStartToFinish(first, last);
			return (first);
		};

		BasicString& replace (size_type pos, size_type len, const BasicString& str) {
			this->replaceCString(pos, len, str.c_string, str.len);
			return (*this);
		};
		BasicString& replace (iterator i1,   iterator i2,   const BasicString& str) {
			this->replaceCString(i1 - this->c_string, i2 - i1, str.c_string, str.len);
			return (*this);
		};
		BasicString& replace (size_type pos, size_type len, const BasicString& str,
							size_type subpos, size_type sublen)
		{
			this->replaceCString(pos, len, str.c_string + subpos, sublen);
			return (*this);
		};
		BasicString& replace (size_type pos, size_type len, const charT* s) {
			this->replaceCString(pos, len, s, strlen(s));
			return (*this);
		};
		BasicString& replace (iterator i1,   iterator i2,   const charT* s) {
			this->replaceCString(i1 - this->c_string, i2 - i1, s, strlen(s));
			return (*this);
		}
		BasicString& replace (size_type pos, size_type len, const charT* s, size_type n) {
			this->replaceCString(pos, len, s, n);
			return (*this);
		};
		BasicString& replace (iterator i1,   iterator i2,   const charT* s, size_type n) {
			this->replaceCString(i1 - this->c_string, i2 - i1, s, n);
			return (*this);
		};
		BasicString& replace (size_type pos, size_type len, size_type n, charT c) {
			this->replaceNChars(pos, len, n, c);
			return (*this);
		};
		BasicString& replace (iterator i1,   iterator i2,   size_type n, charT c) {
			this->replaceNChars(i1 - this->c_string, i2 - i1, n, c);
			return (*this);
		};
		template <class InputIterator>
		BasicString& replace (iterator i1, iterator i2, InputIterator first, InputIterator last) {
			this->replaceCString(i1 - this->c_string, i2 - i1, first, last - first);
			return (*this);
		};

		void swap(BasicString& str) {
			charT *temp_str = str.c_string;
			charT *temp_end = str.c_end;
			size_type temp_len = str.len;

			str.c_string = this->c_string;
			str.c_end = this->c_end;
			str.len = this->len;

			this->c_string = temp_str;
			this->c_end = temp_end;
			this->len = temp_len;
		}

		const char* c_str() const {
			return (this->c_string);
		};

		const charT* data() const {
			return (this->c_string);
		};

		size_type copy(charT* s, size_type len, size_type pos = 0) const {
			if (pos + len > this->len) {
				len = this->len - pos;
			}
			size_type i = 0;
			while (i < len) {
				s[i] = this->c_string[pos];
				++i;
				++pos;
			}
			s[i] = '\0';
			return (i);
		};

		size_type find (const BasicString& str, size_type pos = 0) const {
			if (pos > this->len)
				return (this->npos);
			charT* result = this->strnstr(this->c_string + pos, str.c_string, this->len - pos, str.len);
			if (result == NULL) {
				return (this->npos);
			}
			return (result - this->c_string);
		};
		size_type find (const charT* s, size_type pos = 0) const {
			if (pos > this->len)
				return (this->npos);
			charT* result = this->strnstr(this->c_string + pos, s, this->len - pos, this->strlen(s));
			if (result == NULL) {
				return (this->npos);
			}
			return (result - this->c_string);
		};
		size_type find (const charT* s, size_type pos, size_type n) const {
			if (pos > this->len)
				return (this->npos);
			charT* result = this->strnstr(this->c_string + pos, s, this->len - pos, n);
			if (result == NULL) {
				return (this->npos);
			}
			return (result - this->c_string);
		};
		size_type find (charT c, size_type pos = 0) const {
			if (pos > this->len)
				return (this->npos);
			charT* result = this->strchr(this->c_string + pos, c, this->len);
			if (result == NULL) {
				return (this->npos);
			}
			return (result - this->c_string);
		};

		size_type rfind (const BasicString& str, size_type pos = npos) const {
			if (pos > this->len)
				pos = this->len - 1;
			charT* result = this->rstrnstr(this->c_string, str.c_string, pos + 1, str.len);
			if (result == NULL) {
				return (this->npos);
			}
			return (result - this->c_string);
		};
		size_type rfind (const charT* s, size_type pos = npos) const {
			if (pos > this->len)
				pos = this->len - 1;
			charT* result = this->rstrnstr(this->c_string, s, pos + 1, this->strlen(s));
			if (result == NULL) {
				return (this->npos);
			}
			return (result - this->c_string);
		};
		size_type rfind (const charT* s, size_type pos, size_type n) const {
			if (pos > this->len)
				pos = this->len - 1;
			charT* result = this->rstrnstr(this->c_string, s, pos + 1, n);
			if (result == NULL) {
				return (this->npos);
			}
			return (result - this->c_string);
		};
		size_type rfind (charT c, size_type pos = npos) const {
			if (pos > this->len)
				pos = this->len - 1;
			charT* result = this->strrchr(this->c_string, c, pos + 1);
			if (result == NULL) {
				return (this->npos);
			}
			return (result - this->c_string);
		};

		size_type find_first_of (const BasicString& str, size_type pos = 0) const {
			if (pos > this->len)
				return (npos);
			charT* result = this->strpbrk(this->c_string + pos, str.c_string, this->len - pos, str.len);
			if (result == NULL)
				return (npos);
			return (result - this->c_string);
		};
		size_type find_first_of (const charT* s, size_type pos = 0) const {
			if (pos > this->len)
				return (npos);
			charT* result = this->strpbrk(this->c_string + pos, s, this->len - pos, this->strlen(s));
			if (result == NULL)
				return (npos);
			return (result - this->c_string);
		};
		size_type find_first_of (const charT* s, size_type pos, size_type n) const {
			if (pos > this->len)
				return (npos);
			charT* result = this->strpbrk(this->c_string + pos, s, this->len - pos, n);
			if (result == NULL)
				return (npos);
			return (result - this->c_string);
		};
		size_type find_first_of (charT c, size_type pos = 0) const {
			if (pos > this->len)
				return (npos);
			return this->find(c, pos);
		};

		size_type find_last_of (const BasicString& str, size_type pos = npos) const {
			if (pos > this->len)
				pos = this->len - 1;
			charT* result = this->rstrpbrk(this->c_string, str.c_string, pos + 1, str.len);
			if (result == NULL)
				return (npos);
			return (result - this->c_string);
		};
		size_type find_last_of (const charT* s, size_type pos = npos) const {
			if (pos > this->len)
				pos = this->len - 1;
			charT* result = this->rstrpbrk(this->c_string, s, pos + 1, this->strlen(s));
			if (result == NULL)
				return (npos);
			return (result - this->c_string);
		};
		size_type find_last_of (const charT* s, size_type pos, size_type n) const {
			if (pos > this->len)
				pos = this->len - 1;
			charT* result = this->strpbrk(this->c_string, s, pos + 1, n);
			if (result == NULL)
				return (npos);
			return (result - this->c_string);
		};
		size_type find_last_of (charT c, size_type pos = npos) const {
			if (pos > this->len)
				pos = this->len - 1;
			return this->rfind(c, pos + 1);
		};

		size_type find_first_not_of (const BasicString& str, size_type pos = 0) const {
			if (pos > this->len)
				return (npos);
			charT* result = this->notstrpbrk(this->c_string + pos, str.c_string, this->len - pos, str.len);
			if (result == NULL)
				return (npos);
			return (result - this->c_string);
		};
		size_type find_first_not_of (const charT* s, size_type pos = 0) const {
			if (pos > this->len)
				return (npos);
			charT* result = this->notstrpbrk(this->c_string + pos, s, this->len - pos, this->strlen(s));
			if (result == NULL)
				return (npos);
			return (result - this->c_string);
		};
		size_type find_first_not_of (const charT* s, size_type pos, size_type n) const {
			if (pos > this->len)
				return (npos);
			charT* result = this->notstrpbrk(this->c_string + pos, s, this->len - pos, n);
			if (result == NULL)
				return (npos);
			return (result - this->c_string);
		};
		size_type find_first_not_of (charT c, size_type pos = 0) const {
			if (pos > this->len)
				return (this->npos);
			charT* result = this->notstrchr(this->c_string + pos, c, this->len);
			if (result == NULL) {
				return (this->npos);
			}
			return (result - this->c_string);
		};

		size_type find_last_not_of (const BasicString& str, size_type pos = npos) const {
			if (pos > this->len)
				pos = this->len - 1;
			charT* result = this->notrstrpbrk(this->c_string, str.c_string, pos + 1, str.len);
			if (result == NULL)
				return (npos);
			return (result - this->c_string);
		};
		size_type find_last_not_of (const charT* s, size_type pos = npos) const {
			if (pos > this->len)
				pos = this->len - 1;
			charT* result = this->notrstrpbrk(this->c_string, s, pos + 1, this->strlen(s));
			if (result == NULL)
				return (npos);
			return (result - this->c_string);
		};
		size_type find_last_not_of (const charT* s, size_type pos, size_type n) const {
			if (pos > this->len)
				pos = this->len - 1;
			charT* result = this->notrstrpbrk(this->c_string, s, pos + 1, n);
			if (result == NULL)
				return (npos);
			return (result - this->c_string);
		};
		size_type find_last_not_of (charT c, size_type pos = npos) const {
			if (pos > this->len)
				pos = this->len - 1;
			charT* result = this->notstrrchr(this->c_string, c, pos + 1);
			if (result == NULL) {
				return (this->npos);
			}
			return (result - this->c_string);
		};

		BasicString substr (size_type pos = 0, size_type len = npos) const {
			if (pos > this->len)
				throw std::out_of_range("pos > str length");
			if (len != npos)
				return BasicString(this->c_string + pos, len);
			return BasicString(this->c_string + pos);
		};

		int compare (const BasicString& str) const {
			return (this->strncmp(this->c_string, str.c_string, this->len, str.len));
		};
		int compare (size_type pos, size_type len, const BasicString& str) const{
			if (pos > this->len)
				throw std::out_of_range("pos > str length");
			if (len > this->len - pos)
				len = this->len - pos;
			return (this->strncmp(this->c_string + pos, str.c_string, len, str.len));
		};
		int compare (size_type pos, size_type len, const BasicString& str, size_type subpos, size_type sublen) const
		{
			if (pos > this->len || subpos > str.len)
				throw std::out_of_range("pos > str length");
			if (len > this->len - pos)
				len = this->len - pos;
			if (sublen > str.len - subpos)
				sublen = str.len - subpos;
			return (this->strncmp(this->c_string + pos, str.c_string + subpos, len, sublen));
		};
		int compare (const charT* s) const {
			return (this->strncmp(this->c_string, s, this->len, strlen(s)));
		};
		int compare (size_type pos, size_type len, const charT* s) const {
			if (pos > this->len)
				throw std::out_of_range("pos > str length");
			if (len > this->len - pos)
				len = this->len - pos;
			return (this->strncmp(this->c_string + pos, s, len, strlen(s)));
		};
		int compare (size_type pos, size_type len, const charT* s, size_type n) const {
			if (pos > this->len)
				throw std::out_of_range("pos > str length");
			if (len > this->len - pos)
				len = this->len - pos;
			return (this->strncmp(this->c_string + pos, s, len, n));
		};

		template <typename charU>
		friend std::ostream& operator<<(std::ostream& os, const BasicString<charU>& str);
	};
	typedef BasicString<char>	String;

	template <class charT, class Alloc>
  	BasicString<charT,Alloc> operator+ (const BasicString<charT,Alloc>& lhs, const BasicString<charT,Alloc>& rhs) {
		BasicString<charT,Alloc> copy(lhs);
		copy.append(rhs);
		return copy;
	}
	template <class charT, class Alloc>
  	BasicString<charT, Alloc> operator+ (const BasicString<charT,Alloc>& lhs, const charT* rhs) {
		BasicString<charT,Alloc> copy(lhs);
		copy.append(rhs);
		return copy;
	}
	template <class charT, class Alloc>
  	BasicString<charT, Alloc> operator+ (const charT* lhs, const BasicString<charT,Alloc>& rhs) {
		BasicString<charT,Alloc> copy(lhs);
		copy.append(rhs);
		return copy;
	}
	template <class charT, class Alloc>
  	BasicString<charT, Alloc> operator+ (const BasicString<charT,Alloc>& lhs, charT rhs) {
		BasicString<charT,Alloc> copy(lhs);
		copy.push_back(rhs);
		return copy;
	}
	template <class charT, class Alloc>
  	BasicString<charT, Alloc> operator+ (charT lhs, const BasicString<charT,Alloc>& rhs) {
		BasicString<charT,Alloc> copy(lhs);
		copy.append(rhs);
		return copy;
	}

	template <class charT, class Alloc>
  	bool operator== (const BasicString<charT,Alloc>& lhs, const BasicString<charT,Alloc>& rhs) {
		return (!lhs.compare(rhs));
	}
	template <class charT, class Alloc>
  	bool operator== (const charT* lhs, const BasicString<charT,Alloc>& rhs) {
		return (!rhs.compare(lhs));
	}
	template <class charT, class Alloc>
  	bool operator== (const BasicString<charT,Alloc>& lhs, const charT* rhs) {
		return (!lhs.compare(rhs));
	}
	template <class charT, class Alloc>
  	bool operator!= (const BasicString<charT,Alloc>& lhs, const BasicString<charT,Alloc>& rhs) {
		return (lhs.compare(rhs));
	}
	template <class charT, class Alloc>
  	bool operator!= (const charT* lhs, const BasicString<charT,Alloc>& rhs) {
		return (rhs.compare(lhs));
	}
	template <class charT, class Alloc>
  	bool operator!= (const BasicString<charT,Alloc>& lhs, const charT* rhs) {
		return (lhs.compare(rhs));
	}
	template <class charT, class Alloc>
  	bool operator<  (const BasicString<charT,Alloc>& lhs, const BasicString<charT,Alloc>& rhs) {
		if (lhs.compare(rhs) < 0)
			return (true);
		return (false);
	}
	template <class charT, class Alloc>
  	bool operator<  (const charT* lhs, const BasicString<charT,Alloc>& rhs) {
		if (rhs.compare(lhs) >= 0)
			return (true);
		return (false);
	}
	template <class charT, class Alloc>
  	bool operator<  (const BasicString<charT,Alloc>& lhs, const charT* rhs) {
		if (lhs.compare(rhs) < 0)
			return (true);
		return (false);
	}
	template <class charT, class Alloc>
  	bool operator<= (const BasicString<charT,Alloc>& lhs, const BasicString<charT,Alloc>& rhs) {
		if (lhs.compare(rhs) <= 0)
			return (true);
		return (false);
	}
	template <class charT, class Alloc>
  	bool operator<= (const charT* lhs, const BasicString<charT,Alloc>& rhs) {
		if (rhs.compare(lhs) > 0)
			return (true);
		return (false);
	}
	template <class charT, class Alloc>
  	bool operator<= (const BasicString<charT,Alloc>& lhs, const charT* rhs) {
		if (lhs.compare(rhs) <= 0)
			return (true);
		return (false);
	}
	template <class charT, class Alloc>
  	bool operator>  (const BasicString<charT,Alloc>& lhs, const BasicString<charT,Alloc>& rhs) {
		if (lhs.compare(rhs) > 0)
			return (true);
		return (false);
	}
	template <class charT, class Alloc>
  	bool operator>  (const charT* lhs, const BasicString<charT,Alloc>& rhs) {
		if (rhs.compare(lhs) <= 0)
			return (true);
		return (false);
	}
	template <class charT, class Alloc>
  	bool operator>  (const BasicString<charT,Alloc>& lhs, const charT* rhs) {
		if (lhs.compare(rhs) > 0)
			return (true);
		return (false);
	}
	template <class charT, class Alloc>
  	bool operator>= (const BasicString<charT,Alloc>& lhs, const BasicString<charT,Alloc>& rhs) {
		if (lhs.compare(rhs) >= 0)
			return (true);
		return (false);
	}
	template <class charT, class Alloc>
  	bool operator>= (const charT* lhs, const BasicString<charT,Alloc>& rhs) {

		if (rhs.compare(lhs) < 0)
			return (true);
		return (false);
	}
	template <class charT, class Alloc>
  	bool operator>= (const BasicString<charT,Alloc>& lhs, const charT* rhs) {
		if (lhs.compare(rhs) >= 0)
			return (true);
		return (false);
	}

	template <class charT, class Alloc>
  	void swap (BasicString<charT,Alloc>& x, BasicString<charT,Alloc>& y) {
		x.swap(y);
	}

	template <class charT, class Alloc>
  	std::basic_istream<charT>& operator>> (std::basic_istream<charT>& is, BasicString<charT,Alloc>& str) {
		typename std::basic_istream<charT>::sentry ok(is, false);
		if (ok)
		{
			try {
				str.erase();
				charT buffer[128];
				size_t len = 0;

				int c = is.rdbuf()->sgetc();
				while (c != -1 && c != 32 && (c < 9 || c > 13)) {
					if (len == sizeof(buffer)/sizeof(charT))
					{
						str.append(buffer, sizeof(buffer)/sizeof(charT));
						len = 0;
					}
					buffer[len++] = (charT)c;
					c = is.rdbuf()->snextc();
				}
				str.append(buffer, len);
			} catch(...) {
				is.setstate(std::basic_istream<charT>::ios_base::badbit);
				throw;
			}
		}
		return (is);
	}

	template<class charT>
	std::ostream& operator<< (std::ostream& os, const BasicString<charT>& str) {
		os << str.c_str();
		return (os);
	}

	template <class charT, class Alloc>
  	std::basic_istream<charT>& getline(std::basic_istream<charT>& is, BasicString<charT,Alloc>& str, charT delim) {
		str.clear();
		char ch;
		while (is.get(ch) && ch != delim)
			str.push_back(ch);
		return (is);
	}
	template <class charT, class Alloc>
 	std::basic_istream<charT>& getline (std::basic_istream<charT>& is, BasicString<charT,Alloc>& str) {
		str.clear();
		char ch;
		while (is.get(ch) && ch != '\n')
			str.push_back(ch);
		return (is);
	}
} // namespace ft


#endif // !STRING
