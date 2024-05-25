#pragma once

#include <stdbool.h>

enum Difficulty {
    EASY,
    AVERAGE,
    HARD
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

void init_player(Game* game);
void confirm_name(Game* game);



void play_quiz(Game* game, int start_index, int *que_index, int *p_index, int *used_powerup);
int init_timer(Game* game);
void init_difficulty(Game* game);
int choose_category(Game* game);
void init_index(int *array, int size, int modulo, int multiple);
int is_duplicate(int *array, int size, int value);
int is_correctanswer(char user_answer, Quiz quiz);

void center_item(int size);
void add_spaces(int size, int max_size);

void writeScore(char *name, int score);
void printTopScores(Score scores[], int numScores, int n); 
void readScore(Score **scores, int *numScores);
void sortScores(Score scores[], int numScores);



bool used_easy_category[10] = {false}; 
bool used_average_category[10] = {false};
bool used_hard_category[10] = {false};

int cat_flg1 = 0, cat_flg2 = 0, cat_flg3 = 0;
int opt[4] = {'A', 'B', 'C', 'D'};
int used_powerup[3] = {0};

