#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// ANSI color codes for better visual feedback
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

// Configuration constants
#define MAX_INPUT_LENGTH 1000
#define WORDS_PER_BLOCK 40  // Number of words in each typing block

// Word bank with various difficulty levels - these are common English words
const char *word_bank[] = {
    "the", "quick", "brown", "fox", "jumps", "over", "lazy", "dog",
    "computer", "program", "typing", "speed", "accuracy", "practice",
    "keyboard", "monitor", "software", "hardware", "internet", "network",
    "algorithm", "function", "variable", "constant", "structure", "pointer",
    "memory", "process", "thread", "system", "application", "database",
    "server", "client", "protocol", "security", "encryption", "data",
    "information", "technology", "development", "programming", "language",
    "interface", "terminal", "command", "execute", "compile", "debug",
    "python", "javascript", "learning", "student", "teacher", "education",
    "science", "mathematics", "engineering", "design", "creative", "problem",
    "solution", "analysis", "research", "project", "team", "collaboration"
};

// Structure to store typing statistics
typedef struct {
    int total_chars;        // Total characters in the reference text
    int typed_chars;        // Total characters typed by user
    int correct_chars;      // Correctly typed characters
    int incorrect_chars;    // Incorrectly typed characters
    double time_taken;      // Actual time taken in seconds
    double wpm;             // Words per minute
    double accuracy;        // Accuracy percentage
    int total_words;        // Total words in the reference text
} TypingStats;

/**
 * Function: generate_text_block
 * Purpose: Creates a random sequence of words from the word bank
 * Parameters: 
 *   - buffer: Character array to store the generated text
 *   - num_words: Number of words to generate
 * 
 * This function randomly selects words from our word bank and
 * concatenates them with spaces to create a typing challenge.
 * Each practice session will have a unique combination of words.
 */
void generate_text_block(char *buffer, int num_words) {
    int word_bank_size = sizeof(word_bank) / sizeof(word_bank[0]);
    buffer[0] = '\0';  // Start with empty string
    
    for (int i = 0; i < num_words; i++) {
        // Randomly select a word from the word bank
        // This ensures variety and prevents memorization
        int random_index = rand() % word_bank_size;
        strcat(buffer, word_bank[random_index]);
        
        // Add space after each word except the last one
        // This matches natural typing patterns
        if (i < num_words - 1) {
            strcat(buffer, " ");
        }
    }
}

/**
 * Function: calculate_statistics
 * Purpose: Analyzes the typed text against the reference text
 * Parameters:
 *   - reference: The original text that should be typed
 *   - typed: The actual text typed by the user
 *   - stats: Pointer to TypingStats structure to store results
 *   - time_taken: Time taken to type (in seconds)
 * 
 * This function performs a detailed character-by-character comparison
 * to determine accuracy. It also calculates typing speed using the
 * standard WPM formula that's used across the typing industry.
 */
void calculate_statistics(const char *reference, const char *typed, 
                         TypingStats *stats, double time_taken) {
    stats->total_chars = strlen(reference);
    stats->typed_chars = strlen(typed);
    stats->correct_chars = 0;
    stats->incorrect_chars = 0;
    stats->time_taken = time_taken;
    
    // Count the number of words in reference text
    // A word is any sequence of characters separated by spaces
    stats->total_words = 1;  // Start with 1 for the first word
    for (int i = 0; i < stats->total_chars; i++) {
        if (reference[i] == ' ') {
            stats->total_words++;
        }
    }
    
    // Compare character by character to count correct and incorrect entries
    // We compare up to the length of the shorter text to avoid array overruns
    int min_length = (stats->total_chars < stats->typed_chars) ? 
                     stats->total_chars : stats->typed_chars;
    
    for (int i = 0; i < min_length; i++) {
        if (reference[i] == typed[i]) {
            stats->correct_chars++;
        } else {
            stats->incorrect_chars++;
        }
    }
    
    // If the user typed more characters than required, count extras as errors
    // This discourages adding random characters to inflate the character count
    if (stats->typed_chars > stats->total_chars) {
        stats->incorrect_chars += (stats->typed_chars - stats->total_chars);
    }
    
    // If the user typed fewer characters, count missing ones as errors
    // This ensures that incomplete typing is reflected in the accuracy
    if (stats->typed_chars < stats->total_chars) {
        stats->incorrect_chars += (stats->total_chars - stats->typed_chars);
    }
    
    // Calculate accuracy: (correct characters / total reference characters) * 100
    // This gives us a percentage that represents how well the user matched the text
    if (stats->total_chars > 0) {
        stats->accuracy = (double)stats->correct_chars / stats->total_chars * 100.0;
    } else {
        stats->accuracy = 0.0;
    }
    
    // Calculate WPM (Words Per Minute)
    // Method 1: Using the standard 5-character-per-word convention
    // This is the most common method and allows comparison with other typing tests
    if (time_taken > 0) {
        double minutes = time_taken / 60.0;
        stats->wpm = (stats->typed_chars / 5.0) / minutes;
        
        // Alternative calculation using actual word count (commented out)
        // stats->wpm = stats->total_words / minutes;
    } else {
        stats->wpm = 0.0;
    }
}

