#pragma once

#include <stdbool.h>



// Define constants for the game
#define MAX_QUESTIONS 10
#define TOTAL_QUESTIONS 10

#define MAX_OPTIONS 4
#define TOTAL_OPTIONS 4

#define MAX_CATEGORIES 5
#define TOTAL_CATEGORIES 5

#define MAX_POWERUPS 3
#define TOTAL_POWERUPS 5

#define MAX_NAME_LENGTH 10

// Enumeration for difficulty levels
enum Difficulty {
    EASY,
    AVERAGE,
    DIFFICULT
};

// Structure to store player's score
typedef struct {
    char name[20];
    int score;
} Score;

// Structure to store a quiz question and its options
typedef struct {
    char* level;
    char* category;
    char* question;
    char* option[4];
    char* correct_answer[4];
} Quiz;

// Structure to store a power-up's details
typedef struct {
    char* name;
    char* description;
    char key;
} Powerup;

// Node structure for a singly linked list
struct myNode {
    char* value;
    struct myNode* next;
};
typedef struct myNode myNode;

// Singly linked list structure
struct SLList {
    myNode* head;
    myNode* tail;
    int size;
};
typedef struct SLList SLList;

// Function prototypes for singly linked list operations
myNode* get_node(SLList* list, int index);
myNode* create_node(char* x);

// Structure to store player's details
typedef struct {
    char* name;
    int score;
    int lives;
} Player;

// Structure to store the game's state and configurations
typedef struct {
    enum Difficulty difficulty;
    Player player;
    Quiz* quizzes;
    int num_quizzes;
    Powerup* powerups;
    int num_powerups;
    int used_powerup[5];
    int clicked_powerup[5];
    int timer;
} Game;

// Function prototypes for game initialization and operations
void init_list(SLList* list); // This initializes the singly linkedlist
char* get_item(SLList* list, int index); // This return the value based on the index
void set_item(SLList* list, int index, char* item); // This change the value in a given index
void insert_item(SLList* list, int index, char* item); // This add a value in the linkedlist
void remove_tem(SLList* list, int index); // This remove a value in a given index and make the value from the next index to move to the deleted index.
void clear_list(SLList* list); // This reset the linkedlists or reintialize it

// These functions initializes the quiz from a given array, so that if we add a new value, we don't need to change its size.
void init_contents(Game* game);
Quiz create_quiz(const char* level, const char* category, const char* question, const char* option[4], const char* correct_answer[4]);
Powerup create_powerup(const char* name, const char* description);
void add_quiz(Game* game, const Quiz* quiz);
void add_powerup(Game* game, const Powerup* powerUp);

// This initializes the player, and make the player be familiar with the game
void init_player(Game* game);
void confirm_name(Game* game);
void init_welcome_remarks(Game* game);


// This function initializes the question and answer part, this is the main program.
void play_quiz(Game* game, int start_index, int *que_index, int *p_index, int *used_powerup);
int init_timer(Game* game);
void init_difficulty(Game* game);
int choose_category(Game* game, SLList* list, int cat_index[MAX_CATEGORIES]);
void mark_category_used(int categoryIndex, Game* game);
void init_index(int *array, int size, int modulo, int multiple);
int is_duplicate(int *array, int size, int value);
int is_correct_answer(char user_answer, Quiz quiz);
void format_header1(char **buffer, int *size, const char *player_name, int score, int lives);
void format_header2(char **buffer, int *size, const char *level, const char *category, int time_left, int question_number);
void populate_list(SLList *list, int *index, Quiz *quizzes, int num_items, int current);

// This is only an adds-on to make the program look appealing
void center_text(char* text, int len);
void add_spaces(int size, int max_size);
void clear_screen();
void press_continue();

// This functions initializes the history of play made by the users
void write_score(char *name, int score);
void read_score(Score **scores, int *numScores);
void print_top_scores(Score scores[], int numScores);


// External variables for various screens and states
extern char* separator;
extern char* title_screen1;
extern char* title_screen2;

extern char* welcome_screen1;
extern char* welcome_screen2;
extern char* welcome_screen3;
extern char* welcome_screen4;
extern char* welcome_screen5;
extern char* welcome_screen6;
extern char* cat_easy_average;
extern char* cat_hard;
extern char* leaderboards;
extern char* lose_screen;
extern char* win_screen;
extern char* about_screen1;
extern char* about_screen2;
extern char* mechanics1;
extern char* mechanics2;
extern char* mechanics3;
extern char* show_category;
extern char* easy_remark1;
extern char* easy_remark2;
extern char* easy_remark3;
extern char* average_remark1;
extern char* average_remark2;
extern char* average_remark3;
extern char* difficult_remark1;
extern char* difficult_remark2;
extern char* difficult_remark3;
extern char* no_score_leaderboard;

extern int n1, n2, n3;
extern int cat_flg1;
extern int cat_flg2;
extern int cat_flg3;
extern int reset_flg;
extern int cht1;
extern int cht2;
extern int dif1, dif2, dif3;
extern int c_flg1;
extern int c_flg2;
extern int c_flg3;
extern int opt[4];
extern bool used_easy_category[10];
extern bool used_average_category[10];
extern bool used_hard_category[10];
extern int used_powerup[3];
