    #include <iostream>
    #include <cmath>
    using namespace std;
// this code make a binary guessing game .
    // computer only can guess the game 


    class ComputerGuesser {
    private:
        int minRange;
        int maxRange;
        int currentGuess;

    public:
    
        ComputerGuesser(int min, int max) : minRange(min), maxRange(max), currentGuess(0) {
          
            makeGuess();
        }

        int makeGuess() {
// the core of the logic of binary game
            currentGuess = minRange + (maxRange - minRange) / 2;
            return currentGuess;
        }

       // operations on current guess and the status of game
        bool receiveFeedback(char feedbackCode) {
            if (feedbackCode == 'C' || feedbackCode == 'c') {
                // Correct guess, game over
                return true;
            }
            // if higher 
            if (feedbackCode == 'H' || feedbackCode == 'h') {
               
                minRange = currentGuess + 1;
            }
            // il lower
            else if (feedbackCode == 'L' || feedbackCode == 'l') {
                
                maxRange = currentGuess - 1;
            }

            // check min >max
            if (minRange > maxRange) {
                cout << "\n Wait, your number must not be in the range you provided, or you gave inconsistent feedback." << endl;
                return true; // i mentioned it in main 
            }

           
            return false;
        }

        // Accessors for range and guess
        int getCurrentGuess() const {
            return currentGuess;
        }

        int getMinRange() const { return minRange; }
        int getMaxRange() const { return maxRange; }
    };

    // calculate number of guesses according to log2(max);
    int calculateMaxGuesses(int startMin, int startMax) {
        if (startMax < startMin) return 0;
        int rangeSize = startMax - startMin ;
 
        return (int)ceil(log2(rangeSize))-1;// complexity and simplicity
    }

    // --- Main Game Execution ---
    int main() {
        // default min max
        int startMin = 1;
        int startMax = 100; 

        cout << "--- Binary Guessing Game Engine ---" << endl;

        // the new values of min max
        cout << "Enter the MINIMUM number for the search range (e.g., 1): ";
        cin >> startMin;

        cout << "Enter the MAXIMUM number for the search range (e.g., 1000): ";
        cin >> startMax;

        // a check step to ensure that max>min (check 1)
        if (startMax <= startMin) {
            cout << "Invalid range. Maximum must be greater than minimum." << endl;
            return 1;
        }

        // call calculate guess function to get the number of turns
        int maxRequiredGuesses = calculateMaxGuesses(startMin, startMax);


  
        cout << "------------------------------------------" << endl<< endl ;
        cout << "Please THINK  of a number between " << startMin << " and " << startMax << "." << endl<< endl ;
        cout << "The computer is guaranteed to guess it in a maximum of " << maxRequiredGuesses << " tries." << endl<< endl;
        cout << "------------------------------------------" << endl;

        // intialize the class that has the function that is the backbone of the game
        ComputerGuesser game=  ComputerGuesser(startMin, startMax);
   
        //
        char userFeedback = ' ';
        int guessCount = 0;

        // Main Game Loop
        while (userFeedback != 'C' && userFeedback != 'c') {
            guessCount++;

            // intialize guess to make it in the
            int guess = game.makeGuess();

            cout << "\n--- GUESS #" << guessCount << " (Current Range: " << game.getMinRange() << "-" << game.getMaxRange() << ") ---" << endl;
            cout << "Is your number **" << guess << "**?" << endl;

            // Input/Feedback
            cout << "Enter (H) for Higher, (L) for Lower, or (C) for Correct: ";
            cin >> userFeedback;

            // return that computer loses
            if (guessCount > maxRequiredGuesses && userFeedback != 'C' && userFeedback != 'c') {
                cout << "\n COMPUTER LOSES! The algorithm was exceeded." << endl;
                cout << "The computer should have solved this already. You must have given inconsistent feedback." << endl;
                break;
            }

            // Process Feedback and update search space
            if (game.receiveFeedback(userFeedback)) {
               // return true to stop after a decision
                break;
            }
        }

        // 4. Game End Output  final messages
        if (userFeedback == 'C' || userFeedback == 'c') {
            cout << "\n VICTORY! The computer guessed your number (" << game.getCurrentGuess() << ") in "
                << guessCount << " guesses!" << endl;
        }
        else if (guessCount > maxRequiredGuesses) {
           
            cout << "\n Game Over due to inconsistent feedback." << endl;
        }
        else {
            cout << "\nG_a_m_e O_v_e_r." << endl;
        }

        return 0;
    }