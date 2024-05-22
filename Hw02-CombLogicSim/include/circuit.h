// Circuit

#ifndef COMBLOGICSIM_CIRCUIT_H
#define COMBLOGICSIM_CIRCUIT_H

#include <string>

#include "config.h"
#include "gates.h"
#include "exception.h"

// Circuit State
#define CIRCUIT_STATE_OK 0
#define CIRCUIT_STATE_WAIT 1  // DEFAULT STATE
#define CIRCUIT_STATE_LOOP 2
#define CIRCUIT_STATE_ERROR (-1)

class Circuit {
    /*
     * Circuit Class
     */
    struct GateInfo {
        int type;           // Gate type
        int numInputs;      // Number of inputs
        std::string *pins;  // Pin names

        GateInfo() : type(-1), numInputs(0), pins(nullptr) {}
        ~GateInfo() {
            delete[] pins;
        }
    };

private:
    int numGates;           // Number of gates
    int numInputs;          // Number of inputs
    Gate **gates;           // Gates
    GateInfo *gateInfos;    // Gate information
    signal_t *inputs;       // Inputs
    bool *flags;            // Flags of inputs
    int state;              // Circuit State

public:
    explicit Circuit(int _numGates, int _numInputs);                         // Constructor
    ~Circuit();                                                              // Destructor
    void AddGate(int index, int type, int gateNumInputs, std::string *pins); // Add gate
    void SetInput(int index, signal_t value);                                // Set input signal value
    void Reset();                                                            // Reset circuit (only inputs)
    void Run();                                                              // Run the circuit
    [[nodiscard]] signal_t GetOutput(int index) const;                       // Get output signal value
    [[nodiscard]] inline int GetState() const {                              // Get circuit state
        return state;
    }
    [[nodiscard]] inline int GetNumGates() const {                           // Get number of gates
        return numGates;
    }
    [[nodiscard]] inline int GetNumInputs() const {                          // Get number of inputs
        return numInputs;
    }
};

#endif //COMBLOGICSIM_CIRCUIT_H
