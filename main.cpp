#include <iostream>   // For Input and Output
#include <fstream>    // For file input and output
#include <vector>     // Input file is stored in a vector
#include <cassert>    // For the assert statement used to validate file open
#include <ctime>      // Needed to set the random number seed to be the current time & for timer
#include <cctype>     // Allows using the tolower(), toupper() functions
#include <string>     // For string()
using namespace std;  // To avoid prefacing cin and cout with std::


//--------------------------------------------------------------------------------
// Display welcome message, introducing the user to the program and
// describing the instructions for the game
void gameDisplayInstructions()
{
    cout << "Wordle Reload \n"
        << " \n"
        << "The objective of this game is to guess the randomly selected \n"
        << "word within a given number of attempts. You can select either \n"
        << "a three or five word board. \n"
        << "At the conlusion of the game, stats will be displayed.  \n"
        << "Indicators will be given if characters of the user entered \n"
        << "word are reflected in the guessed word. \n"
        << "  - If the character is in the correct position, the character \n"
        << "    will display as an uppercase value.\n"
        << "  - If the character is within the random word, the character \n"
        << "    will display as a lowercase value.\n"
        << "  - If you enter a character that is not in the word, an asterisk '*' \n"
        << "    will display."
        << " \n"
        << endl;
}// end displayWelcomeMessage()


//--------------------------------------------------------------------------------
// Read Wordle words into three-letter or five-letter vectors
// Vectors are passed by reference parameters so additions persist
void readWordleWords(vector <string> & threeLettered, vector <string> & fiveLettered)
{
    // Read Wordle words file into vectors
    ifstream inStream;                      // Declare an input stream for reading
    inStream.open("wordlewords.txt");       // Open Wordle words file
    assert(inStream.fail() == false);       // Ensure file open worked
    
    // If you had previously used the vector, then you might want to clear it
    // before continuing to use it, though that is not the case here.
    threeLettered.clear();
    fiveLettered.clear();
    
    // Keep repeating while input from the file yields a word
    string newWord;                         // Store a single input word
    while (inStream >> newWord) {           // While there is another word to be read
        if (newWord.length()==3) {
            // Add this new word to the end of the vector, growing it in the process
            threeLettered.push_back(newWord);
        }
        if (newWord.length()==5) {
            fiveLettered.push_back(newWord);
        }
    }
    
    // Close the Wordle words file
    inStream.close();
} // end readWordleWords()


//--------------------------------------------------------------------------------
// Binary search algorithm
int binarySearch(const vector <string> & wordleWords, string guessWord) {
    int mid;
    int low;
    int high;
    
    low = 0;
    high = wordleWords.size() - 1;
    
    while (high >= low) {
        mid = (high + low) / 2;
        if (wordleWords[mid] < guessWord) {
            low = mid + 1;
        }
        else if (wordleWords[mid] > guessWord) {
            high = mid - 1;
        }
        else {
            return mid;
        }
    }
    
    return -1; // not found
}


