// Circuit

#include "circuit.h"

Circuit::Circuit(int _numGates, int _numInputs) : numGates(_numGates), numInputs(_numInputs) {
    gates = new Gate *[numGates];
    gateInfos = new GateInfo[numGates];
    inputs = new signal_t[numInputs];
    flags = new bool[numInputs];
    state = CIRCUIT_STATE_WAIT;

    for (int i = 0; i < numGates; i++) {
        gates[i] = nullptr;
    }
    for (int i = 0; i < numInputs; i++) {
        flags[i] = false;
    }
}

Circuit::~Circuit() {
    for (int i = 0; i < numGates; i++) {
        delete gates[i];
    }
    delete[] gates;
    delete[] gateInfos;
    delete[] inputs;
    delete[] flags;
}

void Circuit::AddGate(int index, int type, int gateNumInputs, std::string *pins) {
    if (index >= 0 && index < numGates) {
        if (gates[index] == nullptr) {
            switch (type) {
                case GATE_TYPE_AND:
                    if (!ANDGate::CheckInputNum(gateNumInputs)) {
                        state = CIRCUIT_STATE_ERROR;
                        throw CircuitException(CIRCUIT_EXCEPTION_INVALID_INPUT);
                    }
                    gates[index] = new ANDGate(gateNumInputs);
                    break;
                case GATE_TYPE_OR:
                    if (!ORGate::CheckInputNum(gateNumInputs)) {
                        state = CIRCUIT_STATE_ERROR;
                        throw CircuitException(CIRCUIT_EXCEPTION_INVALID_INPUT);
                    }
                    gates[index] = new ORGate(gateNumInputs);
                    break;
                case GATE_TYPE_NOT:
                    if (!NOTGate::CheckInputNum(gateNumInputs)) {
                        state = CIRCUIT_STATE_ERROR;
                        throw CircuitException(CIRCUIT_EXCEPTION_INVALID_INPUT);
                    }
                    gates[index] = new NOTGate();
                    break;
                case GATE_TYPE_XOR:
                    if (!XORGate::CheckInputNum(gateNumInputs)) {
                        state = CIRCUIT_STATE_ERROR;
                        throw CircuitException(CIRCUIT_EXCEPTION_INVALID_INPUT);
                    }
                    gates[index] = new XORGate(gateNumInputs);
                    break;
                case GATE_TYPE_NAND:
                    if (!NANDGate::CheckInputNum(gateNumInputs)) {
                        state = CIRCUIT_STATE_ERROR;
                        throw CircuitException(CIRCUIT_EXCEPTION_INVALID_INPUT);
                    }
                    gates[index] = new NANDGate(gateNumInputs);
                    break;
                case GATE_TYPE_NOR:
                    if (!NORGate::CheckInputNum(gateNumInputs)) {
                        state = CIRCUIT_STATE_ERROR;
                        throw CircuitException(CIRCUIT_EXCEPTION_INVALID_INPUT);
                    }
                    gates[index] = new NORGate(gateNumInputs);
                    break;
                default:
                    state = CIRCUIT_STATE_ERROR;
                    throw CircuitException(CIRCUIT_EXCEPTION_GATE_TYPE_ERROR);
            }

            gateInfos[index].type = type;
            gateInfos[index].numInputs = gateNumInputs;
            gateInfos[index].pins = new std::string[gateNumInputs];
            for (int i = 0; i < gateNumInputs; i++) {
                if (pins[i].length() >= 2 && ((pins[i][0] == 'I') || (pins[i][0] == 'O'))) {
                    gateInfos[index].pins[i] = pins[i];
                } else {
                    state = CIRCUIT_STATE_ERROR;
                    throw CircuitException(CIRCUIT_EXCEPTION_INVALID_INPUT);
                }
            }

            state = CIRCUIT_STATE_WAIT;
        } else {
            throw CircuitException(CIRCUIT_EXCEPTION_VALUE_RESET_WARNING);
        }
    } else {
        throw CircuitException(CIRCUIT_EXCEPTION_INDEX_OUT_OF_RANGE);
    }
}

void Circuit::SetInput(int index, signal_t value) {
    if (index >= 0 && index < numInputs) {
        if (value == 0 || value == 1) {
            inputs[index] = value;
            flags[index] = true;
            state = CIRCUIT_STATE_WAIT;
        } else {
            throw CircuitException(CIRCUIT_EXCEPTION_INVALID_INPUT);
        }
    } else {
        throw CircuitException(CIRCUIT_EXCEPTION_INDEX_OUT_OF_RANGE);
    }
}

void Circuit::Reset() {
    for (int i = 0; i < numInputs; i++) {
        flags[i] = false;
    }
    state = CIRCUIT_STATE_WAIT;
}

