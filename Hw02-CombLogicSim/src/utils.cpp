// Utils

#include <iostream>
#include <string>
#include <sstream>
#include "utils.h"

void GateInit(Circuit &circuit) {
    int numGates = circuit.GetNumGates();
    std::string buffer;
    for (int i = 0; i < numGates; i++) {
        std::getline(std::cin, buffer);
        if (buffer.empty()) { // Ignore empty line
            i--;
            continue;
        }
        std::string type;
        int numInputs;
        std::string *pins;
        std::istringstream iss(buffer);
        iss >> type >> numInputs;
        pins = new std::string[numInputs];
        for (int j = 0; j < numInputs; j++) {
            iss >> pins[j];
        }

        int gateType;

        if (type == "AND") {
            gateType = GATE_TYPE_AND;
        } else if (type == "OR") {
            gateType = GATE_TYPE_OR;
        } else if (type == "NOT") {
            gateType = GATE_TYPE_NOT;
        } else if (type == "XOR") {
            gateType = GATE_TYPE_XOR;
        } else if (type == "NAND") {
            gateType = GATE_TYPE_NAND;
        } else if (type == "NOR") {
            gateType = GATE_TYPE_NOR;
        } else {
            throw CircuitException(CIRCUIT_EXCEPTION_GATE_TYPE_ERROR);
        }

        circuit.AddGate(i, gateType, numInputs, pins);

        delete[] pins;
    }
}

void InputInit(Circuit &circuit, signal_t *inputs, int numInputs) {
    if (numInputs != circuit.GetNumInputs()) {
        throw CircuitException(CIRCUIT_EXCEPTION_INVALID_INPUT);
    }

    circuit.Reset();
    for (int i = 0; i < numInputs; i++) {
        circuit.SetInput(i, inputs[i]);
    }
}
