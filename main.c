#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_INPUT_LENGTH 1000
#define WORDS_PER_BLOCK 40

const char *word_bank[] = {
    "the", "quick", "brown", "fox", "jumps", "over", "lazy", "dog",
    "computer", "program", "typing", "speed", "accuracy", "practice"
};

typedef struct {
    int total_chars, typed_chars, correct_chars;
    double time_taken, wpm, accuracy;
} TypingStats;

void clear_screen() {
    printf("\033[2J\033[H");
}

void print_header(int block) {
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║        TYPING TRAINER - BLOCK %d     ║\n", block);
    printf("╚══════════════════════════════════════╝\n\n");
}

void generate_text_block(char *buffer, int num_words) {
    int size = sizeof(word_bank) / sizeof(word_bank[0]);
    buffer[0] = '\0';
    for (int i = 0; i < num_words; i++) {
        int idx = rand() % size;
        strcat(buffer, word_bank[idx]);
        if (i < num_words - 1) strcat(buffer, " ");
    }
}

void calculate_statistics(const char *ref, const char *typed, TypingStats *s, double t) {
    s->total_chars = strlen(ref);
    s->typed_chars = strlen(typed);
    s->correct_chars = 0;
    
    for (int i = 0; i < s->total_chars && i < s->typed_chars; i++) {
        if (ref[i] == typed[i]) s->correct_chars++;
    }
    
    s->time_taken = t;
    s->accuracy = (double)s->correct_chars / s->total_chars * 100;
    if (t > 0) s->wpm = (s->typed_chars / 5.0) / (t / 60.0);
}

double get_user_input_with_timing(const char *ref, char *buf, int max_len) {
    printf("╔══════════════════════════════════════╗\n");
    printf("║  TEXT TO TYPE:                      ║\n");
    printf("╠══════════════════════════════════════╣\n");
    printf("║  %s%-40s║\n", ref, "");
    printf("╚══════════════════════════════════════╝\n\n");
    
    printf("⏱️  Start typing now: ");
    time_t start = time(NULL);
    fgets(buf, max_len, stdin);
    buf[strcspn(buf, "\n")] = 0;
    return difftime(time(NULL), start);
}

int main() {
    srand(time(NULL));
    char ref[MAX_INPUT_LENGTH], typed[MAX_INPUT_LENGTH];
    TypingStats stats;
    int block = 1;
    char choice;
    
    clear_screen();
    printf("🚀 TYPING TRAINER - Press Enter to start...");
    getchar();
    
    do {
        clear_screen();
        print_header(block);
        
        generate_text_block(ref, WORDS_PER_BLOCK);
        double time = get_user_input_with_timing(ref, typed, MAX_INPUT_LENGTH);
        calculate_statistics(ref, typed, &stats, time);
        
        printf("\n╔══════════════════════════════════════╗\n");
        printf("║           RESULTS                    ║\n");
        printf("╠══════════════════════════════════════╣\n");
        printf("║  Speed:  %.1f WPM             %-10s║\n", stats.wpm, "");
        printf("║  Accuracy: %.1f%%             %-10s║\n", stats.accuracy, "");
        printf("║  Time: %.1f sec               %-10s║\n", stats.time_taken, "");
        printf("╚══════════════════════════════════════╝\n\n");
        
        printf("Next block? (y/n): ");
        scanf(" %c", &choice);
        block++;
    } while (choice == 'y' || choice == 'Y');
    
    printf("\n✅ Great practice session! Keep improving!\n");
    return 0;
}
