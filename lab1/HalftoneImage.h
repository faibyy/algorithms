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
	inline static const double precision = 1e-6;
	HalftoneImage(size_t width, size_t height, bool autofill = false);
	~HalftoneImage();
	HalftoneImage(const HalftoneImage& src);
	HalftoneImage& operator=(const HalftoneImage& src);
	bool operator==(const HalftoneImage& src) const;
	bool operator!=(const HalftoneImage& src) const;
	T& operator()(size_t row, size_t column);
	const T& operator()(size_t row, size_t column) const;