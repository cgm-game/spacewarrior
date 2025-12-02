//
// Created by gyan on 5/18/24.
//

#include <screen.hpp>

void displayScreen(std::function<void()> drawFunction) {
    currentScreenDrawFunction = drawFunction;
}