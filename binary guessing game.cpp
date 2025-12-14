#include <iostream>
#include <cmath> // Not strictly needed for floor(), but good for complex math

using namespace std;

// The class that holds the binary search logic for the computer's guessing
class ComputerGuesser {
private:
    int minRange;
    int maxRange;
    int currentGuess;

public:
    // 1. Constructor: Initializes the search range
    ComputerGuesser(int min, int max) : minRange(min), maxRange(max), currentGuess(0) {

        makeGuess();
    }

    // 2. makeGuess: Calculates the next optimal guess (the midpoint)
    int makeGuess() {

        currentGuess = minRange + (maxRange - minRange) / 2;
        return currentGuess;
    }

    // 'H' = Higher, 'L' = Lower, 'C' = Correct
    bool receiveFeedback(char feedbackCode) {
        if (feedbackCode == 'C' || feedbackCode == 'c') {
            // Correct guess, game over
            return true;
        }

        if (feedbackCode == 'H' || feedbackCode == 'h') {
            // Secret number is HIGHER than the guess.
            // Adjust the minimum bound to one more than the current guess.
            minRange = currentGuess + 1;
        }
        else if (feedbackCode == 'L' || feedbackCode == 'l') {
            // Secret number is LOWER than the guess.
            // Adjust the maximum bound to one less than the current guess.
            maxRange = currentGuess - 1;
        }

        // Check if the search space is still valid
        if (minRange > maxRange) {
            cout << "\n Wait, your number must not be in the range you provided, or you gave inconsistent feedback." << endl;
            return true; // Force game end due to inconsistency
        }

        // Return false to indicate the game should continue
        return false;
    }

    // Accessor for the current guess
    int getCurrentGuess() const {
        return currentGuess;
    }

    // Accessor for the current range limits (for printing instructions)
    int getMinRange() const { return minRange; }
    int getMaxRange() const { return maxRange; }
};

// --- Main Game Execution ---
int main() {

    // some start talk
    const int START_MIN = 1;
    const int START_MAX = 1000;

    // the first display message
    cout << " Welcome to the Computer-Guesser game!" << endl;
    cout << "------------------------------------------" << endl;
    cout << "Please **think of a number** between " << START_MIN << " and " << START_MAX << " (inclusive)." << endl;
    cout << "The computer will try to guess it in 7 or fewer tries." << endl;
    cout << "------------------------------------------" << endl;

    // declare computerGuesser class

    ComputerGuesser game(START_MIN, START_MAX);

    char userFeedback = ' ';
    int guessCount = 0;// calculate the number of gusses of computer "COPMUTER GUESS"

    // 2. Main Game Loop
    while (userFeedback != 'C' && userFeedback != 'c') {
        guessCount++;

        // 2a. Computer makes a guess (Binary Search step)
        int guess = game.makeGuess();

        cout << "\n--- GUESS #" << guessCount << " (Range: " << game.getMinRange() << "-" << game.getMaxRange() << ") ---" << endl;
        cout << "Is your number **" << guess << "**?" << endl;

        // 2b. Input/Feedback
        cout << "Enter (H) for Higher, (L) for Lower, or (C) for Correct: ";
        cin >> userFeedback;

        // Handle a common error/break case before processing
        if (guessCount > 7 && guess <= START_MAX) {
            cout << "\n ERROR: The computer should have solved this already! You may have given inconsistent feedback." << endl;
            break;
        }

        // 2c. Process Feedback and update search space
        if (game.receiveFeedback(userFeedback)) {
            // The function returns true if the game is over (Correct or inconsistent)
            break;
        }
    }

    // 3. Game End Output
    if (userFeedback == 'C' || userFeedback == 'c') {
        cout << "\n🎉 VICTORY! The computer guessed your number (" << game.getCurrentGuess() << ") in "
            << guessCount << " guesses!" << endl;
    }
    else {
        cout << "\nGame Over." << endl;
    }

    return 0;
}
