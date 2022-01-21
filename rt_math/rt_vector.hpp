#ifndef RT_VECTOR_HPP
#define	RT_VECTOR_HPP

#include <stdexcept>
#include <cmath>

namespace rt
{
	template <class T>
	class RTVector
	{
	private:
		T *data;
		size_t dims;


	public:
		RTVector() {
			this->dims = 1;
			this->data = new T[1];
			this->data[0] = 0;
		};
		RTVector(T *x, size_t n) {
			this->dims = n;
			this->data = x;
		}
		RTVector(T x, T y) {
			this->dims = 2;
			this->data = new T[2];
			this->data[0] = x;
			this->data[1] = y;
		}
		RTVector(T x, T y, T z) {
			this->dims = 3;
			this->data = new T[3];
			this->data[0] = x;
			this->data[1] = y;
			this->data[2] = z;
		}
		RTVector(T x, T y, T z, T w) {
			this->dims = 4;
			this->data = new T[4];
			this->data[0] = x;
			this->data[1] = y;
			this->data[2] = z;
			this->data[3] = w;
		}
		~RTVector() {
			delete[] this->data;
		};
		RTVector(const RTVector<T>& rhs) {
			this->data = new T[rhs.dims];
			this->dims = rhs.dims;
			for (size_t i = 0; i < this->dims; ++i)
				this->data[i] = rhs.data[i];
		};
		RTVector<T>& operator= (const RTVector<T>& rhs) {
			if (this->data == NULL)
				this->data = new T[rhs.dims];
			this->dims = rhs.dims;
			for (size_t i = 0; i < this->dims; ++i)
				this->data[i] = rhs.data[i];
			return (*this);
		};

        T* getData() {
            return data;
        }

		size_t getDims() const {
			return this->dims;
		};

		T norm() const {
			T sum = static_cast<T>(0.0);
			for (size_t i = 0; i < dims; ++i)
				sum += this->data[i] * this->data[i];
			return sqrt(sum);
		};
		RTVector<T> get_normalized() const {
			T norm = this->norm();
			if (norm == static_cast<T>(0.0))
				throw std::runtime_error("try to normalize zero vector!");
			return RTVector<T>(*this) * (static_cast<T>(1.0) / norm);
		};
		void normalize() {
			T norm = this->norm();
			if (norm == static_cast<T>(0.0))
				throw std::runtime_error("try to normalize zero vector!");
			for (size_t i = 0; i < dims; ++i)
				this->data[i] = this->data[i] * (static_cast<T>(1.0) / norm);
		};

		T& operator[] (char c) {
			if (c == 'x')
				return (data[0]);
			if (c == 'y')
				return (data[1]);
			if (c == 'z')
				return (data[2]);
			if (c == 'w')
				return (data[3]);
			throw std::runtime_error("Value is out of range");
		};

		T& operator[] (int i) {
			return (this->data[i]);
		};

		T& operator[] (size_t i) {
			return (this->data[i]);
		};

		const T& operator[] (char c) const {
			if (c == 'x')
				return (data[0]);
			if (c == 'y')
				return (data[1]);
			if (c == 'z')
				return (data[2]);
			if (c == 'w')
				return (data[3]);
			throw std::runtime_error("Value is out of range!");
		};

		const T& operator[] (int i) const {
			return (this->data[i]);
		};

		const T& operator[] (size_t i) const {
			return (this->data[i]);
		};

		RTVector<T> operator+ (const RTVector<T> rhs) const {
			if (this->dims != rhs.dims)
				throw std::invalid_argument("Vector dimensions do not match!");
			T* result = new T[this->dims];
			for (size_t i = 0; i < dims; ++i)
				result[i] = this->data[i] + rhs.data[i];
			return (RTVector<T>(result, this->dims));
		};
		RTVector<T> operator- (const RTVector<T> rhs) const {
			if (this->dims != rhs.dims)
				throw std::invalid_argument("Vector dimensions do not match!");
			T* result = new T[this->dims];
			for (size_t i = 0; i < dims; ++i)
				result[i] = this->data[i] - rhs.data[i];
			return (RTVector<T>(result, this->dims));
		};
		RTVector<T> operator* (const RTVector<T> rhs) const {
			if (this->dims != rhs.dims)
				throw std::invalid_argument("Vector dimensions do not match!");
			T* result = new T[this->dims];
			for (size_t i = 0; i < dims; ++i)
				result[i] = this->data[i] * rhs.data[i];
			return (RTVector<T>(result, this->dims));
		};


		static T dot(const RTVector<T> &a, const RTVector<T> &b) {
			if (a.dims != b.dims)
				throw std::invalid_argument("Vector dimensions do not match!");
			T sum = static_cast<T>(0.0);
			for (size_t i = 0; i < a.dims; ++i)
				sum += a[i] * b[i];
			return (sum);
		};
		static RTVector<T> cross(const RTVector<T> &a, const RTVector<T> &b) {
			if (a.dims != b.dims)
				throw std::invalid_argument("Vector dimensions do not match!");
			if (a.dims != 3)
				throw std::invalid_argument("The cross-product can only be computed for 3d vectors!");

			T* result = new T[3];
			result[0] = (a.data[1] * b.data[2]) - (a.data[2] * b.data[1]);
			result[1] = -((a.data[0] * b.data[2]) - (a.data[2] * b.data[0]));
			result[2] = (a.data[0] * b.data[1]) - (a.data[1] * b.data[0]);
			return (RTVector<T>(result, 3));
		};


		template <class U>
		friend RTVector<U> operator* (const U &lhs, const RTVector<U> &rhs);
		template <class U>
		friend RTVector<U> operator* (const RTVector<U> &rhs, const U &lhs);
		template <class U>
		friend RTVector<U> operator/ (const U &lhs, const RTVector<U> &rhs);
		template <class U>
		friend RTVector<U> operator/ (const RTVector<U> &rhs, const U &lhs);

		template <class U>
		friend std::ostream& operator<<(std::ostream& os, const RTVector<U>& vec);
	};

	template <class U>
	RTVector<U> operator* (const U &lhs, const RTVector<U> &rhs)
	{
		U* result = new U[rhs.dims];
		for (size_t i = 0; i < rhs.dims; ++i)
			result[i] = rhs.data[i] * lhs;
		return (RTVector<U>(result, rhs.dims));
	};

	template <class U>
	RTVector<U> operator* (const RTVector<U> &rhs, const U &lhs)
	{
		U* result = new U[rhs.dims];
		for (size_t i = 0; i < rhs.dims; ++i)
			result[i] = rhs.data[i] * lhs;
		return (RTVector<U>(result, rhs.dims));
	};

	template <class U>
	RTVector<U> operator/ (const U &lhs, const RTVector<U> &rhs)
	{
		U* result = new U[rhs.dims];
		for (size_t i = 0; i < rhs.dims; ++i)
			result[i] = rhs.data[i] / lhs;
		return (RTVector<U>(result, rhs.dims));
	};

	template <class U>
	RTVector<U> operator/ (const RTVector<U> &rhs, const U &lhs)
	{
		U* result = new U[rhs.dims];
		for (size_t i = 0; i < rhs.dims; ++i)
			result[i] = rhs.data[i] / lhs;
		return (RTVector<U>(result, rhs.dims));
	};

	template <class U>
	std::ostream& operator<<(std::ostream& os, const RTVector<U>& vec) {
		os << "[ ";
		for (size_t i = 0; i < vec.getDims(); i++) {
			os << vec[i] << " ";
		}
		os << "]";
		return os;
	}

} // namespace rt


#endif // !RT_VECTOR_HPP
