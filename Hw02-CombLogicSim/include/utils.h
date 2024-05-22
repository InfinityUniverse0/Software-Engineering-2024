// Utils

#ifndef COMBLOGICSIM_UTILS_H
#define COMBLOGICSIM_UTILS_H

#include "exception.h"
#include "gates.h"
#include "circuit.h"

// Gate Initialization from standard input
void GateInit(Circuit &circuit);

// Input Initialization
void InputInit(Circuit &circuit, signal_t *inputs, int numInputs);

#endif //COMBLOGICSIM_UTILS_H