//--------------------------------------------------------------------------------
void wordlePlay(int numLetters, const vector <string> & wordleWords, int & totalCorrect, int & currentStreak, int & longestStreak, int & totalTime) 
{
    string randomWord = wordleWords.at(rand() % wordleWords.size());    // random index within range: 0..wordleWords.size()-1
    string guessWord;
    string outputWord = string(numLetters, '*');
    vector <string> allGuesses;
    int totalAttempts = 0;
    int timeLimit = numLetters * 10 - 10;
    time_t startTime;    // set to current time
    int elapsedSeconds = 0;

    // cout << randomWord;
  
    cout << "\nTo get started, enter your first " << numLetters << " letter word.\n"
        << "You have " << numLetters + 1 << " attempts to guess the random word.\n"
        << "The timer will start after your first word entry.\n"
        << "Try to guess the word within " << timeLimit << " seconds.\n";

    for (int attempt = 0; totalAttempts != numLetters+1; attempt++) {
        cout << "\nPlease enter word -->  ";
        cin >> guessWord;

        if (totalAttempts == 0) {
          startTime = time(NULL);
        }

        // Put user-inputted word as lowercase
        for (int i = 0; i < guessWord.length(); i++) {
            guessWord.at(i) = tolower(guessWord.at(i));
        }

        if (guessWord.length() > numLetters || guessWord.length() < numLetters) {
            cout << "Invalid word entry - please enter a word that is " << numLetters << " characters long.\n";
            continue;
        }
        
        // Check if user-entered word is in wordlewords.txt
        if (binarySearch(wordleWords, guessWord) == -1) {
            cout << "Not a playable word, please select another word.\n";
            continue;
        }

        for (int charIndex = 0; charIndex < numLetters; charIndex++) {
            // -  If character is not in word, display asterisk '*'
            if (randomWord.find(guessWord.at(charIndex)) == string::npos) {
                outputWord.at(charIndex) = '*';
            }
            // -	If character is in correct position, display it as uppercase
            else {
                if (randomWord.find(guessWord.at(charIndex), charIndex) == charIndex) {
                    outputWord.at(charIndex) = toupper(guessWord.at(charIndex));
                }
                // -	If character is within random word, display it as lowercase 
                else {
                    outputWord.at(charIndex) = guessWord.at(charIndex);
                } 
            }
        }

        // Store and print all guesses per word
        allGuesses.push_back(outputWord);
        cout << "\n\n";
        for (int guessNum = 0; guessNum < allGuesses.size(); guessNum++) {
            for (int position = 0; position < numLetters; position++) {
                cout << " [ " << allGuesses.at(guessNum).at(position) << " ] ";
            }
            cout << endl;
        }

        totalAttempts += 1;

        // Find the difference between the current time and the start time
        elapsedSeconds = difftime(time( NULL), startTime);
      
        if (randomWord.compare(guessWord) == 0) {
            cout << "\nNice Work!  You guessed the correct word\n"
                << "  - You completed the board in: " << elapsedSeconds << " seconds.\n"
                << "  - It took you " << totalAttempts << "/" << numLetters+1 << " attempts.\n";
            totalTime += elapsedSeconds;
            totalCorrect += 1;
            currentStreak += 1;
            if (currentStreak > longestStreak) {
                longestStreak = currentStreak;
            }
            break;
        }

        if (elapsedSeconds > timeLimit) {
            cout << "Your time has expired.  Try again.\n"
                << "  - You are " << elapsedSeconds - timeLimit << " seconds over the " << timeLimit << " second time limit.\n";
            if (currentStreak > longestStreak) {
                longestStreak = currentStreak;
            }
            currentStreak = 0;    // reset
            break;
        } 
      
        else if (totalAttempts == numLetters+1 && randomWord.compare(guessWord) != 0) {
            cout << "\nMaximum amount of attempts have been reached. Try again.\n";
            if (currentStreak > longestStreak) {
                longestStreak = currentStreak;
            }
            currentStreak = 0;    // reset
            break;
        }

    }
  
} // end menuOption1()


//--------------------------------------------------------------------------------
int main() 
{
    int menuOption;
    int totalCorrect = 0;
    int currentStreak = 0;
    int longestStreak = 0;
    int totalTime = 0;
  
    // Read in Wordle words from file and store into 3-word and 5-word vectors
    vector <string> threeLettered;
    vector <string> fiveLettered;
    readWordleWords(threeLettered, fiveLettered);

    // Display game instructions
    gameDisplayInstructions();

    // Seed the random number generator
    srand(time(NULL));  // leave at default 1 for test cases / predictable results, 
                        // otherwise time(0) or time(NULL) for different results
  
    // Keep looping to display menu until user selects the option to exit
    while (true) {
        cout << "\nSelect a menu option: \n"
            << "  1. To play Wordle Reload 3 letter play \n"
            << "  2. To play Wordle Reload 5 letter play \n"
            << "  3. Exit the program \n"
            << "Your choice --> ";
        
        cin >> menuOption;
      
        switch(menuOption) {
            case(1): {
                wordlePlay(3, threeLettered, totalCorrect, currentStreak, longestStreak, totalTime);
                break;
            }
            case(2): {
                wordlePlay(5, fiveLettered, totalCorrect, currentStreak, longestStreak, totalTime);
                break;
            }
            case(3): {
                cout << "\nOverall Stats:\n"
                    << "  - You guessed: " << totalCorrect << "\n"
                    << "  - Your longest streak is: " << longestStreak << "\n";
                if ((totalCorrect == 0) && (longestStreak == 0)) {
                    cout << "  - Average word completion time: N/A\n";
                } else {
                    cout << "  - Average word completion time: " << totalTime / longestStreak << "\n";
                }
                cout << "Exiting program";
              
                totalCorrect = 0;       // reset
                longestStreak = 0;      // reset
                totalTime = 0;          // reset
                exit(0);
                break;
            }
            default: {
                cout << "\nInvalid menu option, please try again!\n";
                exit(0);
                break;
            }
        }
    }

	  return 0;
} // end main()

