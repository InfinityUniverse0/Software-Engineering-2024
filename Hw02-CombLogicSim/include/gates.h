// Gates

#ifndef COMBLOGICSIM_GATES_H
#define COMBLOGICSIM_GATES_H

#include "config.h"

// Gate Types
#define GATE_TYPE_AND 0
#define GATE_TYPE_OR 1
#define GATE_TYPE_NOT 2
#define GATE_TYPE_XOR 3
#define GATE_TYPE_NAND 4
#define GATE_TYPE_NOR 5

// Gate Status Code
#define GATE_STATUS_OK 0
#define GATE_STATUS_RANGE_ERROR 1
#define GATE_STATUS_INVALID_INPUT 2
#define GATE_STATUS_INVALID_OUTPUT (-1)

class Gate {
    /*
     * Gate Base Class (Virtual)
     */
protected:
    int n;             // Number of inputs
    signal_t *inputs;  // Inputs
    signal_t output;   // Output
    bool *flags;       // Flags of inputs (true if input is valid)
    int count;         // Count of valid inputs
    bool valid;        // Output is valid

public:
    explicit Gate(int numInputs);              // Constructor
    ~Gate();                                   // Destructor
    void Reset();                              // Reset gate
    int SetInput(int index, signal_t value);   // Set input signal value
    virtual void Calculate() = 0;              // Calculate output
    [[nodiscard]] signal_t GetOutput() const;  // Get output signal value
    [[nodiscard]] inline bool CheckInputFlag(int index) const {
        if (index >= 0 && index < n) {         // Check input flag
            return flags[index];
        }
        return false;
    }

    static bool CheckInputNum(int numInputs);  // Check number of inputs
};

class ANDGate : public Gate {
    /*
     * AND Gate
     */
public:
    explicit ANDGate(int numInputs);           // Constructor
    void Calculate() override;                 // Calculate output

    static bool CheckInputNum(int numInputs);  // Check number of inputs
};

class ORGate : public Gate {
    /*
     * OR Gate
     */
public:
    explicit ORGate(int numInputs);            // Constructor
    void Calculate() override;                 // Calculate output

    static bool CheckInputNum(int numInputs);  // Check number of inputs
};

class NOTGate : public Gate {
    /*
     * NOT Gate
     */
public:
    NOTGate();                                 // Constructor
    void Calculate() override;                 // Calculate output

    static bool CheckInputNum(int numInputs);  // Check number of inputs
};

class XORGate : public Gate {
    /*
     * XOR Gate
     */
public:
    explicit XORGate(int numInputs);           // Constructor
    void Calculate() override;                 // Calculate output

    static bool CheckInputNum(int numInputs);  // Check number of inputs
};

class NANDGate : public Gate {
    /*
     * NAND Gate
     */
public:
    explicit NANDGate(int numInputs);          // Constructor
    void Calculate() override;                 // Calculate output

    static bool CheckInputNum(int numInputs);  // Check number of inputs
};

class NORGate : public Gate {
    /*
     * NOR Gate
     */
public:
    explicit NORGate(int numInputs);           // Constructor
    void Calculate() override;                 // Calculate output

    static bool CheckInputNum(int numInputs);  // Check number of inputs
};

#endif //COMBLOGICSIM_GATES_H
