#ifndef FILE_READER_H
#define FILE_READER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

struct SimulationInput {
    int width = 0;
    int height = 0;
    double g = 0.0;
    double rho = 0.0;
    std::vector<double> initialValues;
};

class FileReader {
public:
    static SimulationInput readFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        SimulationInput input;

        // ������ �������� ����
        file >> input.width >> input.height;
        if (!file || input.width <= 0 || input.height <= 0) {
            throw std::runtime_error("Invalid field dimensions: width and height must be positive.");
        }
        std::cout << "Width: " << input.width << ", Height: " << input.height << std::endl;

        // ������ ���������� ����������
        file >> input.g >> input.rho;
        if (!file) {
            throw std::runtime_error("Failed to read gravity or density parameters.");
        }
        std::cout << "Gravity: " << input.g << ", Density: " << input.rho << std::endl;

        // �������� �������� g
        if (input.g <= 0) {
            std::cerr << "Warning: Gravity (g) is non-positive. Check your input file." << std::endl;
        }

        // ������ ��������� ��������
        input.initialValues.resize(input.width * input.height);
        for (auto& val : input.initialValues) {
            file >> val;
            if (!file) {
                throw std::runtime_error("Failed to read initial value. Ensure the file format is correct.");
            }
        }

        // �������� ������������ ��������
        if (input.width * input.height != static_cast<int>(input.initialValues.size())) {
            throw std::runtime_error("Mismatch between field dimensions and initial values.");
        }

        // ������ ������ ���������� �������� ��� ��������
        std::cout << "First 10 initial values: ";
        for (int i = 0; i < std::min(10, static_cast<int>(input.initialValues.size())); ++i) {
            std::cout << input.initialValues[i] << " ";
        }
        std::cout << std::endl;

        // ����������� ��������� ������
        std::cout << "Successfully loaded input file: " << filename << "\n";

        return input;
    }
};

#endif // FILE_READER_H