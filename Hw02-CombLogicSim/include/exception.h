// Exception handling

#ifndef COMBLOGICSIM_EXCEPTION_H
#define COMBLOGICSIM_EXCEPTION_H

#include <exception>
#include <string>

// Circuit Exception
#define CIRCUIT_EXCEPTION_INDEX_OUT_OF_RANGE "Circuit Exception: Index out of range"
#define CIRCUIT_EXCEPTION_GATE_TYPE_ERROR "Circuit Exception: Gate type error"
#define CIRCUIT_EXCEPTION_INVALID_INPUT "Circuit Exception: Invalid input"
#define CIRCUIT_EXCEPTION_RUNTIME_ERROR "Circuit Exception: Runtime error"
#define CIRCUIT_EXCEPTION_VALUE_RESET_WARNING "Circuit Exception: Value reset warning"

class CircuitException : public std::exception {
    /*
     * Circuit Exception
     */
private:
    std::string message; // Exception message

public:
    explicit CircuitException(const std::string &msg) : message(msg) {}

    [[nodiscard]] const char *what() const noexcept override {
        return message.c_str();
    }
};

#endif //COMBLOGICSIM_EXCEPTION_H
