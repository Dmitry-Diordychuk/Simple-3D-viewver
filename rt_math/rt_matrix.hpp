#ifndef RT_MATRIX_HPP
#define RT_MATRIX_HPP

#include <stdexcept>
#include "rt_vector.hpp"

namespace rt
{
	template <class T>
	class RTMatrix
	{
	private:
		T *data;
		size_t n;
		size_t rows;
		size_t cols;

	public:
		RTMatrix() {
			this->rows = 1;
			this->cols = 1;
			this->n = 1;
			this->data = new T[1];
			this->data[0] = static_cast<T>(0.0);
		};
		RTMatrix(size_t rows, size_t cols) {
			this->rows = rows;
			this->cols = cols;
			this->n = rows * cols;
			this->data = new T[n];
			for (size_t i = 0; i < n; ++i)
				data[i] = static_cast<T>(0.0);
		};
		RTMatrix(size_t rows, size_t cols, const T *data) {
			this->rows = rows;
			this->cols = cols;
			this->n = rows * cols;
			this->data = new T[n];
			for (size_t i = 0; i < n; ++i)
				this->data[i] = data[i];
		};
		RTMatrix(size_t rows, size_t cols, size_t n, T *data) {
			this->rows = rows;
			this->cols = cols;
			this->n = n;
			this->data = data;
		}
		~RTMatrix() {
			delete[] this->data;
		};
		RTMatrix(const RTMatrix<T>& rhs) {
			this->rows = rhs.rows;
			this->cols = rhs.cols;
			this->n = rhs.n;
			this->data = new T[n];
			for (size_t i = 0; i < n; ++i)
				this->data[i] = rhs.data[i];
		};
		RTMatrix& operator=(const RTMatrix<T>& rhs) {
			if (*this == rhs)
				return (*this);
			this->rows = rhs.rows;
			this->cols = rhs.cols;
			this->n = rhs.n;
			this->data = new T[n];
			for (size_t i = 0; i < n; ++i)
				this->data[i] = rhs.data[i];
			return (*this);
		};

		T* getData() {
			return this->data;
		}

		void toIdentity() {
			if (this->rows != this->cols)
				throw std::runtime_error("Identity matrix must be square!");
			for (size_t i = 0; i < this->rows; ++i)
				for (size_t j = 0; j < this->cols; ++j)
					if (i == j)
						(*this)[i][j] = static_cast<T>(1.0);
					else
						(*this)[i][j] = static_cast<T>(0.0);
		}

		void inverse() {
			if (this->rows != this->cols)
				throw std::runtime_error("Can't invese not square matrix!");

			RTMatrix<T> identity = RTMatrix<T>(this->rows, this->cols);
			identity.toIdentity();
			join(identity);

			// Двигаемся по диагонали (прямой ход)
			for (size_t diag = 0; diag < this->rows; ++diag)
			{
				// Переставляет на первую строку подматрицы самое большое число
				size_t max = findRowWithMaxElement(diag, diag);
				if (max != diag)
					swapRow(diag, max);
				if ((*this)[diag][diag] == static_cast<T>(0.0))
					throw std::runtime_error("Matrix is singular!");
				// Приводим диагональный элемент к 1
				if ((*this)[diag][diag] != 0)
					multRow(diag, static_cast<T>(1.0)/(*this)[diag][diag]);
				// Обрабатываем все строки ниже диагональной
				for (size_t i = diag + 1; i < this->rows; ++i)
					multAdd(i, diag, (*this)[i][diag] > 0 ? -(*this)[i][diag] : (*this)[i][diag]);
			}
			// Обратный ход
			for (size_t diag = this->rows - 1; diag > 0; --diag)
			{
				for (size_t i = diag - 1; i > 0; --i)
				{
					if ((*this)[i][diag] != 0)
						multAdd(i, diag, (*this)[i][diag] > 0 ? -(*this)[i][diag] : (*this)[i][diag]);
				}
				if ((*this)[0][diag] != 0)
					multAdd(0, diag, (*this)[0][diag] > 0 ? -(*this)[0][diag] : (*this)[0][diag]);
			}

			*this = separate(this->cols / 2);
		}

		T* operator[](size_t i) {
			return (this->data + i * this->cols);
		};
		T* operator[](size_t i) const {
			return (this->data + i * this->cols);
		};
		size_t getRows() {return this->rows;};
		size_t getCols() {return this->cols;};

		bool operator== (const RTMatrix<T>& rhs) {
			return (compare(rhs, static_cast<T>(1e-9)));
		};
		bool compare(const RTMatrix<T>& rhs, float error) {
			if (this->rows != rhs.rows || this->cols != rhs.cols)
				return false;
			T sum = static_cast<T>(0.0);
			for (size_t i = 0; i < n; ++i)
				sum += (data[i] - rhs.data[i]) * (data[i] - rhs.data[i]);
			float err = sqrt(sum / n);
			if (err < error)
				return true;
			return false;
		};

