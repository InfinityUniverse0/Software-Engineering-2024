// main

#include <iostream>
#include "config.h"
#include "exception.h"
#include "circuit.h"
#include "utils.h"

int main() {
    int query; // Number of queries
    std::cin >> query;
    if (query < 1) {
        std::cerr << "Invalid query number" << std::endl;
        return 1;
    }

    int numInputs, numGates;
    std::cin >> numInputs >> numGates;
    if (numGates < 1 || numGates > MAX_GATES_NUM || numInputs < 0 || (numInputs > MAX_INPUTS_NUM * numGates)) {
        std::cerr << "Invalid input number or gate number" << std::endl;
        std::cerr << "0 <= numInputs <= " << MAX_INPUTS_NUM * numGates << std::endl;
        std::cerr << "1 <= numGates <= " << MAX_GATES_NUM << std::endl;
        std::cerr << "Got Input number: " << numInputs << ", Got Gate number: " << numGates << std::endl;
        return 1;
    }

    for (int i = 0; i < query; i++) { // For each query
        Circuit circuit(numGates, numInputs);
        int runTimes; // Number of runs
        try {
            GateInit(circuit);

            std::cin >> runTimes;
            if (runTimes < 1 || runTimes > MAX_RUN_CYCLES) {
                std::cerr << "Invalid run number" << std::endl;
                std::cerr << "1 <= runTimes <= " << MAX_RUN_CYCLES << std::endl;
                std::cerr << "Got runTimes: " << runTimes << std::endl;
            }

            auto **inputs = new signal_t *[runTimes]; // Input value array
            auto **outputIndex = new int *[runTimes]; // Output index array
            auto *outputNum = new int[runTimes]; // Output number array
            for (int j = 0; j < runTimes; j++) { // Read inputs
                inputs[j] = new int[numInputs];
                for (int k = 0; k < numInputs; k++) {
                    std::cin >> inputs[j][k];
                }
            }
            for (int j = 0; j < runTimes; j++) { // Read output index
                std::cin >> outputNum[j];
                outputIndex[j] = new int[outputNum[j]];
                for (int k = 0; k < outputNum[j]; k++) {
                    std::cin >> outputIndex[j][k];
                    outputIndex[j][k]--; // 1-based to 0-based
                }
            }

            auto **outputs = new int *[runTimes]; // Output value array
            auto *flags = new bool[runTimes]; // Output flag array

            for (int j = 0; j < runTimes; j++) { // For each run
                InputInit(circuit, inputs[j], numInputs);
                outputs[j] = new int[outputNum[j]];
                circuit.Run();
                if (circuit.GetState() == CIRCUIT_STATE_OK) {
                    flags[j] = true;
                    for (int k = 0; k < outputNum[j]; k++) {
                        outputs[j][k] = circuit.GetOutput(outputIndex[j][k]);
                    }
                } else {
                    flags[j] = false;
                }
            }

            // Print
            for (int j = 0; j < runTimes; j++) {
                if (flags[j]) {
                    for (int k = 0; k < outputNum[j]; k++) {
                        if (k > 0) {
                            std::cout << " ";
                        }
                        std::cout << outputs[j][k];
                    }
                } else {
                    std::cout << LOOP_ERROR_MSG;
                }
                std::cout << std::endl;

                delete[] inputs[j];
                delete[] outputIndex[j];
                delete[] outputs[j];
            }

            delete[] inputs;
            delete[] outputIndex;
            delete[] outputNum;
            delete[] outputs;
            delete[] flags;
        } catch (CircuitException &e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }
    return 0;
}