/**
 * Function: display_statistics
 * Purpose: Shows detailed performance metrics to the user
 * Parameters:
 *   - stats: Pointer to TypingStats structure containing the results
 *   - block_number: The current block number being displayed
 * 
 * This function provides color-coded feedback to help users quickly
 * understand their performance level. The visual cues make it easier
 * to track improvement over multiple practice sessions.
 */
void display_statistics(const TypingStats *stats, int block_number) {
    printf("\n");
    printf(BOLD CYAN "========================================\n");
    printf("       BLOCK %d RESULTS\n", block_number);
    printf("========================================\n" RESET);
    
    printf("\n" BOLD "Time Taken:\n" RESET);
    // Convert seconds to a more readable format
    int minutes = (int)(stats->time_taken / 60);
    int seconds = (int)(stats->time_taken) % 60;
    if (minutes > 0) {
        printf("  %d minute(s) and %d second(s)\n", minutes, seconds);
    } else {
        printf("  %.1f seconds\n", stats->time_taken);
    }
    
    printf("\n" BOLD "Typing Speed:\n" RESET);
    // Provide context about what's considered a good WPM
    printf("  %.2f WPM (Words Per Minute)\n", stats->wpm);
    if (stats->wpm >= 80) {
        printf(GREEN "  Outstanding speed! You're typing like a professional.\n" RESET);
    } else if (stats->wpm >= 60) {
        printf(GREEN "  Excellent speed! Above average performance.\n" RESET);
    } else if (stats->wpm >= 40) {
        printf(YELLOW "  Good speed! This is around average.\n" RESET);
    } else if (stats->wpm >= 20) {
        printf(YELLOW "  Decent speed. Keep practicing to improve!\n" RESET);
    } else {
        printf(YELLOW "  You're building your foundation. Speed will come with practice!\n" RESET);
    }
    
    printf("\n" BOLD "Accuracy:\n" RESET);
    // Color code accuracy: Green for good, Yellow for okay, Red for needs improvement
    if (stats->accuracy >= 95) {
        printf(GREEN "  %.2f%% - Exceptional accuracy!\n" RESET, stats->accuracy);
    } else if (stats->accuracy >= 90) {
        printf(GREEN "  %.2f%% - Excellent work!\n" RESET, stats->accuracy);
    } else if (stats->accuracy >= 80) {
        printf(YELLOW "  %.2f%% - Good, focus a bit more on precision\n" RESET, stats->accuracy);
    } else if (stats->accuracy >= 70) {
        printf(YELLOW "  %.2f%% - Fair, but there's room for improvement\n" RESET, stats->accuracy);
    } else {
        printf(RED "  %.2f%% - Keep practicing! Slow down to improve accuracy\n" RESET, stats->accuracy);
    }
    
    printf("\n" BOLD "Detailed Statistics:\n" RESET);
    printf("  Words in block:           %d\n", stats->total_words);
    printf("  Total characters to type: %d\n", stats->total_chars);
    printf("  Characters you typed:     %d\n", stats->typed_chars);
    printf("  Correct characters:       " GREEN "%d" RESET "\n", stats->correct_chars);
    printf("  Incorrect characters:     " RED "%d" RESET "\n", stats->incorrect_chars);
    
    // Calculate and show characters per second as an additional metric
    if (stats->time_taken > 0) {
        double chars_per_second = stats->typed_chars / stats->time_taken;
        printf("  Characters per second:    %.2f\n", chars_per_second);
    }
    
    printf("\n" CYAN "========================================\n" RESET);
}

