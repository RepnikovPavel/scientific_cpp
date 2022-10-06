#pragma once
#include <array>
#include <iostream>
#include <format>
#include <string>
#include <chrono>


class timer
{
	using clock_t = std::chrono::high_resolution_clock;
	using microseconds = std::chrono::microseconds;
public:
	timer();
	double stop();
	~timer();
private:
	const clock_t::time_point start_;
};

struct InitStruct
{
	double init_value;
};

template<size_t M, size_t N>
	requires (M > 1 && N == 1) || (M == 1 && N > 1) || (M > 1 && N > 1)
class matrix
{
public:
	matrix() = delete;

	matrix(const matrix<M, N>& other) :_container(other._container){}
	matrix(matrix<M, N>&& other)
	{
		_container = std::move(other._container);
	}
	matrix(InitStruct init_struct)
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				_container[i][j] = init_struct.init_value;
			}
		}
	}
	auto& operator[](size_t i)&
	{
		return _container[i];
	}
	auto& operator=(matrix<M, N>&& other)&
	{
		_container = std::move(other._container);
		return *this;
	}
	auto T()
	{
		matrix<N, M> new_matrix({ .init_value = 0.0 });
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				new_matrix._container[j][i] = _container[i][j];
			}
		}
		return new_matrix;
	}

	auto&& operator+(matrix<M, N>&& other) &&
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				_container[i][j]+= other._container[i][j];
			}
		}
		return std::move(*this);
	}
	auto&& operator+(matrix<M, N>&& other) &
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				other._container[i][j] += _container[i][j];
			}
		}
		return std::move(other);
	}
	auto&& operator+(matrix<M, N>& other)&&
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				_container[i][j] += other._container[i][j];
			}
		}
		return std::move(*this);
	}
	auto operator+(matrix<M, N>& other)&
	{
		matrix<M, N> new_matrix({ .init_value = 0.0 });
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				new_matrix._container[i][j] = _container[i][j] + other._container[i][j];
			}
		}
		return new_matrix;
	}

	auto&& operator-(matrix<M, N>&& other)&&
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				_container[i][j] -= other._container[i][j];
			}
		}
		return std::move(*this);
	}
	auto&& operator-(matrix<M, N>&& other)&
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				other._container[i][j] = _container[i][j] - other._container[i][j];
			}
		}
		return std::move(other);
	}
	auto&& operator-(matrix<M, N>& other)&&
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				_container[i][j] -= other._container[i][j];
			}
		}
		return std::move(*this);
	}
	auto operator-(matrix<M, N>& other)&
	{
		matrix<M, N> new_matrix({ .init_value = 0.0 });
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				new_matrix._container[i][j] = _container[i][j] - other._container[i][j];
			}
		}
		return new_matrix;
	}

	void operator+=(matrix<M, N>&& other)&
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				_container[i][j] += other._container[i][j];
			}
		}
	}
	void operator+=(matrix<M, N>& other)&
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				_container[i][j] += other._container[i][j];
			}
		}
	}
	void operator*=(double alpha)
	{
		for (size_t i = 0; i < M; i++)
		{
			for (size_t j = 0; j < N; j++)
			{
				_container[i][j] *= alpha;
			}
		}
	}

	void PrintSelf() const 
	{
		std::cout << "\t|";
		for (size_t j = 0; j < N; j++)
		{
			if (j == N - 1)
			{
				std::cout << std::format("{:.5f}", _container[0][j]);
			}
			else {
				std::cout << std::format("{:.5f} ", _container[0][j]);
			}
		}
		std::cout << std::format("|{}x{}",M,N) << std::endl;

		for (size_t i = 1; i < M; i++)
		{
			std::cout << "\t|";
			for (size_t j = 0; j < N; j++)
			{
				if (j==N-1)
				{
					std::cout << std::format("{:.5f}", _container[i][j]);
				}
				else {
					std::cout << std::format("{:.5f} ", _container[i][j]);
				}

			}
			if (i==(N-1))
			{
				std::cout << "|";
			}
			else {
				std::cout << "|" << std::endl;
			}
		}
	}

	std::array<std::array<double,N>,M> _container;
};

template<size_t M, size_t N1, size_t N2>
auto operator*(matrix<M, N1>&& A1, matrix<N1, N2>&& A2)
{
	matrix<M, N2> new_matrix({ .init_value = 0.0 });
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < N2; j++)
		{
			double sum = 0.0;
			for (size_t k = 0; k < N1; k++)
			{
				sum += A1._container[i][k] * A2._container[k][j];
			}
			new_matrix._container[i][j] = sum;
		}
	}
	return new_matrix;
}
template<size_t N>
double operator*(matrix<1, N>&& A1, matrix<N,1>&& A2)
{
	double sum = 0.0;
	for (size_t i = 0; i < N; i++)
	{
		sum += A1._container[0][i] * A2._container[i][0];
	}
	return sum;
}
template<size_t M>
auto&& operator*(matrix<M, M>&& A1, matrix<M, M>&& A2)
{
	for (size_t i = 0; i < M; i++)
	{	
		std::array<M>&& buffer;
		for (size_t j = 0; j < M; j++)
		{
			double sum = 0.0;
			for (size_t k = 0; k < M; k++)
			{
				sum += A1._container[i][k] * A2._container[k][j];
			}
			buffer[j] = sum;
		}
		A1._container[i] = std::move(buffer);
	}
	return std::move(A1);
}

