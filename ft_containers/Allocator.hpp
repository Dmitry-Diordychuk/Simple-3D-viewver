#ifndef ALLOCATOR
#define ALLOCATOR

#include <cstddef>
#include <limits>

namespace ft {
	template <class T>
	class Allocator
	{
	private:
	public:
		typedef T			value_type;
		typedef T*			pointer;
		typedef T&			reference;
		typedef const T*	const_pointer;
		typedef const T&	const_reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

		template <class U>
		struct rebind {
			typedef Allocator<U> other;
		};

		pointer			address(reference value) const {
			return &value;
		};
		const_pointer	address(const_reference value) const {
			return &value;
		};

		Allocator() throw() {};
		Allocator(const Allocator&) throw() {};
		template <class U>
		Allocator(const Allocator<U>&) throw() {};

		~Allocator() throw() {};

		Allocator& operator=(const Allocator&) = default;

		size_type max_size() const throw() {
			return (std::numeric_limits<size_type>::max() / sizeof(T));
		};

		pointer allocate(size_type num, const void * hint=0) {
			return (pointer)(::operator new ((num + (size_t)hint) * sizeof(T)));
		};

		void construct(pointer p, const_reference value) {
			new((void*)p)T(value);
		};

		void destroy(pointer p) {
			p->~T();
		};

		void deallocate(pointer p, size_type) {
			::operator delete((void*)p);
		};
	};
}

#endif // !ALLOCATOR
