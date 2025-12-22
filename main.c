#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>  // For gettimeofday()

int main() {
    char *sentences[] = {
        "The quick brown fox jumps over the lazy dog",
        "Pack my box with five dozen liquor jugs",
        "How vexingly quick daft zebras jump",
        "The five boxing wizards jump quickly",
        "Sphinx of black quartz judge my vow",
        "Two driven jocks help fax my big quiz",
        "Five quacking zephyrs jolt my wax bed",
        "The job requires extra pluck and zeal from every young wage earner",
        "A quick movement of the enemy will jeopardize six gunboats",
        "All questions asked by five watch experts amazed the judge"
    };
    
    int num_sentences = 10;
    char input[200];
    char play_again;
    int len, i, correct, words;
    double t, wpm, acc;
    struct timeval start, end;  // For microsecond precision
    
    // Seed random number generator
    srand(time(NULL));
    
    do {
        // Pick a random sentence
        int random_index = rand() % num_sentences;
        char *text = sentences[random_index];
        
        printf("\n=== GAME START ===\n");
        printf("Type the following sentence:\n");
        printf("%s\n", text);
        printf("> ");
        
        gettimeofday(&start, NULL);  // Start timer
        fgets(input, 200, stdin);
        gettimeofday(&end, NULL);    // End timer
        input[strcspn(input, "\n")] = 0;
        
        len = strlen(text);
        correct = 0;
        words = 1;
        for(i = 0; i < len; i++) {
            if(i < strlen(input) && text[i] == input[i]) correct++;
            if(text[i] == ' ') words++;
        }
        
        // Calculate elapsed time in seconds with microsecond precision
        t = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
        
        acc = (correct * 100.0) / len;
        
        // Prevent division by zero
        if(t > 0) {
            wpm = (words * 60.0) / t;
        } else {
            wpm = 0;
        }
        
        printf("\n=== GAME OVER ===\n");
        printf("Time: %.2fs | Accuracy: %.2f%% | WPM: %.2f\n", t, acc, wpm);
        
        printf("\nDo you want to play again? (y/n): ");
        scanf(" %c", &play_again);
        getchar(); // Clear newline from buffer
        
    } while(play_again == 'y' || play_again == 'Y');
    
    printf("\nThanks for playing!\n");
    
    return 0;
}
