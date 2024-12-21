#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include "Fixed.h"
#include "Simulator.h"
#include "FileReader.h"

int main() {
    try {
        // Фиксированный путь к входному файлу
        const std::string inputFilePath = "input_large.txt";

        // Чтение данных из файла
        SimulationInput input = FileReader::readFromFile(inputFilePath);

        // Преобразование начальных значений в Fixed<32, 16>
        std::vector<Fixed<32, 16>> fixedInitialValues;
        for (const auto& val : input.initialValues) {
            fixedInitialValues.emplace_back(val);
        }

        // Создание симулятора
        FluidSimulator<Fixed<32, 16>> simulator(input.width, input.height);

        // Инициализация поля
        simulator.initialize(fixedInitialValues);

        // Определение количества потоков
        int numThreads = std::thread::hardware_concurrency(); // Автоматический выбор
        if (numThreads == 0) numThreads = 4; // Если невозможно определить, используем 4 потока
        std::cout << "Using " << numThreads << " threads for simulation.\n";

        // Замер общего времени выполнения
        auto totalStart = std::chrono::high_resolution_clock::now();

        // Запуск симуляции
        for (int step = 0; step < 10; ++step) {
            std::cout << "Simulating step " << step + 1 << "...\n";

            // Замер времени для одного шага
            auto stepStart = std::chrono::high_resolution_clock::now();

            simulator.simulateStep(input.g, input.rho, numThreads);

            auto stepEnd = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> stepElapsed = stepEnd - stepStart;

            // Сохранение результата
            simulator.saveToFile("output_step_" + std::to_string(step + 1) + ".txt");

            std::cout << "Step " << step + 1 << " completed in " << stepElapsed.count() << " seconds.\n";
        }

        // Конец общего времени
        auto totalEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> totalElapsed = totalEnd - totalStart;

        std::cout << "Simulation completed successfully.\n";
        std::cout << "Total simulation time: " << totalElapsed.count() << " seconds.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