		RTMatrix<T> separate(size_t col_index) {
			RTMatrix<T> m1 = RTMatrix(this->rows, col_index);
			RTMatrix<T> m2 = RTMatrix(this->rows, this->cols - col_index);

			for (size_t i = 0; i < rows; ++i)
				for (size_t j = 0; j < cols; ++j)
					if (j < col_index)
						m1[i][j] = (*this)[i][j];
					else
						m2[i][j - col_index] = (*this)[i][j];
			*this = m1;
			return (m2);
		};

		template <class U>
		friend RTMatrix<U> operator+ (const RTMatrix<U>& lhs, const RTMatrix<U>& rhs);
		template <class U>
		friend RTMatrix<U> operator+ (const U& lhs, const RTMatrix<U>& rhs);
		template <class U>
		friend RTMatrix<U> operator+ (const RTMatrix<U>& rhs, const U& lhs);

		template <class U>
		friend RTMatrix<U> operator- (const RTMatrix<U>& lhs, const RTMatrix<U>& rhs);
		template <class U>
		friend RTMatrix<U> operator- (const U& lhs, const RTMatrix<U>& rhs);
		template <class U>
		friend RTMatrix<U> operator- (const RTMatrix<U>& rhs, const U& lhs);

		template <class U>
		friend RTMatrix<U> operator* (const RTMatrix<U>& lhs, const RTMatrix<U>& rhs);
		template <class U>
		friend RTMatrix<U> operator* (const U& lhs, const RTMatrix<U>& rhs);
		//template <class U>
		//friend RTMatrix<U> operator* (const RTMatrix<U>& rhs, const U& lhs);

		template <class U>
		friend rt::RTVector<U> operator* (const RTMatrix<U>& lhs, const rt::RTVector<U>& rhs);

	private:
		void swapRow(size_t i_1, size_t i_2) {
			T *temp = new T[this->cols];
			for (size_t k = 0; k < this->cols; ++k)
				temp[k] = (*this)[i_1][k];
			for (size_t k = 0; k < this->cols; ++k)
				(*this)[i_1][k] = (*this)[i_2][k];
			for (size_t k = 0; k < this->cols; ++k)
				(*this)[i_2][k] = temp[k];
			delete[] temp;
		};
		void multAdd(size_t i, size_t j, T multiplier) {
			for (size_t k = 0; k < this->cols; ++k)
				(*this)[i][k] += (*this)[j][k] * multiplier;
		};
		void multRow(size_t i, T multiplier) {
			for (size_t k = 0; k < this->cols; ++k)
				(*this)[i][k] *= multiplier;
		};
		void join(const RTMatrix<T>& rhs) {
			if (this->rows != rhs.rows)
				throw std::runtime_error("Join failed due to rows are not equal!");
			size_t new_cols = this->cols + rhs.cols;
			T *new_data = new T[this->rows * (new_cols)];
			for (size_t i = 0; i < rows; ++i) {
				size_t k = 0;
				for (size_t j = 0; j < new_cols; ++j)
				{
					if (j < cols)
						new_data[i * new_cols + j] = (*this)[i][j];
					else {
						new_data[i * new_cols + j] = rhs[i][k];
						++k;
					}
				}
			}
			this->cols = new_cols;
			this->n = this->cols * this->rows;
			delete[] this->data;
			this->data = new_data;
		};
		int findRowWithMaxElement(int col, size_t startingRow) {
			T max = (*this)[startingRow][col];
			size_t index = startingRow;
			for (size_t i = startingRow + 1; i < this->rows; ++i)
				if (fabs((*this)[i][col]) > fabs(max))
				{
					index = i;
					max = (*this)[i][col];
				}
			return index;
		};
	};

	template <class U>
	RTMatrix<U> operator+ (const RTMatrix<U>& lhs, const RTMatrix<U>& rhs) {
		if (lhs.rows != rhs.rows || lhs.cols != rhs.cols)
			throw std::runtime_error("Matrixies are not the same size!");
		size_t rows = lhs.rows;
		size_t cols = lhs.cols;
		size_t n = lhs.n;
		U *data = new U[n];
		for (size_t i = 0; i < n; ++i)
			data[i] = lhs.data[i] + rhs.data[i];
		return RTMatrix<U>(rows, cols, n, data);
	};
	template <class U>
	RTMatrix<U> operator+ (const U& lhs, const RTMatrix<U>& rhs) {
		size_t rows = rhs.rows;
		size_t cols = rhs.cols;
		size_t n = rhs.n;
		U *data = new U[n];
		for (size_t i = 0; i < n; ++i)
			data[i] = lhs + rhs.data[i];
		return RTMatrix<U>(rows, cols, n, data);
	};
	template <class U>
	RTMatrix<U> operator+ (const RTMatrix<U>& rhs, const U& lhs) {
		size_t rows = rhs.rows;
		size_t cols = rhs.cols;
		size_t n = rhs.n;
		U *data = new U[n];
		for (size_t i = 0; i < n; ++i)
			data[i] = lhs + rhs.data[i];
		return RTMatrix<U>(rows, cols, n, data);
	};

