#pragma once

#include <stdbool.h>

#define MAX_QUESTIONS 10
#define TOTAL_QUESTIONS 10

#define MAX_OPTIONS 4
#define TOTAL_OPTIONS 4

#define MAX_CATEGORIES 5
#define TOTAL_CATEGORIES 5

#define MAX_POWERUPS 3
#define TOTAL_POWERUPS 5

#define MAX_NAME_LENGTH 10

enum Difficulty {
    EASY,
    AVERAGE,
    DIFFICULT
};

typedef struct {
    char name[20];
    int score;
    int numScores;
} Score;

typedef struct {
    char* level;
    char* category;
    char* question;
    char* option[4];
    char* correct_answer[4];
} Quiz;

typedef struct {
    char* name;
    char* description;
} Powerup;

typedef struct {
    char* name;
    int score;
    int lives;
} Player;

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


Quiz create_quiz(const char* level, const char* category, const char* question, const char* option[4], const char* correct_answer[4]);
Powerup create_powerup(const char* name, const char* description);

void init_contents(Game* game);
void add_quiz(Game* game, const Quiz* quiz);
void add_powerup(Game* game, const Powerup* powerUp);
void show_title_screen();
void show_welcome_screen(Game* game);
void show_lose_screen(Game* game, int j, int num_scrores);
void show_about_screen();

void init_player(Game* game);
void confirm_name(Game* game);

void play_quiz(Game* game, int start_index, int *que_index, int *p_index, int *used_powerup);
int init_timer(Game* game);
void init_difficulty(Game* game);
int choose_category(Game* game);
void markCategoryAsUsed(int categoryIndex, Game* game);
void init_index(int *array, int size, int modulo, int multiple);
int is_duplicate(int *array, int size, int value);
int is_correctanswer(char user_answer, Quiz quiz);

void center_item(int size);
void add_spaces(int size, int max_size);

void writeScore(char *name, int score);
void printTopScores(Score scores[], int numScores, int n); 
void readScore(Score **scores, int *numScores);
void sortScores(Score scores[], int numScores);

extern char* separator;
extern char* title_screen;
extern char* welcome_screen;
extern char* cat_easy_average;
extern char* cat_hard;
extern char* lose_screen;
extern char* about_screen;

extern int cat_flg1;
extern int cat_flg2;
extern int cat_flg3;
extern int cht1;
extern int cht2;
extern int opt[4];
extern bool used_easy_category[10];
extern bool used_average_category[10];
extern bool used_hard_category[10];

extern int used_powerup[3];