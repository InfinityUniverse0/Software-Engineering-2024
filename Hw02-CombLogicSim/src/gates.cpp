// Gates

#include "gates.h"

Gate::Gate(int numInputs) : n(numInputs) {
    inputs = new signal_t[n];
    output = 0; // Default output (NOT valid)
    flags = new bool[n];
    count = 0;
    valid = false;
}

Gate::~Gate() {
    delete[] inputs;
    delete[] flags;
}

void Gate::Reset() {
    for (int i = 0; i < n; i++) { // Reset flags
        flags[i] = false;
    }
    count = 0; // Reset count
    valid = false; // Reset valid flag
}

int Gate::SetInput(int index, signal_t value) {
    /*
     * Set the `index`-th input signal to `value`
     *   Parameters:
     *     index: which input to set. (0 <= index < n)
     *     value: the value to set. (0 or 1)
     *   Return:
     *     GATE_STATUS_OK: if the input is set successfully
     *     GATE_STATUS_RANGE_ERROR: if the input index is out of range
     *     GATE_STATUS_INVALID_INPUT: if the input value is invalid
     *   NOTE:
     *     Bounding Check & Validity Check are executed before setting the value.
     *     If all inputs are set, the Calculate() function will be called.
     */
    if (index >= 0 && index < n) {
        if (value == 0 || value == 1) {
            if (!flags[index]) {
                flags[index] = true;
                count++;
            }
            inputs[index] = value;
            valid = false; // Reset valid flag

            if (count == n) {
                Calculate();
            }

            return GATE_STATUS_OK; // OK
        }
        return GATE_STATUS_INVALID_INPUT; // INVALID INPUT
    }
    return GATE_STATUS_RANGE_ERROR; // RANGE ERROR
}

signal_t Gate::GetOutput() const {
    if (valid) {
        return output;
    }
    return GATE_STATUS_INVALID_OUTPUT;
}

bool Gate::CheckInputNum(int numInputs) {
    if (numInputs >= 0 && numInputs <= MAX_INPUTS_NUM) {
        return true;
    }
    return false;
}

ANDGate::ANDGate(int numInputs) : Gate(numInputs) {}

void ANDGate::Calculate() {
    if (count == n) {
        signal_t result = 1;
        for (int i = 0; i < n; i++) {
            result &= inputs[i];
        }
        output = result;
        valid = true;
    }
}

bool ANDGate::CheckInputNum(int numInputs) {
    if (numInputs >= 2 && numInputs <= MAX_INPUTS_NUM) {
        return true;
    }
    return false;
}

ORGate::ORGate(int numInputs) : Gate(numInputs) {}

void ORGate::Calculate() {
    if (count == n) {
        signal_t result = 0;
        for (int i = 0; i < n; i++) {
            result |= inputs[i];
        }
        output = result;
        valid = true;
    }
}

bool ORGate::CheckInputNum(int numInputs) {
    if (numInputs >= 2 && numInputs <= MAX_INPUTS_NUM) {
        return true;
    }
    return false;
}

NOTGate::NOTGate() : Gate(1) {}

void NOTGate::Calculate() {
    if (count == 1) {
        output = !inputs[0];
        valid = true;
    }
}

bool NOTGate::CheckInputNum(int numInputs) {
    if (numInputs == 1) {
        return true;
    }
    return false;
}

XORGate::XORGate(int numInputs) : Gate(numInputs) {}

void XORGate::Calculate() {
    if (count == n) {
        signal_t result = 0;
        for (int i = 0; i < n; i++) {
            result ^= inputs[i];
        }
        output = result;
        valid = true;
    }
}

bool XORGate::CheckInputNum(int numInputs) {
    if (numInputs >= 2 && numInputs <= MAX_INPUTS_NUM) {
        return true;
    }
    return false;
}

NANDGate::NANDGate(int numInputs) : Gate(numInputs) {}

void NANDGate::Calculate() {
    if (count == n) {
        signal_t result = 1;
        for (int i = 0; i < n; i++) {
            result &= inputs[i];
        }
        output = !result;
        valid = true;
    }
}

bool NANDGate::CheckInputNum(int numInputs) {
    if (numInputs >= 2 && numInputs <= MAX_INPUTS_NUM) {
        return true;
    }
    return false;
}

NORGate::NORGate(int numInputs) : Gate(numInputs) {}

void NORGate::Calculate() {
    if (count == n) {
        signal_t result = 0;
        for (int i = 0; i < n; i++) {
            result |= inputs[i];
        }
        output = !result;
        valid = true;
    }
}

bool NORGate::CheckInputNum(int numInputs) {
    if (numInputs >= 2 && numInputs <= MAX_INPUTS_NUM) {
        return true;
    }
    return false;
}