/**
 * Function: show_instructions
 * Purpose: Displays game instructions and controls to the user
 * 
 * Clear instructions help users understand what to expect and how
 * to get the most out of their practice sessions.
 */
void show_instructions() {
    printf("\n");
    printf(BOLD MAGENTA "╔════════════════════════════════════════════╗\n");
    printf("║  REAL-TIME TYPING TRAINER GAME             ║\n");
    printf("║  (Speed Measurement Mode)                  ║\n");
    printf("╚════════════════════════════════════════════╝\n" RESET);
    printf("\n");
    printf(BOLD "HOW TO PLAY:\n" RESET);
    printf("1. You will be shown a block of random words to type\n");
    printf("2. The timer starts automatically when the text appears\n");
    printf("3. Type the text as accurately and quickly as you can\n");
    printf("4. Press ENTER when you finish typing the block\n");
    printf("5. Your speed (WPM), accuracy, and time taken will be shown\n");
    printf("6. You can practice multiple blocks to track improvement\n");
    printf("\n");
    printf(BOLD "IMPORTANT TIPS:\n" RESET);
    printf("• " BOLD "Accuracy over speed:" RESET " It's better to type correctly than quickly\n");
    printf("• " BOLD "Don't rush:" RESET " There's no time limit - take the time you need\n");
    printf("• " BOLD "Use proper technique:" RESET " Position your fingers on the home row\n");
    printf("• " BOLD "Stay relaxed:" RESET " Tension slows you down and causes mistakes\n");
    printf("• " BOLD "Focus:" RESET " Try to type without looking at the keyboard\n");
    printf("\n");
    printf(BOLD "UNDERSTANDING YOUR RESULTS:\n" RESET);
    printf("• WPM measures how fast you type (higher is better)\n");
    printf("• Accuracy shows how correctly you typed (aim for 95%%+)\n");
    printf("• Both metrics together indicate your overall typing skill\n");
    printf("\n");
}

/**
 * Function: clear_screen
 * Purpose: Clears the terminal screen for better readability
 * 
 * This uses ANSI escape codes which work on most Unix/Linux terminals
 * and modern Windows terminals (Windows 10+). A clean screen helps
 * users focus on the current task without distraction.
 */
void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/**
 * Function: get_user_input_with_timing
 * Purpose: Displays the text block and measures how long it takes to type
 * Parameters:
 *   - reference_text: The text that should be typed
 *   - buffer: Where to store the user's input
 *   - max_length: Maximum characters to read
 * Returns: Time taken to type (in seconds)
 * 
 * This function is the heart of the program. It displays the text,
 * starts the timer, waits for user input, and then calculates the
 * elapsed time. The timing is precise to the second level.
 */
double get_user_input_with_timing(const char *reference_text, char *buffer, int max_length) {
    // Display the text to be typed in a nicely formatted box
    printf("\n" BOLD "Type the following text exactly as shown:\n" RESET);
    printf(CYAN "┌────────────────────────────────────────────────────────────────┐\n");
    
    // Format the text with proper word wrapping for readability
    // We'll display approximately 60 characters per line
    char *text_copy = strdup(reference_text);
    char *word = strtok(text_copy, " ");
    int line_length = 0;
    
    printf("│ " RESET);
    while (word != NULL) {
        int word_len = strlen(word);
        // If adding this word would exceed our line width, start a new line
        if (line_length + word_len + 1 > 60) {
            printf("\n" CYAN "│ " RESET);
            line_length = 0;
        }
        printf(BOLD "%s " RESET, word);
        line_length += word_len + 1;
        word = strtok(NULL, " ");
    }
    
    // Pad the last line to align the box border
    if (line_length > 0) {
        printf("\n");
    }
    
    printf(CYAN "└────────────────────────────────────────────────────────────────┘\n" RESET);
    free(text_copy);
    
    // Important: Start the timer AFTER displaying the text
    // This gives users a moment to read the text before they start typing
    printf("\n" BOLD YELLOW "⏱  Timer started! Begin typing now...\n" RESET);
    printf(GREEN "\nYour typing: " RESET);
    fflush(stdout);
    
    // Record the start time - this is when we consider typing to begin
    time_t start_time = time(NULL);
    
    // Wait for user input - the user can take as long as they need
    // The program will wait patiently until they press Enter
    if (fgets(buffer, max_length, stdin) != NULL) {
        // Remove newline character if present
        // fgets includes the newline, but we don't want it for comparison
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
    }
    
    // Record the end time - typing is complete
    time_t end_time = time(NULL);
    
    // Calculate elapsed time using difftime for precision
    // This handles any potential clock adjustments that might occur
    double time_taken = difftime(end_time, start_time);
    
    printf(YELLOW "⏱  Timer stopped! Calculating your results...\n" RESET);
    
    return time_taken;
}