template<size_t M, size_t N1, size_t N2>
auto operator*(matrix<M, N1>& A1, matrix<N1, N2>& A2)
{
	matrix<M, N2> new_matrix({ .init_value = 0.0 });
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < N2; j++)
		{
			double sum = 0.0;
			for (size_t k = 0; k < N1; k++)
			{
				sum += A1._container[i][k] * A2._container[k][j];
			}
			new_matrix._container[i][j] = sum;
		}
	}
	return new_matrix;
}
template<size_t N>
double operator*(matrix<1, N>& A1, matrix<N, 1>& A2)
{
	double sum = 0.0;
	for (size_t i = 0; i < N; i++)
	{
		sum += A1._container[0][i] * A2._container[i][0];
	}
	return sum;
}
template<size_t M>
auto operator*(matrix<M, M>& A1, matrix<M, M>& A2)
{
	matrix<M, M> new_matrix({ .init_value = 0.0 });
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < M; j++)
		{
			double sum = 0.0;
			for (size_t k = 0; k < M; k++)
			{
				sum += A1._container[i][k] * A2._container[k][j];
			}
			new_matrix._container[i][j] = sum;
		}
	}
	return new_matrix;
}

template<size_t M, size_t N1, size_t N2>
auto operator*(matrix<M, N1>& A1, matrix<N1, N2>&& A2)
{
	matrix<M, N2> new_matrix({ .init_value = 0.0 });
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < N2; j++)
		{
			double sum = 0.0;
			for (size_t k = 0; k < N1; k++)
			{
				sum += A1._container[i][k] * A2._container[k][j];
			}
			new_matrix._container[i][j] = sum;
		}
	}
	return new_matrix;
}
template<size_t N>
double operator*(matrix<1, N>& A1, matrix<N, 1>&& A2)
{
	double sum = 0.0;
	for (size_t i = 0; i < N; i++)
	{
		sum += A1._container[0][i] * A2._container[i][0];
	}
	return sum;
}
template<size_t M>
auto operator*(matrix<M, M>& A1, matrix<M, M>&& A2)
{
	matrix<M, M> new_matrix({ .init_value = 0.0 });
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < M; j++)
		{
			double sum = 0.0;
			for (size_t k = 0; k < M; k++)
			{
				sum += A1._container[i][k] * A2._container[k][j];
			}
			new_matrix._container[i][j] = sum;
		}
	}
	return new_matrix;
}

template<size_t M, size_t N1, size_t N2>
auto operator*(matrix<M, N1>&& A1, matrix<N1, N2>& A2)
{
	matrix<M, N2> new_matrix({ .init_value = 0.0 });
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < N2; j++)
		{
			double sum = 0.0;
			for (size_t k = 0; k < N1; k++)
			{
				sum += A1._container[i][k] * A2._container[k][j];
			}
			new_matrix._container[i][j] = sum;
		}
	}
	return new_matrix;
}
template<size_t N>
double operator*(matrix<1, N>&& A1, matrix<N, 1>& A2)
{
	double sum = 0.0;
	for (size_t i = 0; i < N; i++)
	{
		sum += A1._container[0][i] * A2._container[i][0];
	}
	return sum;
}
template<size_t M>
auto&& operator*(matrix<M, M>&& A1, matrix<M, M>& A2)
{
	for (size_t i = 0; i < M; i++)
	{
		std::array<M>&& buffer;
		for (size_t j = 0; j < M; j++)
		{
			double sum = 0.0;
			for (size_t k = 0; k < M; k++)
			{
				sum += A1._container[i][k] * A2._container[k][j];
			}
			buffer[j] = sum;
		}
		A1._container[i] = std::move(buffer);
	}
	return std::move(A1);
}

template<size_t M,size_t N>
auto&& operator*(double alpha, matrix<M, N>&& A)
{
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			A._container[i][j] *= alpha;
		}
	}
	return std::move(A);
}
template< size_t M, size_t N>
auto operator*(double alpha, matrix<M, N>& A)
{
	matrix<M, N> new_matrix;
	for (size_t i = 0; i < M; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			new_matrix._container[i][j] = alpha*A._container[i][j];
		}
	}
	return new_matrix;
}


template<size_t M,size_t N>
void Print(const matrix<M,N>& matrix)
{	
	matrix.PrintSelf();
	std::cout << std::endl;
}

template<typename Last>
void print_wstrings(Last last)
{
	std::wcout << last;
}
template<typename First, typename ...Rest>
void print_wstrings(First first, Rest... rest)
{
	std::wcout << first << " ";
	print_wstrings(rest ...);
}
template<typename ...PrintableData>
void Print(PrintableData... data)
{
	std::wcout <<"\t";
	print_wstrings(data ...);
	std::wcout << std::endl;
}
