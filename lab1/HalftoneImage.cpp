#include "HalftoneImage.h"

template <typename T>
HalftoneImage<T>::HalftoneImage(size_t width, size_t height, bool autofill) : width(width), height(height) {
	if (width == 0 || height == 0) {
		throw std::invalid_argument("Dimesions can not be zero.");
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
		throw std::out_of_range("One of indexes or both are out of range.");
	}

	return data[row * width + column];
}

template <typename T>
const T& HalftoneImage<T>::operator()(size_t row, size_t column) const {
	if (row >= height || column >= width) {
		throw std::out_of_range("One of indexes or both are out of range.");
	}

	return data[row * width + column];
}