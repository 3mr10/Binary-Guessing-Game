#include <iostream>
#include <cmath>
#include <cstdlib> // For rand(), srand()
#include <ctime>   // For time()
#include <string>

using namespace std;


// Calculates the ceiling of log2(range size) 
int calculateMaxGuesses(int startMin, int startMax) {
    if (startMax < startMin) return 0;

    int rangeSize = startMax - startMin + 1;
 
    return (int)ceil(log2(rangeSize));
}

class GuessGame {
protected:
    const int MIN_RANGE;
    const int MAX_RANGE;

public:
    GuessGame(int min, int max) : MIN_RANGE(min), MAX_RANGE(max) {}

    virtual ~GuessGame() {}

    // Pure Virtual Function: Forces subclasses to define their own game logic
    virtual void play() = 0;
};

// ----------------------------------------------------------------------
// Subclass 1:
// ----------------------------------------------------------------------
class ComputerGuesser : public GuessGame {
private:
    int minRange;
    int maxRange;
    int currentGuess;

public:
    ComputerGuesser(int min, int max) : GuessGame(min, max), minRange(min), maxRange(max), currentGuess(0) {
        makeGuess();
    }

    int makeGuess() {
        currentGuess = minRange + (maxRange - minRange) / 2;
        return currentGuess;
    }

    bool receiveFeedback(char feedbackCode) {
        if (feedbackCode == 'C' || feedbackCode == 'c') return true;

        if (feedbackCode == 'H' || feedbackCode == 'h') {
            minRange = currentGuess + 1;
        }
        else if (feedbackCode == 'L' || feedbackCode == 'l') {
            maxRange = currentGuess - 1;
        }

        if (minRange > maxRange) {
            cout << "\n Wait, your number must not be in the range you provided, or you gave inconsistent feedback." << endl;
            return true;
        }
        return false;
    }

    int getCurrentGuess() const { return currentGuess; }
    int getMinRange() const { return minRange; }
    int getMaxRange() const { return maxRange; }

    void play() override {
        int maxRequiredGuesses = calculateMaxGuesses(MIN_RANGE, MAX_RANGE);
        char userFeedback = ' ';
        int guessCount = 0;

        cout << "\n--- Mode 1: Computer Guesses ---" << endl;
        cout << "Please THINK of a number between " << MIN_RANGE << " and " << MAX_RANGE << "." << endl;
        cout << "The computer is guaranteed to guess it in a maximum of " << maxRequiredGuesses << " tries." << endl;
        cout << "------------------------------------------" << endl;

        while (userFeedback != 'C' && userFeedback != 'c') {
            guessCount++;
            int guess = makeGuess();

            cout << "\n--- GUESS #" << guessCount << " (Current Range: " << getMinRange() << "-" << getMaxRange() << ") ---" << endl;
            cout << "Is your number **" << guess << "**?" << endl;
            cout << "Enter (H) for Higher, (L) for Lower, or (C) for Correct: ";
            cin >> userFeedback;

            // Secondary Loss Condition (exceeds theoretical max)
            if (guessCount > maxRequiredGuesses && userFeedback != 'C' && userFeedback != 'c') {
                cout << "\n COMPUTER LOSES! The algorithm was exceeded." << endl;
                cout << "The computer should have solved this already. You must have given inconsistent feedback." << endl;
                break;
            }

            if (receiveFeedback(userFeedback)) {
                break;
            }
        }

        if (userFeedback == 'C' || userFeedback == 'c') {
            cout << "\n VICTORY! The computer guessed your number (" << getCurrentGuess() << ") in "
                << guessCount << " guesses!" << endl;
        }
        else {
            cout << "\nGame Over." << endl;
        }
    }
};

// ----------------------------------------------------------------------
// Subclass 2:
// ----------------------------------------------------------------------
class HumanGuesser : public GuessGame {
private:
    int secretNumber;

    void generateSecretNumber() {
        srand(time(0));
        secretNumber = rand() % (MAX_RANGE - MIN_RANGE + 1) + MIN_RANGE;
    }

    string giveFeedback(int guess) {
        if (guess < secretNumber) {
            return "Too Low (Higher)!";
        }
        else if (guess > secretNumber) {
            return "Too High (Lower)!";
        }
        else {
            return "Correct!";
        }
    }

public:
    HumanGuesser(int min, int max) : GuessGame(min, max) {
        generateSecretNumber();
    }

    void play() override {
        int maxRequiredGuesses = calculateMaxGuesses(MIN_RANGE, MAX_RANGE);
        int userGuess = 0;
        int guessCount = 0;
        string feedback = "";

        cout << "\n--- Mode 2: Human Guesses ---" << endl;
        cout << "I have picked a secret number between " << MIN_RANGE << " and " << MAX_RANGE << "." << endl;
        cout << "Your challenge: Win by using the Binary Search Strategy in " << maxRequiredGuesses << " or fewer tries!" << endl;
        cout << "------------------------------------------" << endl;

        while (feedback != "Correct!") {
            guessCount++;

            // CHECK 1: The Human has exceeded the optimal limit
            if (guessCount > maxRequiredGuesses) {
                cout << "\n YOU LOSE! You exceeded the maximum optimal guesses (" << maxRequiredGuesses << "). The number was " << secretNumber << "." << endl;
                cout << "To win this game, you must strictly follow the Binary Search method ." << endl;
                return; 
            }

            cout << "\n> Guess #" << guessCount << ": Enter your guess: ";

            if (!(cin >> userGuess)) {
                cout << " Invalid input. Please enter a number." << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                guessCount--; // Do not count invalid input as a guess
                continue;
            }

            if (userGuess < MIN_RANGE || userGuess > MAX_RANGE) {
                cout << " Your guess must be within the original range (" << MIN_RANGE << "-" << MAX_RANGE << ")." << endl;
                guessCount--; // Do not count out-of-range input
                continue;
            }

            feedback = giveFeedback(userGuess);

            if (feedback == "Correct!") {
                cout << "\n CORRECT! You found " << secretNumber
                    << " in " << guessCount << " guesses." << endl;
                cout << "You won within the required limit of " << maxRequiredGuesses << " guesses!" << endl;
            }
            else {
                cout << "➡️ " << feedback << " Try again." << endl;
            }
        }
    }
};

int main() {
    int startMin = 1;
    int startMax = 100;
    int mode;

    cout << "--- Binary Guessing Game Engine ---" << endl;

    // Get Range Input
    cout << "Enter the MINIMUM number for the search range (e.g., 1): ";
    cin >> startMin;
    cout << "Enter the MAXIMUM number for the search range (e.g., 1000): ";
    cin >> startMax;

    if (startMax <= startMin || cin.fail()) {
        cout << "Invalid range or input." << endl;
        return 1;
    }

    // Get Mode Input
    cout << "\nSELECT MODE:" << endl;
    cout << "1. Computer Guesses (You pick the number)" << endl;
    cout << "2. Human Guesses (Computer picks the number)" << endl;
    cout << "Enter mode (1 or 2): ";
    cin >> mode;

    GuessGame* game = nullptr; // Polymorphic pointer

    switch (mode) {
    case 1:
        game = new ComputerGuesser(startMin, startMax);
        break;
    case 2:
        game = new HumanGuesser(startMin, startMax);
        break;
    default:
        cout << "Invalid mode selected. Exiting." << endl;
        return 1;
    }

    // Call the play function polymorphically
    game->play();

    delete game;

    return 0;
}
