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
void init_welcome_remarks(Game* game);

void play_quiz(Game* game, int start_index, int *que_index, int *p_index, int *used_powerup);
int init_timer(Game* game);
void init_difficulty(Game* game);
int choose_category(Game* game);
void mark_category_used(int categoryIndex, Game* game);
void init_index(int *array, int size, int modulo, int multiple);
int is_duplicate(int *array, int size, int value);
int is_correctanswer(char user_answer, Quiz quiz);


void center_item(int size);
void add_spaces(int size, int max_size);
void clear_screen();
void press_continue();

void write_score(char *name, int score);
void read_score(Score **scores, int *numScores);
void print_top_scores(Score scores[], int numScores, int n);



extern char* separator;
extern char* title_screen;
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
extern char* about_screen;
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


extern int cat_flg1;
extern int cat_flg2;
extern int cat_flg3;
extern int cht1;
extern int cht2;
extern int dif1, dif2, dif3;
extern int opt[4];
extern bool used_easy_category[10];
extern bool used_average_category[10];
extern bool used_hard_category[10];

extern int used_powerup[3];