/**
 * Function: main
 * Purpose: Main game loop that coordinates all typing blocks
 * 
 * The main function orchestrates the entire program flow. It handles
 * initialization, manages the practice session loop, and ensures
 * a smooth user experience from start to finish.
 */
int main() {
    // Seed the random number generator with current time
    // This ensures different word combinations each time the program runs
    srand(time(NULL));
    
    char reference_text[MAX_INPUT_LENGTH];
    char typed_text[MAX_INPUT_LENGTH];
    TypingStats stats;
    int block_number = 1;
    char continue_choice;
    
    // Display welcome message and instructions
    clear_screen();
    show_instructions();
    
    printf(BOLD "Press ENTER when you're ready to start your first typing block..." RESET);
    getchar();
    
    // Main game loop - continues until user chooses to quit
    // This allows for unlimited practice sessions in one sitting
    do {
        clear_screen();
        
        // Display block header with current block number
        printf(BOLD CYAN "\n╔════════════════════════════════════════════╗\n");
        printf("║  TYPING BLOCK %d                            ║\n", block_number);
        printf("╚════════════════════════════════════════════╝\n" RESET);
        
        // Generate a fresh block of words for this practice session
        // Using WORDS_PER_BLOCK ensures consistent challenge difficulty
        generate_text_block(reference_text, WORDS_PER_BLOCK);
        
        // Display text and get user input while measuring time
        // This is where the actual practice happens
        double time_taken = get_user_input_with_timing(reference_text, typed_text, MAX_INPUT_LENGTH);
        
        // Analyze performance and calculate all metrics
        calculate_statistics(reference_text, typed_text, &stats, time_taken);
        
        // Show detailed results with helpful feedback
        display_statistics(&stats, block_number);
        
        // Offer encouragement based on performance
        printf("\n" BOLD "Performance Notes:\n" RESET);
        
        // Provide personalized feedback to help user improve
        if (stats.accuracy >= 95 && stats.wpm >= 60) {
            printf(GREEN "  Excellent work! You have both speed and precision.\n" RESET);
        } else if (stats.accuracy >= 95) {
            printf(GREEN "  Great accuracy! As you practice, your speed will naturally increase.\n" RESET);
        } else if (stats.wpm >= 60) {
            printf(YELLOW "  Good speed! Try slowing down slightly to improve accuracy.\n" RESET);
        } else if (stats.accuracy < 80) {
            printf(YELLOW "  Focus on typing correctly rather than quickly. Speed follows accuracy!\n" RESET);
        } else {
            printf(YELLOW "  Keep practicing! You're building muscle memory with each session.\n" RESET);
        }
        
        // Ask if user wants to continue practicing
        printf("\n" BOLD "Would you like to practice another block? (y/n): " RESET);
        scanf(" %c", &continue_choice);
        getchar();  // Consume the newline character left by scanf
        
        block_number++;
        
    } while (continue_choice == 'y' || continue_choice == 'Y');
    
    // Display farewell message with session summary
    printf("\n" BOLD GREEN "╔════════════════════════════════════════════╗\n");
    printf("║  Thank you for practicing!                 ║\n");
    printf("╚════════════════════════════════════════════╝\n" RESET);
    printf("\nSession Summary:\n");
    printf("  Total blocks completed: %d\n", block_number - 1);
    printf("  Words per block: %d\n", WORDS_PER_BLOCK);
    printf("  Total words practiced: %d\n", (block_number - 1) * WORDS_PER_BLOCK);
    printf("\n" BOLD "Keep practicing daily to see continuous improvement!\n" RESET);
    printf("Remember: Consistency is more important than duration.\n\n");
    
    return 0;
}
