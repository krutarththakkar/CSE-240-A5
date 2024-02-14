
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define MAX_LEADERS 5

struct Player {
    char name[50];
    int guesses;
};

void saveLeaders(struct Player leaders[], int numLeaders) {
    FILE *file = fopen("leaders.txt", "w");
    if (!file) {
        return;
    }

    for (int i = 0; i < numLeaders; ++i) {
        fprintf(file, "%s %d\n", leaders[i].name, leaders[i].guesses);
    }

    fclose(file);
}

void readLeaders(struct Player leaders[], int *numLeaders) {
    FILE *file = fopen("leaders.txt", "r");
    if (!file) {
        *numLeaders = 0;
        return;
    }

    *numLeaders = 0;
    while (*numLeaders < 5 && fscanf(file, "%s %d", leaders[*numLeaders].name, &leaders[*numLeaders].guesses) == 2) {
        ++(*numLeaders);
    }

    fclose(file);
}

int main() {
    char input;
    char playerName[50];
    int guess;
    int numberToGuess;
    int minRange = 10;
    int maxRange = 100;
    int numLeaders = 0;
    struct Player leaders[5];

    readLeaders(leaders, &numLeaders);

    for (;;) {
        printf("Welcome! Press 'q' to quit or any other key to continue: ");
        scanf(" %c", &input);

        if (input == 'q' || input == 'Q') {
            printf("Bye Bye!\n");
            break;
        }

        printf("Please enter your name to start: ");
        scanf("%s", playerName);

        int playerGuesses = 0;
        srand(time(NULL));
        numberToGuess = rand() % (maxRange - minRange + 1) + minRange;

        do {
            double sqrtNumber = sqrt(numberToGuess);
            printf("%.8lf is the square root of what number? Guess a value between %d and %d: ", sqrtNumber, minRange, maxRange);
            scanf("%d", &guess);
            ++playerGuesses;

            if (guess < numberToGuess) {
                printf("Too low, guess again: ");
            } else if (guess > numberToGuess) {
                printf("Too high, guess again: ");
            } else {
                printf("You got it, baby!\n");
                printf("You made %d guess%s.\n", playerGuesses, (playerGuesses == 1) ? "" : "es");

                int found = 0;
                for (int i = 0; i < numLeaders; ++i) {
                    if (strcmp(leaders[i].name, playerName) == 0) {
                        if (playerGuesses < leaders[i].guesses) {
                            leaders[i].guesses = playerGuesses;
                        }
                        found = 1;
                        break;
                    }
                }

                if (!found) {
                    if (numLeaders < 5) {
                        strcpy(leaders[numLeaders].name, playerName);
                        leaders[numLeaders].guesses = playerGuesses;
                        ++numLeaders;
                    } else {
                        int maxGuessesIndex = 0;
                        for (int j = 1; j < numLeaders; ++j) {
                            if (leaders[j].guesses > leaders[maxGuessesIndex].guesses) {
                                maxGuessesIndex = j;
                            }
                        }

                        if (playerGuesses < leaders[maxGuessesIndex].guesses) {
                            strcpy(leaders[maxGuessesIndex].name, playerName);
                            leaders[maxGuessesIndex].guesses = playerGuesses;
                        }
                    }
                }

                for (int i = 0; i < numLeaders - 1; ++i) {
                    for (int j = i + 1; j < numLeaders; ++j) {
                        if (leaders[i].guesses > leaders[j].guesses) {
                            struct Player temp = leaders[i];
                            leaders[i] = leaders[j];
                            leaders[j] = temp;
                        }
                    }
                }

                printf("Here are the current leaders:\n");
                for (int i = 0; i < numLeaders; ++i) {
                    printf("%s made %d guess%s\n", leaders[i].name, leaders[i].guesses, (leaders[i].guesses == 1) ? "" : "es");
                }
            }
        } while (guess != numberToGuess);

        saveLeaders(leaders, numLeaders);
    }

    return 0;
}
