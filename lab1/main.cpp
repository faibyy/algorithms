#include "HalftoneImage.h"
#include <iostream>

int main() {
    try {
        std::cout << "--- demonstration for <short> type ---" << std::endl;

        HalftoneImage<short> img1(2, 4, true);
        std::cout << "Randomly filled img1(2, 4):\n" << img1 << std::endl;

        HalftoneImage<short> img2 = img1;
        if (img1 == img2) {
            std::cout << "img1 == img2" << std::endl;
        }

        img2(0, 1) = 10;
        if (img1 != img2) {
            std::cout << "img2(1,2)=10; img2 != img1" << std::endl;
            std::cout << "New img2:\n" << img2 << std::endl;
        }

        HalftoneImage<short> img3(2, 4);
        img3 = img2;
        std::cout << "img3 = img2\n" << img3 << std::endl;

        long scalar = 10;
        HalftoneImage<short> img4 = (img1 + scalar) * 10;
        std::cout << "img4 = (img1 + scalar) * 10:\n" << img4 << std::endl;
        img4 = !img4;
        std::cout << "img4 with inverted values:\n" << !img4 << std::endl;

        HalftoneImage<short> img5(4, 2, true);
        std::cout << "img5(4,2):\n" << img5 << std::endl;
        HalftoneImage<short> sum_result = img4 + img5;
        std::cout << "img4(2,4)+img5(4,2)=sum_result(4,4):\n" << sum_result << std::endl;
        std::cout << "Fill factor of img5: " << img1.get_fill_factor() << std::endl;

        HalftoneImage<short> task_img(5, 5, true);
        std::cout << "\n--- task solution ---" << std::endl;
        std::cout << "Original image:\n" << task_img << std::endl;
        flip_image_180(task_img);
        std::cout << "Flipped image:\n" << task_img << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }

    return 0;
}