	template <class U>
	RTMatrix<U> operator- (const RTMatrix<U>& lhs, const RTMatrix<U>& rhs) {
		if (lhs.rows != rhs.rows || lhs.cols != rhs.cols)
			throw std::runtime_error("Matrixies are not the same size!");
		size_t rows = lhs.rows;
		size_t cols = lhs.cols;
		size_t n = lhs.n;
		U *data = new U[n];
		for (size_t i = 0; i < n; ++i)
			data[i] = lhs.data[i] - rhs.data[i];
		return RTMatrix<U>(rows, cols, n, data);
	};
	template <class U>
	RTMatrix<U> operator- (const U& lhs, const RTMatrix<U>& rhs) {
		size_t rows = rhs.rows;
		size_t cols = rhs.cols;
		size_t n = rhs.n;
		U *data = new U[n];
		for (size_t i = 0; i < n; ++i)
			data[i] = lhs - rhs.data[i];
		return RTMatrix<U>(rows, cols, n, data);
	};
	template <class U>
	RTMatrix<U> operator- (const RTMatrix<U>& rhs, const U& lhs) {
		size_t rows = rhs.rows;
		size_t cols = rhs.cols;
		size_t n = rhs.n;
		U *data = new U[n];
		for (size_t i = 0; i < n; ++i)
			data[i] = lhs - rhs.data[i];
		return RTMatrix<U>(rows, cols, n, data);
	};

	template <class U>
	RTMatrix<U> operator* (const RTMatrix<U>& lhs, const RTMatrix<U>& rhs) {
		if (lhs.rows != rhs.rows || lhs.cols != rhs.cols)
			throw std::runtime_error("Matrixies are not the same size!");
		size_t rows = lhs.rows;
		size_t cols = lhs.cols;
		size_t n = lhs.n;
		U *data = new U[n];
		for (size_t i = 0; i < n; ++i)
			data[i] = lhs.data[i] * rhs.data[i];
		return RTMatrix<U>(rows, cols, n, data);
	};
	template <class U>
	RTMatrix<U> operator* (const U& lhs, const RTMatrix<U>& rhs) {
		size_t rows = rhs.rows;
		size_t cols = rhs.cols;
		size_t n = rhs.n;
		U *data = new U[n];
		for (size_t i = 0; i < n; ++i)
			data[i] = lhs * rhs.data[i];
		return RTMatrix<U>(rows, cols, n, data);
	};
	//template <class U>
	//RTMatrix<U> operator* (const RTMatrix<U>& rhs, const U& lhs) {
		//if (lhs.cols != rhs.rows)
			//throw std::runtime_error("Wrong matrices dimensions!");
		//U *result = new U[lhs.rows * rhs.cols];
		//for (size_t lhs_i = 0; lhs_i < lhs.rows; ++lhs_i) {
			//for (size_t rhs_j = 0; rhs_j < rhs.cols; ++rhs_j) {
				//U value = static_cast<U>(0.0);
				//for (size_t lhs_j = 0; lhs_j < lhs.cols; +++lhs_j) {
					//size_t lhs_linear_index = (lhs_i * lhs.cols) + lhs.cols;
					//size_t rhs_linear_index = (lhs_j * rhs.cols) + rhs_j;

					//value += (lhs.data[lhs_linear_index] * rhs.data[rhs_linear_index]);
				//}

				//int result_linear_index = (lhs_i * rhs.cols) + rhs_j;
				//result[result_linear_index] = elementResult;
			//}
		//}
		//return RTMatrix<U>(rows, cols, n, result);
	//};

	template <class U>
	rt::RTVector<U> operator* (const RTMatrix<U>& lhs, const rt::RTVector<U>& rhs) {
		if (lhs.cols != rhs.getDims())
			throw std::runtime_error("Dimensions isn't equal!");
		U *result = new U[lhs.rows];
		for (size_t i = 0; i < lhs.rows; ++i)
		{
			U sum = static_cast<U>(0.0);
			for (size_t j = 0; j < lhs.cols; ++j)
				sum += lhs[i][j] * rhs[j];
			result[i] = sum;
		}
		return rt::RTVector<U>(result, lhs.rows);
	};
}

#endif // !RT_MATRIX_HPP
