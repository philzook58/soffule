#include <cstdint>
#include <iostream>
extern "C" {

int32_t f(int32_t x) {
    return x + 1;
}

const char *g() {
    std::cout << "g called" << std::endl;
    return "Hello world";
}

}
