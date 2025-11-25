#include <iostream>
#include <fstream>

int main() {
    std::ofstream file("hello.txt");
    if (file.is_open()) {
        file << "Hello1";
        file.close();
        std::cout << "Файл 'hello.txt' успешно создан.\n";
    } else {
        std::cerr << "Ошибка: не удалось создать файл.\n";
        return 1;
    }
    return 0;
}