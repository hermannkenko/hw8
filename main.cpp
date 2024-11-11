#include <iostream>
#include <bitset>
#include <cstdlib>
#include <ctime>
#include <cstdint> // Required for uint8_t

class MotorStatus {
private:
    uint8_t motorState;       // Each bit represents the status of a motor (1 = ON, 0 = OFF)
    uint8_t overheatingMotors; // Each bit represents an overheating motor (1 = overheating)
    int turnCount;             // Tracks the number of attempts or turns

    // Initializes motor states and starts with one randomly selected motor overheating
    void initialize() {
        srand(time(0));                   // Seed random number generator
        motorState = rand() % 256;        // Randomly set motor states (on/off)
        updateOverheating();              // Select an initial overheating motor
        turnCount = 0;                    // Reset turn count
        displayStatus();                  // Display initial motor status
    }

    // Randomly selects a new motor to overheat that is currently ON and not overheating
    void updateOverheating() {
        int newMotor;
        do {
            newMotor = rand() % 8;       // Randomly select a motor (0 to 7)
        } while (((1 << newMotor) & motorState) == 0 ||     // Ensure motor is ON
                 ((1 << newMotor) & overheatingMotors) != 0); // Ensure motor is not already overheating
        
        overheatingMotors |= (1 << newMotor);  // Set the selected motor to overheat
    }
    
    // Displays the current state of the motors, overheating status, and turn count
    void displayStatus() {
        std::cout << "Motor State (ON/OFF): " << std::bitset<8>(motorState) << std::endl;
        std::cout << "Overheating Motors:   " << std::bitset<8>(overheatingMotors) << std::endl;
        std::cout << "Turn Count: " << turnCount << std::endl;
    }

public:
    MotorStatus() { initialize(); }  // Constructor calls initialize() to set up the motors

    // Attempts to turn off motors that match the user’s guess and are overheating
    bool turnOff(uint8_t guess) {
        turnCount++;  // Increment turn count for each guess attempt

        // Determine motors that are both guessed and currently overheating
        uint8_t motorsToTurnOff = overheatingMotors & guess;
        
        // Update motorState to turn off guessed overheating motors
        motorState &= ~motorsToTurnOff;

        // Check if guessed motors were actually overheating and turned off successfully
        if (motorsToTurnOff > 0) { // If there was an overheating motor matched by the guess
            overheatingMotors &= ~motorsToTurnOff;  // Update overheating status to reflect shut-off motors
            displayStatus();                        // Show updated motor status
            return overheatingMotors == 0;          // Return true if all overheating motors are turned off
        }

        // If no overheating motors matched, overheat an additional motor
        updateOverheating();
        displayStatus();                             // Show updated motor and overheating status
        return false;
    }
};

int main() {
    MotorStatus motorStatus;     // Create a MotorStatus object, initializing motors

    uint8_t guess = 0b00000001;  // Initial guess for turning off an overheating motor
    while (!motorStatus.turnOff(guess)) { // Loop until all overheating motors are turned off
        guess <<= 1;                      // Shift guess to check the next motor in sequence
        if (guess == 0) guess = 0b00000001; // Reset guess to the first motor if it overflows past 8 bits
    }
    std::cout << "All overheating motors have been turned off!" << std::endl;
    return 0;
}


