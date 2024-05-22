#include <gtest/gtest.h>
#include "circuit.h"

class CircuitTest : public ::testing::Test {
protected:
    Circuit* circuit;

    void SetUp() override {
        circuit = new Circuit(5, 3);
    }

    void TearDown() override {
        delete circuit;
    }
};

TEST_F(CircuitTest, Constructor) {
    EXPECT_EQ(circuit->GetNumGates(), 5);
    EXPECT_EQ(circuit->GetNumInputs(), 3);
    EXPECT_EQ(circuit->GetState(), CIRCUIT_STATE_WAIT);
}

TEST_F(CircuitTest, RunCircuit) {
    /*
     * Test Circuit
     * Add the following gates:
     *   XOR 2 I1 I2
     *   XOR 2 O1 I3
     *   AND 2 O1 I3
     *   AND 2 I1 I2
     *   OR 2 O3 O4
     * Set the inputs to:
     *   0 1 1
     * Then reset the inputs to:
     *   1 1 1
     * The output of the circuit should be:
     */
    std::string pins[2] = {"I1", "I2"};
    circuit->AddGate(0, GATE_TYPE_XOR, 2, pins);

    pins[0] = "O1"; pins[1] = "I3";
    circuit->AddGate(1, GATE_TYPE_XOR, 2, pins);

    pins[0] = "O1"; pins[1] = "I3";
    circuit->AddGate(2, GATE_TYPE_AND, 2, pins);

    pins[0] = "I1"; pins[1] = "I2";
    circuit->AddGate(3, GATE_TYPE_AND, 2, pins);

    pins[0] = "O3"; pins[1] = "O4";
    circuit->AddGate(4, GATE_TYPE_OR, 2, pins);

    EXPECT_EQ(circuit->GetState(), CIRCUIT_STATE_WAIT);

    // Set the inputs to 0 1 1
    circuit->Reset();
    EXPECT_EQ(circuit->GetState(), CIRCUIT_STATE_WAIT);
    circuit->SetInput(0, 0);
    circuit->SetInput(1, 1);
    circuit->SetInput(2, 1);
    circuit->Run();

    EXPECT_EQ(circuit->GetState(), CIRCUIT_STATE_OK);
    EXPECT_EQ(circuit->GetOutput(4), 1);
    EXPECT_EQ(circuit->GetOutput(1), 0);

    // Set the inputs to 1 1 1
    circuit->Reset();
    EXPECT_EQ(circuit->GetState(), CIRCUIT_STATE_WAIT);
    circuit->SetInput(0, 1);
    circuit->SetInput(1, 1);
    circuit->SetInput(2, 1);
    circuit->Run();

    EXPECT_EQ(circuit->GetState(), CIRCUIT_STATE_OK);
    EXPECT_EQ(circuit->GetOutput(4), 1);
    EXPECT_EQ(circuit->GetOutput(1), 1);
}

TEST_F(CircuitTest, LoopCircuit) {
    /*
     * Test Loop Circuit
     * Add the following gates:
     *   XOR 2 I1 O2
     *   XOR 2 O1 I3
     *   AND 2 O1 I3
     *   AND 2 I1 I2
     *   OR 2 O3 O4
     * Set the inputs to:
     *   0 1 1
     */
    std::string pins[2] = {"I1", "O2"};
    circuit->AddGate(0, GATE_TYPE_XOR, 2, pins);

    pins[0] = "O1"; pins[1] = "I3";
    circuit->AddGate(1, GATE_TYPE_XOR, 2, pins);

    pins[0] = "O1"; pins[1] = "I3";
    circuit->AddGate(2, GATE_TYPE_AND, 2, pins);

    pins[0] = "I1"; pins[1] = "I2";
    circuit->AddGate(3, GATE_TYPE_AND, 2, pins);

    pins[0] = "O3"; pins[1] = "O4";
    circuit->AddGate(4, GATE_TYPE_OR, 2, pins);

    EXPECT_EQ(circuit->GetState(), CIRCUIT_STATE_WAIT);

    // Set the inputs to 0 1 1
    circuit->Reset();
    EXPECT_EQ(circuit->GetState(), CIRCUIT_STATE_WAIT);
    circuit->SetInput(0, 0);
    circuit->SetInput(1, 1);
    circuit->SetInput(2, 1);
    circuit->Run();

    EXPECT_EQ(circuit->GetState(), CIRCUIT_STATE_LOOP);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
