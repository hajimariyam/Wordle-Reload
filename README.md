# Wordle-Reload
 
*A twist on Wordle, programmed in C++.*

Guess a randomly selected 3-letter or 5-letter word within a given number of attempts and seconds. Indicators are given if characters of the user-entered word are reflected in the guessed word. Stats are displayed at the end of the game.

## Features:
- Words are chosen from wordlewords.txt at random. The seed for the random number is created from the current time.
- Binary search is used to ensure that any word the user enters is a playable word, i.e., it is in wordlewords.txt
- If the character is in the correct position, the character will display as an uppercase value. Example: 
  - Random word: fan
  - Guessed word: sad
  - Output: [ * ] [ A ] [ * ]
- If the character is within the random word, the character will display as a lowercase value. Example:
  - Random word: fan
  - Guessed word: net
  - Output: [ n ] [ * ] [ * ]
- If you enter a character that is not in the word, an asterisk '*' will display. Example:
  - Random word: fan
  - Guessed word: tic
  - Output: [ * ] [ * ] [ * ]
- A board with previously attempted words is displayed with each new attempt in the round. Example:
  - [ e ] [ R ] [ * ] [ * ] [ E ]
  - [ * ] [ R ] [ I ] [ M ] [ E ]
  - [ * ] [ * ] [ * ] [ * ] [ * ]
  - [ * ] [ * ] [ * ] [ * ] [ * ]
- After each round, the number of attempts and seconds used are displayed. The start timestamp is used to determine how many seconds have elapsed.
- Each rounds ends when the word is guessed correctly, the timer runs out, or all attempts are used.
- There is no limit to the number of rounds the user can play; each round has a different randomized word.
- When the user exits the game, overall stats are displayed. Example:
  - You guessed: 7
  - Your longest streak is: 4
  - Average word completion time: 0


