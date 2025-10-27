#ifndef H_HALFTONEIMAGE
#define H_HALFTONEIMAGE
#include <iostream>
#include <cmath>
#include <random>

template <typename T>
class HalftoneImage {
private:
	size_t width;
	size_t height;
	T* data = nullptr;

public:
	inline static const double precision = 1e-8;
	HalftoneImage(size_t width, size_t height, bool autofill = false);
	~HalftoneImage();
	HalftoneImage(const HalftoneImage& src);
	HalftoneImage& operator=(const HalftoneImage& src);
	bool operator==(const HalftoneImage& src) const;
	bool operator!=(const HalftoneImage& src) const;
	T& operator()(size_t row, size_t column);
	const T& operator()(size_t row, size_t column) const;
	HalftoneImage operator*(const HalftoneImage& src) const;
	HalftoneImage operator+(const HalftoneImage& src) const;
	HalftoneImage operator*(const T& src) const;
	HalftoneImage operator+(const T& src) const;
	HalftoneImage operator!() const;
	double get_fill_factor() const;
	size_t get_width() const;
	size_t get_height() const;

};


template <typename T>
HalftoneImage<T>::HalftoneImage(size_t width, size_t height, bool autofill) : width(width), height(height) {
	if (width == 0 || height == 0) {
		throw std::invalid_argument("Dimensions must be positive.");
	}

	data = new T[width * height];

	if (autofill) {
		std::random_device rd;
		std::mt19937 gen(rd());
		if constexpr (std::is_same_v<T, bool>) {
			std::uniform_int_distribution<> random(0, 1);
			for (size_t i = 0; i < width * height; ++i) {
				data[i] = random(gen);
			}
		}
		else if constexpr (std::is_same_v<T, float>) {
			std::uniform_real_distribution<T> random(0.0, 1.0);
			for (size_t i = 0; i < width * height; ++i) {
				data[i] = random(gen);
			}
		}
		else if constexpr (std::is_integral_v<T>)
		{
			std::uniform_int_distribution<T> random(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
			for (size_t i = 0; i < width * height; ++i) {
				data[i] = random(gen);
			}
		}
	}
	else {
		for (size_t i = 0; i < width * height; ++i) {
			data[i] = static_cast<T>(0);
		}
	}
}

template <typename T>
HalftoneImage<T>::~HalftoneImage() {
	delete[] data;
}

template <typename T>
HalftoneImage<T>::HalftoneImage(const HalftoneImage& src) : width(src.width), height(src.height) {
	data = new T[width * height];
	for (size_t i = 0; i < width * height; ++i) {
		data[i] = src.data[i];
	}
}

template <typename T>
HalftoneImage<T>& HalftoneImage<T>::operator=(const HalftoneImage& src) {
	if (this == &src) {
		return *this;
	}

	delete[] data;

	width = src.width;
	height = src.height;

	data = new T[width * height];

	for (size_t i = 0; i < width * height; ++i) {
		data[i] = src.data[i];
	}

	return *this;
}

template <typename T>
bool HalftoneImage<T>::operator==(const HalftoneImage<T>& src) const {
	if (width != src.width || height != src.height) {
		return false;
	}

	for (size_t i = 0; i < width * height; ++i) {
		if constexpr (std::is_same_v<T, float>) {
			if (std::abs(data[i] - src.data[i]) > precision) {
				return false;
			}
		}
		else {
			if (data[i] != src.data[i]) {
				return false;
			}
		}
	}

	return true;
}

template <typename T>
bool HalftoneImage<T>::operator!=(const HalftoneImage& src) const {
	return !(*this == src);
}

template <typename T>
T& HalftoneImage<T>::operator()(size_t row, size_t column) {
	if (row >= height || column >= width) {
		throw std::out_of_range("Index out of range.");
	}

	return data[row * width + column];
}

template <typename T>
const T& HalftoneImage<T>::operator()(size_t row, size_t column) const {
	if (row >= height || column >= width) {
		throw std::out_of_range("Index out of range.");
	}

	return data[row * width + column];
}

template <typename T>
HalftoneImage<T> HalftoneImage<T>::operator+(const T& scalar) const {
	HalftoneImage<T> result(width, height);

	for (size_t i = 0; i < width * height; ++i) {
		if constexpr (std::is_same_v<T, bool>) {
			result.data[i] = data[i] || static_cast<bool>(scalar);
		}
		else if constexpr (std::is_same_v<T, float>) {
			result.data[i] = data[i] + scalar;
		}
		else {
			long long result_value = static_cast<long long>(data[i]) + static_cast<long long>(scalar);
			if (result_value > std::numeric_limits<T>::max()) {
				result.data[i] = std::numeric_limits<T>::max();
			}
			else if (result_value < std::numeric_limits<T>::min()) {
				result.data[i] = std::numeric_limits<T>::min();
			}
			else {
				result.data[i] = data[i] + scalar;
			}
		}
	}

	return result;
}

template <typename T>
HalftoneImage<T> HalftoneImage<T>::operator*(const T& scalar) const {
	HalftoneImage<T> result(width, height);

	for (size_t i = 0; i < width * height; ++i) {
		if constexpr (std::is_same_v<T, bool>) {
			result.data[i] = data[i] && static_cast<bool>(scalar);
		}
		else if constexpr (std::is_same_v<T, float>) {
			result.data[i] = data[i] * scalar;
		}
		else {
			long long result_value = static_cast<long long>(data[i]) * static_cast<long long>(scalar);
			if (result_value > std::numeric_limits<T>::max()) {
				result.data[i] = std::numeric_limits<T>::max();
			}
			else if (result_value < std::numeric_limits<T>::min()) {
				result.data[i] = std::numeric_limits<T>::min();
			}
			else {
				result.data[i] = data[i] * scalar;
			}
		}
	}

	return result;
}

template <typename T>
HalftoneImage<T> HalftoneImage<T>::operator+(const HalftoneImage<T>& src) const {
	size_t result_width = std::max(width, src.width);
	size_t result_height = std::max(height, src.height);

	HalftoneImage<T> result(result_width, result_height);

	for (size_t row = 0; row < result_height; ++row) {
		for (size_t column = 0; column < result_width; ++column) {
			T value1 = static_cast<T>(0);
			T value2 = static_cast<T>(0);

			if (row < height && column < width) {
				value1 = (*this)(row, column);
			}

			if (row < src.height && column < src.width) {
				value2 = src(row, column);
			}

			if constexpr (std::is_same_v<T, bool>) {
				result(row, column) = value1 || value2;
			}
			else if constexpr(std::is_same_v<T, float>) {
				result(row, column) = value1 + value2;
			}
			else {
				long long result_value = static_cast<long long>(value1) + static_cast<long long>(value2);
				if (result_value > std::numeric_limits<T>::max()) {
					result(row, column) = std::numeric_limits<T>::max();
				}
				else {
					if (result_value < std::numeric_limits<T>::min())
					{
						result(row, column) = std::numeric_limits<T>::min();
					}
					else {
						result(row, column) = value1 + value2;
					}
				}
			}
		}
	}

	return result;
}

template <typename T>
HalftoneImage<T> HalftoneImage<T>::operator*(const HalftoneImage<T>& src) const {
	if (width != src.width || height != src.height) {
		throw std::invalid_argument("Images must have the same dimensions.");
	}

	HalftoneImage<T> result(width, height);

	for (size_t i = 0; i < width * height; ++i) {
		if constexpr (std::is_same_v<T, bool>) {
			result.data[i] = data[i] && src.data[i];
		}
		else if (std::is_same_v<T, float>) {
			result.data[i] = data[i] * src.data[i];
		}
		else {
			long long result_value = static_cast<long long>(data[i]) * static_cast<long long>(src.data[i]);

			if (result_value > std::numeric_limits<T>::max()) {
				result.data[i] = std::numeric_limits<T>::max();
			}
			else if (result_value < std::numeric_limits<T>::min()) {
				result.data[i] = std::numeric_limits<T>::min();
			}
			else
			{
				result.data[i] = data[i] * src.data[i];
			}
		}
	}

	return result;
}

template <typename T>
HalftoneImage<T> HalftoneImage<T>::operator!() const {
	HalftoneImage<T> result(width, height);

	for (size_t i = 0; i < width * height; ++i) {
		if constexpr (std::is_same_v<T, bool>) {
			result.data[i] = !data[i];
		}
		else {
			if (data[i] == std::numeric_limits<T>::min())
			{
				result.data[i] = -(data[i] + 1);
			}
			else
			{
				result.data[i] = -data[i];
			}
		}
	}

	return result;
}

template <typename T>
double HalftoneImage<T>::get_fill_factor() const {
	long double sum = 0;
	for (size_t i = 0; i < width * height; ++i) {
		sum += static_cast<long double>(data[i]);
	}

	long double max_sum = 0;
	size_t value = width * height;

	if (value == 0) {
		return 0.0;
	}

	/*if constexpr (std::is_same_v<T, bool> || std::is_same_v<T, float>) {
		max_sum = static_cast<long double>(value);
	}
	else {
		max_sum = static_cast<long double>(value) * std::numeric_limits<T>::max();
	}*/
	max_sum = static_cast<long double>(value) * std::numeric_limits<T>::max();

	if (max_sum == 0) {
		return 0.0;
	}

	return static_cast<double>(sum / max_sum);
}


template <typename T>
size_t HalftoneImage<T>::get_width() const {
	return width;
}

template <typename T>
size_t HalftoneImage<T>::get_height() const {
	return height;
}

template <typename T>
std::ostream& operator<<(std::ostream& ostream, const HalftoneImage<T>& src) {
	for (size_t i = 0; i < src.get_height(); ++i) {
		for (size_t j = 0; j < src.get_width(); ++j) {
			ostream << src(i, j) << "\t";
			ostream << " ";
		}
		ostream << "\n";
	}
	return ostream;
}

template <typename T>
void flip_image_180(HalftoneImage<T>& image) {
	HalftoneImage<T> rotated_image(image.get_width(), image.get_height());
	for (size_t i = 0; i < image.get_height(); ++i) {
		for (size_t j = 0; j < image.get_width(); ++j) {
			rotated_image(i, j) = image(image.get_width() - i - 1, image.get_height() - j - 1);
		}
	}
	image = rotated_image;
}
#endif