void Circuit::Run() {
    if (state == CIRCUIT_STATE_OK) {
        return; // No need to re-run
    }
    if (state != CIRCUIT_STATE_WAIT) {
        throw CircuitException(CIRCUIT_EXCEPTION_RUNTIME_ERROR);
    }

    // Validity check
    for (int i = 0; i < numGates; i++) {
        if (gates[i] == nullptr) {
            state = CIRCUIT_STATE_ERROR;
            throw CircuitException(CIRCUIT_EXCEPTION_RUNTIME_ERROR);
        }
        gates[i]->Reset(); // Initialization
    }
    for (int i = 0; i < numInputs; i++) {
        if (!flags[i]) {
            state = CIRCUIT_STATE_ERROR;
            throw CircuitException(CIRCUIT_EXCEPTION_RUNTIME_ERROR);
        }
    }

    // Run the circuit and check loop (using Topological Sort)
    bool changed = true;
    bool *outputFlags = new bool[numGates] {false}; // All elements are false

    while (changed) {
        changed = false;

        for (int i = 0; i < numGates; i++) {
            if (gates[i]->GetOutput() != GATE_STATUS_INVALID_OUTPUT) {
                outputFlags[i] = true;
                continue;
            }

            for (int j = 0; j < gateInfos[i].numInputs; j++) {
                // Check input flag
                if (gates[i]->CheckInputFlag(j)) {
                    continue;
                }

                if (gateInfos[i].pins[j][0] == 'I') {
                    int inputIndex = std::stoi(gateInfos[i].pins[j].substr(1));
                    inputIndex--; // 1-based to 0-based

                    if (inputIndex < 0 || inputIndex >= numInputs) {
                        state = CIRCUIT_STATE_ERROR;
                        throw CircuitException(CIRCUIT_EXCEPTION_INDEX_OUT_OF_RANGE);
                    }

                    int statusCode = gates[i]->SetInput(j, inputs[inputIndex]);
                    if (statusCode != GATE_STATUS_OK) {
                        state = CIRCUIT_STATE_ERROR;
                        if (statusCode == GATE_STATUS_RANGE_ERROR) {
                            throw CircuitException(CIRCUIT_EXCEPTION_INDEX_OUT_OF_RANGE);
                        } else if (statusCode == GATE_STATUS_INVALID_INPUT) {
                            throw CircuitException(CIRCUIT_EXCEPTION_INVALID_INPUT);
                        } else {
                            throw CircuitException(CIRCUIT_EXCEPTION_RUNTIME_ERROR);
                        }
                    }

                    changed = true;
                } else { // gateInfos[i].pins[j][0] == 'O'
                    int outputIndex = std::stoi(gateInfos[i].pins[j].substr(1));
                    outputIndex--; // 1-based to 0-based

                    if (outputIndex < 0 || outputIndex >= numGates) {
                        state = CIRCUIT_STATE_ERROR;
                        throw CircuitException(CIRCUIT_EXCEPTION_INDEX_OUT_OF_RANGE);
                    }

                    signal_t gateOutput = gates[outputIndex]->GetOutput();
                    if (gateOutput != GATE_STATUS_INVALID_OUTPUT) {
                        int statusCode = gates[i]->SetInput(j, gateOutput);
                        if (statusCode != GATE_STATUS_OK) {
                            state = CIRCUIT_STATE_ERROR;
                            if (statusCode == GATE_STATUS_RANGE_ERROR) {
                                throw CircuitException(CIRCUIT_EXCEPTION_INDEX_OUT_OF_RANGE);
                            } else if (statusCode == GATE_STATUS_INVALID_INPUT) {
                                throw CircuitException(CIRCUIT_EXCEPTION_INVALID_INPUT);
                            } else {
                                throw CircuitException(CIRCUIT_EXCEPTION_RUNTIME_ERROR);
                            }
                        }

                        changed = true;
                    }
                }
            }
        }
    }

    for (int i = 0; i < numGates; i++) {
        if (!outputFlags[i]) { // Has loop(s)
            state = CIRCUIT_STATE_LOOP;
            break;
        }
    }

    if (state != CIRCUIT_STATE_LOOP) {
        state = CIRCUIT_STATE_OK; // NO loop
    }

    delete[] outputFlags;
}

signal_t Circuit::GetOutput(int index) const {
    if (state != CIRCUIT_STATE_OK) {
        throw CircuitException(CIRCUIT_EXCEPTION_RUNTIME_ERROR);
    }

    if (index >= 0 && index < numGates) {
        signal_t result = gates[index]->GetOutput();
        if (result == GATE_STATUS_INVALID_OUTPUT) {
            throw CircuitException(CIRCUIT_EXCEPTION_RUNTIME_ERROR);
        }
        return result;
    } else {
        throw CircuitException(CIRCUIT_EXCEPTION_INDEX_OUT_OF_RANGE);
    }
}
