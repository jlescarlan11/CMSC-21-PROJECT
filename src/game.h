#pragma once

#include <stdbool.h>

#include "graphics.h"

#define MAX_QUESTIONS 10
#define TOTAL_QUESTIONS 10

#define MAX_OPTIONS 4
#define TOTAL_OPTIONS 4

#define MAX_CATEGORIES 5
#define TOTAL_CATEGORIES 5

#define MAX_POWERUPS 3
#define TOTAL_POWERUPS 5

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
int cht1 = 0, cht2 = 0;
int opt[4] = {'A', 'B', 'C', 'D'};
int used_powerup[3] = {0};

void writeScore(char *name, int score) {
    FILE *file = fopen("assets/scores.txt", "a");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "%s %d\n", name, score);

    fclose(file);
}

void printTopScores(Score scores[], int numScores, int n) {
    printf("\n");
    
    center_item(53);
    printf("\033[0;36m----TOP 3 SCORERS----------------------------------\033[0m\n");

    center_item(45);
    printf("RANK");
    add_spaces(strlen("RANK"), 10);
    printf("NAME");
    add_spaces(strlen("NAME"), 20);
    printf("SCORE\n");
    center_item(53);
    printf("\033[0;36m---------------------------------------------------\033[0m\n");

    center_item(45);
    printf("[1]");
    add_spaces(strlen("[1]"), 10);
    printf("%s", scores[0].name);
    add_spaces(strlen(scores[0].name), 20);
    printf("%d\n", scores[0].score);

    center_item(45);
    printf("[2]");
    add_spaces(strlen("[2]"), 10);
    printf("%s", scores[1].name);
    add_spaces(strlen(scores[1].name), 20);
    printf("%d\n", scores[1].score);

    center_item(45);
    printf("[3]");
    add_spaces(strlen("[3]"), 10);
    printf("%s", scores[2].name);
    add_spaces(strlen(scores[2].name), 20);
    printf("%d\n", scores[2].score);

    center_item(53);
    printf("\033[0;36m---------------------------------------------------\033[0m\n");
}


void readScore(Score **scores, int *numScores) {
    char names[100];
    int score;
    FILE *file = fopen("assets/scores.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    *numScores = 0; 

    while ((fscanf(file, "%s %d", names, &score) == 2)) {
        (*numScores)++;
    }

    *scores = (Score *)malloc((*numScores) * sizeof(Score));
    if (*scores == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return;
    }

    rewind(file);

    for (int i = 0; i < *numScores; i++) {
        if (fscanf(file, "%s %d", names, &score) == 2) {
            strcpy((*scores)[i].name, names);
            (*scores)[i].score = score;
        } else {
            printf("Error reading score from file.\n");
            free(*scores);
            *scores = NULL;
            *numScores = 0;
            fclose(file);
            return;
        }
    }

    fclose(file);
}

void sortScores(Score scores[], int numScores) {
    for (int i = 0; i < numScores - 1; i++) {
        for (int j = i + 1; j < numScores; j++) {
            if (scores[i].score < scores[j].score) {
                Score temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }
}

void init_contents(Game* game) {
    Quiz quiz_data[] = {
        {"EASY", "MATHEMATICS", "CHOOSE A FACTUAL OPTION", {"ALL RECTANGLES ARE SQUARES", "ALL SQUARES ARE RECTANGLES", "SOME RECTANGLES ARE SQUARES", "SOME SQUARES ARE RECTANGLES"}, {"B", "C", "\0", "\0"}},
        {"EASY", "MATHEMATICS", "WHAT IS THE ONLY PRIME NUMBER THAT DOES NOT CONTAIN THE LETTER E?", {"2", "56", "17", "5"}, {"A", "\0", "\0", "\0"}},
        {"EASY", "MATHEMATICS", "HOW MANY SHORT STRAIGHT LINES ARE NEEDED IN ORDER TO SPELL OUT 29 IN WORDS?", {"20", "24", "27", "29"}, {"D", "\0", "\0", "\0"}},
        {"EASY", "MATHEMATICS", "WHAT NUMBER COMES FIRST WHEN ALL THE NUMBERS ARE ARRANGED ALPHABETICALLY?", {"4", "5", "8", "9"}, {"C", "\0", "\0", "\0"}},
        {"EASY", "MATHEMATICS", "CHOOSE THE CORRECT OPTION.", {"SOME REAL NUMBERS ARE COMPLEX NUMBERS", "ALL REAL NUMBERS ARE COMPLEX NUMBERS", "SOME COMPLEX NUMBERS ARE REAL NUMBERS", "REAL NUMBERS ARE NOT COMPLEX NUMBERS"}, {"B", "C", "\0", "\0"}},
        {"EASY", "MATHEMATICS", "IS ZERO A WHOLE NUMBER", {"YES", "NO", "MAYBE", "IN SOME CASES, YES"}, {"A", "\0", "\0", "\0"}},
        {"EASY", "MATHEMATICS", "WHAT IS THE DERIVATIVE OF A CONSTANT?", {"ITSELF", "ZERO", "ONE", "UNDEFINED"}, {"B", "\0", "\0", "\0"}},
        {"EASY", "MATHEMATICS", "WHAT IS THE ONLY NUMBER THAT IS SPELLED IN DESCENDING ORDER (Z-A)?", {"3", "6", "10", "1"}, {"D", "\0", "\0", "\0"}},
        {"EASY", "MATHEMATICS", "WHAT IS THE LONGEST SIDE OF A TRIANGLE CALLED?", {"HYPOTENUSE", "BASE", "VERTICES", "HYPOTEMUSE"}, {"A", "\0", "\0", "\0"}},
        {"EASY", "MATHEMATICS", "WHICH OF THE FOLLOWING POSITIVE INTEGER IS BOTH A PERFECT SQUARE AND A PERFECT CUBE?", {"16", "125", "64", "81"}, {"C", "\0", "\0", "\0"}},
        
        {"EASY", "SCIENCE", "BRONZE, IS AN ALLOY MADE UP OF TWO ELEMENTS. WHAT ARE THE TWO ELEMENTS THAT IS COMMONLY USED IN THE FORMATION OF BRONZE?", {"COPPER AND ZINC", "ALUMINUM AND NICKEL", "COPPER AND TIN", "PHOSPHORUS AND TIN"}, {"C", "\0", "\0", "\0"}},
        {"EASY", "SCIENCE", "HOW MANY BONES DO SHARK HAVE?", {"0", "108", "10", "1"}, {"A", "\0", "\0", "\0"}},
        {"EASY", "SCIENCE", "RAINBOWS ARE FORMED DUE TO WHAT PROCESS OF LIGHT?", {"REFRACTION", "REFLECTION", "DISPERSION", "ABSORPTION"}, {"A", "\0", "\0", "\0"}},
        {"EASY", "SCIENCE", "WHAT IS THE BEST CONDUCTOR OF HEAT AND ELECTRICITY?", {"GOLD", "PLATINUM", "COPPER", "SILVER"}, {"D", "\0", "\0", "\0"}},
        {"EASY", "SCIENCE", "WHAT IS THE LIGHTEST METAL?", {"ALUMINUM", "MAGNESIUM", "CALCIUM", "LITHIUM"}, {"D", "\0", "\0", "\0"}},
        {"EASY", "SCIENCE", "WHAT IS THE LARGEST INTERNAL ORGAN OF THE HUMAN BODY?", {"LUNGS", "STOMACH", "LIVER", "LARGE INTESTINE"}, {"C", "\0", "\0", "\0"}},
        {"EASY", "SCIENCE", "WHAT KIND OF ENERGY DOES FOOD HAVE?", {"MECHANICAL ENERGY", "CHEMICAL ENERGY", "KINETIC ENERGY", "HEAT ENERGY"}, {"B", "\0", "\0", "\0"}},
        {"EASY", "SCIENCE", "WHAT IS THE ELECTRIC CHARGE OF A NEUTRON?", {"POSITIVE", "NEGATIVE", "NO CHARGE", "NONE OF THE ABOVE"}, {"C", "\0", "\0", "\0"}},
        {"EASY", "SCIENCE", "WHAT IS THE POWERHOUSE OF THE CELL?", {"MITHOCONDRIA", "MITOCHONDRION", "MITOCONDREA", "MITHOCONDREIA"}, {"B", "\0", "\0", "\0"}},
        {"EASY", "SCIENCE", "WHAT DOES A CONCHOLOGIST COLLECT?", {"CONCH SHELLS", "MOLLUSC SHELLS", "SEASHELLS", "CRAB SHELLS"}, {"B", "\0", "\0", "\0"}},
        
        {"EASY", "COMPUTER SCIENCE", "WHAT IS THE NAME OF THE FIRST COMPUTER?", {"ENIAC", "ENC", "ENAC", "CCSC"}, {"A", "\0", "\0", "\0"}},
        {"EASY", "COMPUTER SCIENCE", "WHAT DOES HTML STAND FOR?", {"HYPERTEXT MIX LANGUAGE", "HOGS TACKLING MIGHTY LIONS.", "HOW TO LOVE ME", "HYPERTEXT MARKUP LANGUAGE"}, {"D", "\0", "\0", "\0"}},
        {"EASY", "COMPUTER SCIENCE", "WHICH IS OLDER?", {"C", "C++", "FORTRAN", "PYTHON"}, {"C", "\0", "\0", "\0"}},
        {"EASY", "COMPUTER SCIENCE", "WHAT IS BIT SHORT FOR?", {"BYTES", "BINARY BYTES", "BINARY DIGIT", "BINARY BITES"}, {"C", "\0", "\0", "\0"}},
        {"EASY", "COMPUTER SCIENCE", "WHAT DOES THE BOOLEAN DATATYPE REPRESENT?", {"YES AND NO", "MAYBE AND SOMETIMES", "TRUE AND FALSE", "NOTHING"}, {"C", "\0", "\0", "\0"}},
        {"EASY", "COMPUTER SCIENCE", "WHICH OF THE FOLLOWING IS NOT A COMPUTER VIRUS?", {"ILOVEYOU", "WORM", "IMISSYOU", "MELISSA"}, {"C", "\0", "\0", "\0"}},
        {"EASY", "COMPUTER SCIENCE", "WHAT DOES AI STAND FOR?", {"ARTIFICIAL INTELLECT", "ARTIFICIAL INTELLIGENCE", "ARTIFICIAL INTENTION", "ART INVENTION"}, {"B", "\0", "\0", "\0"}},
        {"EASY", "COMPUTER SCIENCE", "WHICH OF THE FOLLOWING IS A TYPE OF MALWARE?", {"WIGGLES", "WORMIES", "HORSES", "TROJANS"}, {"D", "\0", "\0", "\0"}},
        {"EASY", "COMPUTER SCIENCE", "WHAT IS THE SMALLEST UNIT OF DIGITAL IMAGE?", {"BYTES", "BIT", "PIXEL", "ARRAY"}, {"C", "\0", "\0", "\0"}},
        {"EASY", "COMPUTER SCIENCE", "WHAT DOES GPU STAND FOR?", {"GENERAL PROCESSING UNIT", "GRAPHICS IMAGE UNIT", "GENERAL IMAGE UNIT", "GRAPHICS PROCESSING UNIT"}, {"D", "\0", "\0", "\0"}},
        
        {"EASY", "GEOGRAPHY", "WHAT IS THE CAPITAL CITY OF INDONESIA?", {"JAKARTA", "KUALA LUMPUR", "JAYAPURA", "YOGYAKARTA"}, {"A", "\0", "\0", "\0"}},
        {"EASY", "GEOGRAPHY", "WHAT IS THE CAPITAL OF SOUTH COTABATO?", {"GENERAL SANTOS", "KORONODAL", "KIDAPAWAN", "MARAWI"}, {"B", "\0", "\0", "\0"}},
        {"EASY", "GEOGRAPHY", "WHAT IS THE SMALLEST CONTINENT BY AREA?", {"ASIA", "AUSTRALIA AND OCEANIA", "EUROPE", "NORTH AMERICA"}, {"B", "\0", "\0", "\0"}},
        {"EASY", "GEOGRAPHY", "WHAT IS THE LARGEST OCEAN IN THE WORLD?", {"ATLANTIC", "INDIAN", "PACIFIC", "ARCTIC"}, {"C", "\0", "\0", "\0"}},
        {"EASY", "GEOGRAPHY", "WHERE IS THE DEEPEST PART OF THE WORLD LOCATED?", {"INDIAN OCEAN", "PACIFIC OCEAN", "ATLANTIC OCEAN", "ARCTIC OCEAN"}, {"B", "\0", "\0", "\0"}},
        {"EASY", "GEOGRAPHY", "WHAT IS THE TALLEST MOUNTAIN IN THE WORLD?", {"MT. EVEREST", "MT. KILIMANJARO", "MT. FUJI", "MT. PINATUBO"}, {"A", "\0", "\0", "\0"}},
        {"EASY", "GEOGRAPHY", "WHAT IS THE OLDEST CITY OF THE PHILIPPINES?", {"QUEZON CITY", "BAGUIO CITY", "CEBU CITY", "DAVAO CITY"}, {"C", "\0", "\0", "\0"}},
        {"EASY", "GEOGRAPHY", "WHAT IS THE HIGHEST PEAK IN THE PHILIPPINES?", {"OSMENA PEAK", "MT. PINATUBO", "MT. APO", "STARBUKS"}, {"C", "\0", "\0", "\0"}},
        {"EASY", "GEOGRAPHY", "WHICH OF THE FOLLOWING BORDERS CHINA?", {"MONGOLIA", "THAILAND", "MALAYSIA", "INDONESIA"}, {"A", "\0", "\0", "\0"}},
        {"EASY", "GEOGRAPHY", "WHAT IS THE LONGEST RIVER IN THE PHILIPPINES?", {"PASIG RIVER", "CAGAYAN RIVER", "PAMPANGA RIVER", "AGUSAN RIVER"}, {"B", "\0", "\0", "\0"}},      
        
        {"EASY", "dummy1", "WHAT IS THE LONGEST RIVER IN THE PHILIPPINES?", {"PASIG RIVER", "CAGAYAN RIVER", "PAMPANGA RIVER", "AGUSAN RIVER"}, {"B", "\0", "\0", "\0"}}, 
        {"EASY", "dummy1", "WHAT IS THE LONGEST RIVER IN THE PHILIPPINES?", {"PASIG RIVER", "CAGAYAN RIVER", "PAMPANGA RIVER", "AGUSAN RIVER"}, {"B", "\0", "\0", "\0"}}, 
        {"EASY", "dummy1", "WHAT IS THE LONGEST RIVER IN THE PHILIPPINES?", {"PASIG RIVER", "CAGAYAN RIVER", "PAMPANGA RIVER", "AGUSAN RIVER"}, {"B", "\0", "\0", "\0"}}, 
        {"EASY", "dummy1", "WHAT IS THE LONGEST RIVER IN THE PHILIPPINES?", {"PASIG RIVER", "CAGAYAN RIVER", "PAMPANGA RIVER", "AGUSAN RIVER"}, {"B", "\0", "\0", "\0"}}, 
        {"EASY", "dummy1", "WHAT IS THE LONGEST RIVER IN THE PHILIPPINES?", {"PASIG RIVER", "CAGAYAN RIVER", "PAMPANGA RIVER", "AGUSAN RIVER"}, {"B", "\0", "\0", "\0"}},      
        {"EASY", "dummy1", "WHAT IS THE LONGEST RIVER IN THE PHILIPPINES?", {"PASIG RIVER", "CAGAYAN RIVER", "PAMPANGA RIVER", "AGUSAN RIVER"}, {"B", "\0", "\0", "\0"}}, 
        {"EASY", "dummy1", "WHAT IS THE LONGEST RIVER IN THE PHILIPPINES?", {"PASIG RIVER", "CAGAYAN RIVER", "PAMPANGA RIVER", "AGUSAN RIVER"}, {"B", "\0", "\0", "\0"}}, 
        {"EASY", "dummy1", "WHAT IS THE LONGEST RIVER IN THE PHILIPPINES?", {"PASIG RIVER", "CAGAYAN RIVER", "PAMPANGA RIVER", "AGUSAN RIVER"}, {"B", "\0", "\0", "\0"}}, 
        {"EASY", "dummy1", "WHAT IS THE LONGEST RIVER IN THE PHILIPPINES?", {"PASIG RIVER", "CAGAYAN RIVER", "PAMPANGA RIVER", "AGUSAN RIVER"}, {"B", "\0", "\0", "\0"}}, 
        {"EASY", "dummy1", "WHAT IS THE LONGEST RIVER IN THE PHILIPPINES?", {"PASIG RIVER", "CAGAYAN RIVER", "PAMPANGA RIVER", "AGUSAN RIVER"}, {"B", "\0", "\0", "\0"}},
        
        {"AVERAGE", "MATHEMATICS", "A GOOGOL IS 10 TO THE 100TH POWER. HOW MANY ZEROES DOES IT HAVE?", {"100", "1000", "10000", "100000"}, {"A", "\0", "\0", "\0"}},
        {"AVERAGE", "MATHEMATICS", "6 IS THE SMALLEST PERFECT NUMBER, WHAT IS THE NEXT PERFECT NUMBER?", {"16", "24", "36", "28"}, {"D", "\0", "\0", "\0"}},
        {"AVERAGE", "MATHEMATICS", "WHAT BRANCH OF MATHEMATICS DEALS WITH THE STUDY OF CONTINUOUS CHANGE?", {"CALCULUS", "STATISTICS", "ANALYSIS", "ALGEBRA"}, {"A", "\0", "\0", "\0"}},
        {"AVERAGE", "MATHEMATICS", "WHICH OF THE FOLLOWING IS EQUIVALENT TO THE STATEMENT, [IF I WILL STUDY TODAY, THEN I WILL PASS THE EXAM.]", {"IF I STUDY TODAY, THEN I WILL NOT PASS THE EXAM.", "I WILL NOT STUDY TODAY AND I WILL NOT PASS THE EXAM.", "I WILL NOT STUDY TODAY OR I WILL PASS THE EXAM", "IF I WILL NOT STUDY TODAY, THEN I WILL PASS THE EXAM."}, {"C", "\0", "\0", "\0"}},
        {"AVERAGE", "MATHEMATICS", "A CIRCLE HAS AN AREA OF 144, WHAT IS THE RADIUS OF THE CIRCLE?", {"10", "13", "11", "12"}, {"D", "\0", "\0", "\0"}},
        {"AVERAGE", "MATHEMATICS", "WHAT IS THE ONLY NUMBER THAT IS TWICE THE SUM OF ITS DIGITS?", {"16", "17", "18", "19"}, {"C", "\0", "\0", "\0"}}, 
        {"AVERAGE", "MATHEMATICS", "THE NUMBER ZERO WAS ORIGINALLY CALLED WHAT?", {"ZERO", "NOTHING", "CIPHER", "WE DON'T KNOW EITHER"}, {"C", "\0", "\0", "\0"}},
        {"AVERAGE", "MATHEMATICS", "WHAT IS THE TERM FOR THE 1/100TH OF A SECOND?", {"FLASHY", "JIFFY", "NANOSECOND", "MILLISECOND"}, {"B", "\0", "\0", "\0"}},
        {"AVERAGE", "MATHEMATICS", "WHAT IS A NANOSECOND?", {"ONE MILLIONTH OF A SECOND.", "ONE BILLIONTH OF A SECOND", "ONE THOUSANDTH OF A SECOND", "ONE TRILLIONTH OF A SECOND"}, {"B", "\0", "\0", "\0"}},
        {"AVERAGE", "MATHEMATICS", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},

        {"AVERAGE", "SCIENCE", "DIABETES DEVELOPS AS THE RESULT OF A PROBLEM WITH WHICH SPECIFIC ORGAN IN THE BODY?", {"LIVER", "SPLEEN", "THYMUS", "PANCREAS"}, {"D", "\0", "\0", "\0"}},
        {"AVERAGE", "SCIENCE", "WHAT IS THE MEDICAL TERM FOR BAD BREATH?", {"HALITOSIS", "ODONTALGIA", "ODANTOLGIA", "HALLITASIS"}, {"A", "\0", "\0", "\0"}},
        {"AVERAGE", "SCIENCE", "WHICH OF THE FOLLOWING IS THE CLOSEST RELATIVES OF HIPPOS?", {"PIGS", "WHALES", "SEALS", "WARTHOGS"}, {"B", "\0", "\0", "\0"}},
        {"AVERAGE", "SCIENCE", "WHAT IS THE STUDY OF ROCKS CALLED?", {"GEOLOGY", "ROCKOLOGY", "IGNEOULOGY", "PETROLOGY"}, {"D", "\0", "\0", "\0"}},
        {"AVERAGE", "SCIENCE", "WHICH PLANET HAS LESS DENSITY THAN WATER? [A PLANET THAT CAN FLOAT ON WATER]", {"JUPITER", "URANUS", "SATURN", "NEPTUNE"}, {"C", "\0", "\0", "\0"}},
        {"AVERAGE", "SCIENCE", "WHAT IS THE STUDY OF CANCER CALLED?", {"CANCEROLOGY", "VIROLOGY", "ONCOLOGY", "OPTHALMOLOGY"}, {"C", "\0", "\0", "\0"}},
        {"AVERAGE", "SCIENCE", "WHAT IS A SCIENTIST WHO SPECIALIZES IN THE STUDY OF CELLS?", {"CELLOLOGIST", "BIOLOGIST", "CYCLOTOLOGIST", "CYTOLOGIST"}, {"D", "\0", "\0", "\0"}},
        {"AVERAGE", "SCIENCE", "WHAT IS THE GAP BETWEEN NEURONS CALLED?", {"AXON", "ANNEX", "SYNAPSE", "DENDRITES"}, {"C", "\0", "\0", "\0"}},
        {"AVERAGE", "SCIENCE", "WHAT ELEMENT IS DIAMOND MADE OF?", {"CARBON", "CALCIUM", "MANGANESE", "BORON"}, {"A", "\0", "\0", "\0"}},
        {"AVERAGE", "SCIENCE", "WHICH OF THE FOLLOWING IS A BY-PRODUCT OF PHOTOSYNTHESIS?", {"GLUCOSE", "CARBON DIOXIDE", "NITROGEN", "WATER"}, {"A", "\0", "\0", "\0"}},

        {"AVERAGE", "COMPUTER SCIENCE", "WHAT IS A COMPUTER PROGRAM THAT TRANSLATES COMPUTER CODE WRITTEN IN ONE PROGRAMMING LANGUAGE INTO ANOTHER LANGUAGE?", {"GCC", "GOOGLE TRANSLATE", "COMPILER", "IDE"}, {"C", "\0", "\0", "\0"}},
        {"AVERAGE", "COMPUTER SCIENCE", "WHICH OF THE FOLLOWING IS AN EXAMPLE OF IDE?", {"CHATGPT", "ECLIPSE", "SOLAR", "LUNAR"}, {"B", "\0", "\0", "\0"}},
        {"AVERAGE", "COMPUTER SCIENCE", "WHAT IS A METHOD OR PROCESS OF DEFINING A PROBLEM IN TERMS OF ITSELF?", {"RECURSION", "REPITION", "BOOLEAN", "CONDITIONAL"}, {"A", "\0", "\0", "\0"}},
        {"AVERAGE", "COMPUTER SCIENCE", "WHICH OF THE FOLLOWING IS TRUE IN C?", {"ALL STRINGS ARE ARRAYS BUT NOT ALL ARRAYS ARE STRINGS", "ALL STRINGS ARE ARRAYS BUT ARRAYS CAN NEVER BE STRINGS", "SOME STRINGS ARE ARRAYS AND SOME ARRAYS ARE STRINGS", "ALL ARRAYS ARE STRINGS"}, {"A", "\0", "\0", "\0"}},
        {"AVERAGE", "COMPUTER SCIENCE", "WHICH OF THE FOLLOWING IS NOT A CONCEPT IN COMPUTER SCIENCE?", {"MERGE SEARCH", "BINARY SEARCH", "LINEAR SEARCH", "JUMP SEARCH"}, {"A", "\0", "\0", "\0"}},
        {"AVERAGE", "COMPUTER SCIENCE", "HOW DO YOU DECLARE AN ARRAY OF INTEGERS?", {"INT ARRAY;", "INT ARRAY[10];", "INT ARRAY = {10};", "INT ARRAY = 10"}, {"B", "\0", "\0", "\0"}},
        {"AVERAGE", "COMPUTER SCIENCE", "WHAT IS THE BINARY REPRESENTATION OF THE DECIMAL NUMBER 10?", {"1111", "1001", "1011", "1010"}, {"D", "\0", "\0", "\0"}},
        {"AVERAGE", "COMPUTER SCIENCE", "WHAT DOES THE TERM ALGORITHM REFER TO IN COMPUTER SCIENCE?", {"A COMPUTER NETWORK", "A STEP-BY-STEP PROCEDURE FOR PROBLEM SOLVING", "AN INSTRUCTION TO BE FOLLOWED", "A FLOW CHART"}, {"B", "\0", "\0", "\0"}},
        {"AVERAGE", "COMPUTER SCIENCE", "WHAT IS AN IP ADDRESS?", {"A UNIQUE IDENTIFIER FOR A COMPUTER ON A NETWORK", "A PROGRAMMING LANGUAGE USED FOR INTERNET PROTOCOLS", "AN EMAIL SERVER CONFIGURATION", "A SECRET PLACE"}, {"A", "\0", "\0", "\0"}},
        {"AVERAGE", "COMPUTER SCIENCE", "WHICH OF THE FOLLOWING IS A PRIMARY FUNCTION OF AN OPERATING SYSTEM?", {"COMPILING CODE", "MANAGING HARDWARE RESOURCES", "WRITING SOFTWARE APPLICATIONS", "OPERATION OF THE SYSTEM"}, {"B", "\0", "\0", "\0"}},

        {"AVERAGE", "GEOGRAPHY", "WHAT IS THE SECOND SMALLEST COUNTRY IN THE WORLD?", {"MONACO", "BARBADOS", "NAURU", "MARSHALL ISLANDS"}, {"A", "\0", "\0", "\0"}},
        {"AVERAGE", "GEOGRAPHY", "WHAT IS THE PROVINCIAL CAPITAL OF BATANES?", {"ITBAYAT", "BASCO", "BATAN", "ILAGAN"}, {"B", "\0", "\0", "\0"}},
        {"AVERAGE", "GEOGRAPHY", "DOES SPAIN BORDER THE UK?", {"YES", "MAYBE", "NO", "I DON'T CARE"}, {"A", "\0", "\0", "\0"}},
        {"AVERAGE", "GEOGRAPHY", "IN WHAT LAYER OF THE ATMOSPHERE DOES ALMOST ALL WEATHER OCCUR?", {"EXOSPHERE", "STRATOSPHERE", "TROPOSPHERE", "MESOSPHERE"}, {"C", "\0", "\0", "\0"}},
        {"AVERAGE", "GEOGRAPHY", "WHERE IS THE OZONE LAYER LOCATED?", {"STRATOSPHERE", "TROPOSPHERE", "MESOSPHERE", "THERMOSPHERE"}, {"A", "\0", "\0", "\0"}},
        {"AVERAGE", "GEOGRAPHY", "WHAT IS KNOWN AS THE SALAD BOWL OF THE PHILIPPINES?", {"IFUGAO", "BATANES", "LA UNION", "BENGUET"}, {"D", "\0", "\0", "\0"}},
        {"AVERAGE", "GEOGRAPHY", "WHICH OF THE FOLLOWING IS KNOWN AS THE LAND OF FIRE AND ICE?", {"IRELAND", "ICELAND", "DENMARK", "FINLAND"}, {"B", "\0", "\0", "\0"}},
        {"AVERAGE", "GEOGRAPHY", "WHAT IS KNOWN AS THE ISLAND THAT NEVER SLEEPS?", {"CEBU", "AKLAN", "CAMIGUIN", "SIARGAO"}, {"B", "\0", "\0", "\0"}},
        {"AVERAGE", "GEOGRAPHY", "WHICH OF THE FOLLOWING CITIES IS THE MOST POPOLOUS?", {"SHANGHAI", "BEIJING", "TOKYO", "DELHI"}, {"C", "\0", "\0", "\0"}},
        {"AVERAGE", "GEOGRAPHY", "WHICH OF THE FOLLOWING CONTINENTS HAS THE MOST COUNTRIES?", {"ASIA", "AFRICA", "EUROPE", "SOUTH AMERICA"}, {"B", "\0", "\0", "\0"}},

        {"AVERAGE", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"AVERAGE", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"AVERAGE", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"AVERAGE", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"AVERAGE", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"AVERAGE", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"AVERAGE", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"AVERAGE", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"AVERAGE", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"AVERAGE", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},

        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},

        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},

        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "dummy4", "HOW MANY SIDES DOES A MEGAGON HAVE?", {"MANY SIDES", "1000 SIDES", "1000000 SIDES", "100000 SIDES"}, {"C", "\0", "\0", "\0"}},
        
       
    };

    int num_quizzes = sizeof(quiz_data) / sizeof(quiz_data[0]);

    game->quizzes = NULL;
    game->num_quizzes = 0;

    for (int i = 0; i < num_quizzes; i++) {
        Quiz new_quiz = create_quiz(quiz_data[i].level, 
            quiz_data[i].category, 
            quiz_data[i].question, 
            (const char*[]) { quiz_data[i].option[0], quiz_data[i].option[1], quiz_data[i].option[2], quiz_data[i].option[3] },
            (const char*[]) { quiz_data[i].correct_answer[0], quiz_data[i].correct_answer[1], quiz_data[i].correct_answer[2], quiz_data[i].correct_answer[3] }
        );
        add_quiz(game, &new_quiz);
    }

    Powerup powerup_data[] = {
        {"TIME FREEZE", "THE TIMER IS FROZEN TO ALLOW PLAYERS TO ANSWER QUESTION"},
        {"ERASER", "ELIMINATES ONE WRONG OPTION"},
        {"DOUBLE JEOPARDY", "WILL GET A DOUBLE SCORE IF THE PLAYER CHOOSES THE CORRECT ANSWER, AND WILL LOSE ALL SCORE IF THE PLAYER CHOOSES THE WRONG ANSWER"},
        {"PASS", "A PLAYER CAN ATTEMPT THE SAME QUESTION LATER FOR THE LAST"},
        {"IMMUNITY", "A PLAYER CAN ATTEMPT THE SAME QUESTION TWICE IN CASE THEY ANSWERED IT INCORRECTLY THE FIRST TIME"}
    };

    int num_powerups = sizeof(powerup_data) / sizeof(powerup_data[0]);

    game->powerups = NULL;
    game->num_powerups = 0;

    for (int i = 0; i < num_powerups; i++) {
        Powerup new_powerup = create_powerup(powerup_data[i].name, powerup_data[i].description);
        add_powerup(game, &new_powerup);
    }
}

Quiz create_quiz(const char* level, const char* category, const char* question, const char* option[4], const char* correct_answer[4]) {
    Quiz quiz;
    quiz.level = strdup(level);
    quiz.category = strdup(category);
    quiz.question = strdup(question);
    for (int i = 0; i < 4; i++) {
        quiz.option[i] = strdup(option[i]);
        quiz.correct_answer[i] = strdup(correct_answer[i]);
    }
    return quiz;
}

void add_quiz(Game* game, const Quiz* quiz) {
    game->quizzes = realloc(game->quizzes, (game->num_quizzes + 1) * sizeof(Quiz));
    game->quizzes[game->num_quizzes] = *quiz;
    game->num_quizzes++;
}

Powerup create_powerup(const char* name, const char* description) {
    Powerup powerup;
    powerup.name = strdup(name);
    powerup.description = strdup(description);
    return powerup;
}

void add_powerup(Game* game, const Powerup* powerUp) {
    game->powerups = realloc(game->powerups, (game->num_powerups + 1) * sizeof(Powerup));
    game->powerups[game->num_powerups] = *powerUp;
    game->num_powerups++;
}

void init_player(Game* game) {
    system("cls");

    printf("\n");
    center_item(41);
    printf("BEFORE WE START, PLEASE ENTER YOUR NAME\n");
    center_item(23);
    printf("PRESS [ENTER] IF DONE\n");
    printf("\n");
    center_item(13);
    printf("-----------\n");
    center_item(14);
    printf("|\e[0;36m ");


    game->player.name = malloc(11 * sizeof(char));
    scanf("%10s", game->player.name);
    printf("\033[0m");

    for (int i = 0; game->player.name[i]; i++) {
        game->player.name[i] = toupper(game->player.name[i]);
    }

    game->player.score = 0;
    game->player.lives = 3;

    for (int i = 0; i < 5; i++) {
        game->clicked_powerup[i] = 0;
        game->used_powerup[i] = 0;
    }

}

void confirm_name(Game* game) {
    system("cls");
    printf("\n");
    center_item(54 + strlen(game->player.name));
    printf("IS [\033[0;36m%s\033[0m] THE NAME YOU'D LIKE TO USE THROUGHOUT THE GAME?\n\n", game->player.name);
    center_item(18);
    printf("(1) YES    (2) NO\n");
}

void center_item(int size) {
    for(int j = size/2; j < 55; j++) {
        printf(" ");
    }
}

void add_spaces(int size, int max_size) {
    for (int j = size; j < max_size; j++) {
        printf(" ");
    }
    return;
}

void init_difficulty(Game* game) { 
    if (game->player.score < 1000) {
        game->difficulty = EASY;
    } else if (game->player.score >= 1000 && game->player.score < 3500) {
       game->difficulty = AVERAGE;
    } else if (game->player.score >= 5000) {
       game->difficulty = DIFFICULT;
    }
}

int choose_category(Game* game) {
    int cat_index[MAX_CATEGORIES];

    init_index(cat_index, MAX_CATEGORIES, TOTAL_CATEGORIES, 10);

    for(int i = 0; i < MAX_CATEGORIES; i++) {
        if (game->difficulty == EASY) {
            cat_index[i] += 0; 
        } else if (game->difficulty == AVERAGE) {
            cat_index[i] += 50; 
        } else if (game->difficulty == DIFFICULT) {
            cat_index[i] += 100;
        }
    }

    system("cls");

    if (game->difficulty != DIFFICULT) {
        printf(cat_easy_average, game->difficulty == EASY ? 1000 : 5000, game->difficulty == 0 ? "EASY" : "AVERAGE");
    } else {
        printf(cat_hard);
    }

    char input = '0';
    do {
        input = getch();
    } while (input != '1');

    system("cls");

    printf("\n");
    center_item(26);
    printf("PLEASE CHOOSE A CATEGORY\n");

    for (int i = 0; i < 3; i++) {
        if ((game->difficulty == EASY) && (used_easy_category[cat_index[i]]) && (cat_flg1 < 2)) {
            if (cat_flg1 == 0) {
                cat_index[i] = cat_index[3];
            } else if (cat_flg1 == 1) {
                cat_index[i] = cat_index[4];
            }
            cat_flg1++;
        } else if ((game->difficulty == AVERAGE) && (used_average_category[cat_index[i]]) && (cat_flg1 < 2)) {
            if (cat_flg2 == 0) {
                cat_index[i] = cat_index[3];
            } else if (cat_flg2 == 1) {
                cat_index[i] = cat_index[4];
            }
            cat_flg2++;
        } else if ((game->difficulty == DIFFICULT) && (used_hard_category[cat_index[i]]) && (cat_flg1 < 2)) {
            if (cat_flg3 == 0) {
                cat_index[i] = cat_index[3];
            } else if (cat_flg3 == 1) {
                cat_index[i] = cat_index[4];
            }
            cat_flg2++;
        }
    }

    printf("\n\n");
    for (int i = 0; i < 3; i++) {
        if (game->difficulty == EASY) {
            if (!(used_easy_category[cat_index[i]])) {
                center_item(56);
                printf("\033[0;36m------------------------------------------------------\033[0m\n");
                printf("\t\t\t   (%d)\n", i + 1);
                int size = strlen(game->quizzes[cat_index[i]].category);
                center_item(size);
                printf("%s\n", game->quizzes[cat_index[i]].category);
            }
        } else if (game->difficulty == AVERAGE) {
            if (!(used_average_category[cat_index[i]])) {
                center_item(56);
                printf("\033[0;36m------------------------------------------------------\033[0m\n");
                printf("\t\t\t   (%d)\n", i + 1);
                int size = strlen(game->quizzes[cat_index[i]].category);
                center_item(size);
                printf("%s\n", game->quizzes[cat_index[i]].category);
            }
        } else if (game->difficulty == DIFFICULT) {
            if (!(used_hard_category[cat_index[i]])) {
                center_item(56);
                printf("\033[0;36m------------------------------------------------------\033[0m\n");
                printf("\t\t\t   (%d)\n", i + 1);
                int size = strlen(game->quizzes[cat_index[i]].category);
                center_item(size);
                printf("%s\n", game->quizzes[cat_index[i]].category);
            }
        }
    }
    center_item(56);
    printf("\033[0;36m------------------------------------------------------\033[0m\n");

    input = '0'; 
    do {
        input = getch();
    } while ((input < '1') && (input > '3'));

    return cat_index[input - '1'];
}

void init_index(int *array, int size, int modulo, int multiple) {

    int used[modulo]; 
    int count = 0; 

    for (int i = 0; i < size; i++) {
        int randomIndex;
        do {
            randomIndex = (rand() % modulo) * multiple; 
        } while (is_duplicate(used, count, randomIndex)); 

        array[i] = randomIndex; 
        used[count++] = randomIndex; 
    }
}

int is_duplicate(int *array, int size, int value) {
    for (int i = 0; i < size; i++) {
        if (array[i] == value) {
            return 1; 
        }
    }
    return 0; 
}

void play_quiz(Game* game, int start_index, int *que_index, int *pow_index, int *used_powerup) {
    for (int i = 0; i < 10; i++) {
        int del_index = rand() % 4;

        int opt_index[MAX_OPTIONS];
        init_index(opt_index, MAX_OPTIONS, TOTAL_OPTIONS, 1);

        int time_left = init_timer(game), answer_chosen = 0; 
        int ctr = 0;
        while (time_left && !answer_chosen) {
            if (ctr % 30 == 0) {
                PlaySound(TEXT("assets/play.wav"), NULL, SND_FILENAME | SND_ASYNC);
            }
            ctr++;
            if(!(game->clicked_powerup[3]) && (game->used_powerup[3])) {
                int temp = que_index[i];
                que_index[i] = que_index[9];
                que_index[9] = temp;

                game->clicked_powerup[3] = 1;
            }

            system("cls");
            puts(separator);
            printf("    PLAYER NAME: %s", game->player.name);
            add_spaces(17 + strlen(game->player.name), 45);

            printf("      SCORE : %d", game->player.score);
            int size = 0;
            for(int j = game->player.score; j > 0 ; j/=10) {
                size++;
            }
            add_spaces(size + 15, 40);

            printf("     LIVES : ");
            for (int j = 0; j < game->player.lives; j++) {
                printf("<3 ");
            }
            printf("\n");

            printf("    %s : %s", game->quizzes[que_index[i] + start_index].level, game->quizzes[que_index[i] + start_index].category);
            add_spaces(7 + strlen(game->quizzes[que_index[i] + start_index].level) + strlen(game->quizzes[que_index[i] + start_index].category), 45);

            printf("      TIMER: %02d", time_left);
            add_spaces(15, 40);

            printf("    QUESTION : %02d\n", i+1);

            puts(separator);

            printf("\n\n");
            center_item(strlen(game->quizzes[que_index[i] + start_index].question));
            printf("%s\n", game->quizzes[que_index[i] + start_index].question);

            printf("\n\n");

            if(!(game->clicked_powerup[1]) && (game->used_powerup[1])) {
                if (cht1) {
                    for(int j = 0; j < 4; j++) {
                        while(del_index == ((game->quizzes[que_index[i] + start_index].correct_answer[0][0]) - 'A')) {
                            del_index = rand() % 4;
                        } 
                        if(opt_index[j] != del_index) {
                            if(opt_index[j] == ((game->quizzes[que_index[i] + start_index].correct_answer[0][0]) - 'A')) {
                                center_item(56);
                                printf("\033[0;36m------------------------------------------------------\033[0m\n");
                                printf("\t\t\t   \033[0;32m(%c)\n", 'A' + j);
                                center_item(strlen(game->quizzes[que_index[i] + start_index].option[opt_index[j]]));
                                printf("%s\033[0m\n", game->quizzes[que_index[i] + start_index].option[opt_index[j]]);
                            } else {
                                center_item(56);
                                printf("\033[0;36m------------------------------------------------------\033[0m\n");
                                printf("\t\t\t   \033[0;31m(%c)\n", 'A' + j);
                                center_item(strlen(game->quizzes[que_index[i] + start_index].option[opt_index[j]]));
                                printf("%s\033[0m\n", game->quizzes[que_index[i] + start_index].option[opt_index[j]]);
                            }
                        }
                    }

                } else {
                    for(int j = 0; j < 4; j++) {
                        while(del_index == ((game->quizzes[que_index[i] + start_index].correct_answer[0][0]) - 'A')) {
                            del_index = rand() % 4;
                        } 
                        if(opt_index[j] != del_index) {
                            center_item(56);
                            printf("\033[0;36m------------------------------------------------------\033[0m\n");
                            printf("\t\t\t   (%c)\n", 'A' + j);
                            center_item(strlen(game->quizzes[que_index[i] + start_index].option[opt_index[j]]));
                            printf("%s\n", game->quizzes[que_index[i] + start_index].option[opt_index[j]]);
                        }
                    }
                }
            } else {
                if (cht1) {
                    for(int j = 0; j < 4; j++) {
                        if(opt_index[j] == ((game->quizzes[que_index[i] + start_index].correct_answer[0][0]) - 'A')) {
                            center_item(56);
                            printf("\033[0;36m------------------------------------------------------\033[0m\n");
                            printf("\t\t\t   \033[0;32m(%c)\n", 'A' + j);
                            center_item(strlen(game->quizzes[que_index[i] + start_index].option[opt_index[j]]));
                            printf("%s\033[0m\n", game->quizzes[que_index[i] + start_index].option[opt_index[j]]);
                        } else {
                            center_item(56);
                            printf("\033[0;36m------------------------------------------------------\033[0m\n");
                            printf("\t\t\t   \033[0;31m(%c)\n", 'A' + j);
                            center_item(strlen(game->quizzes[que_index[i] + start_index].option[opt_index[j]]));
                            printf("%s\033[0m\n", game->quizzes[que_index[i] + start_index].option[opt_index[j]]);
                        }
                    }
                } else {
                    for(int j = 0; j < 4; j++) {
                        center_item(56);
                        printf("\033[0;36m------------------------------------------------------\033[0m\n");
                        printf("\t\t\t   (%c)\n", 'A' + j);
                        center_item(strlen(game->quizzes[que_index[i] + start_index].option[opt_index[j]]));
                        printf("%s\n", game->quizzes[que_index[i] + start_index].option[opt_index[j]]);
                    }
                }
            }
            center_item(56);
            printf("\033[0;36m------------------------------------------------------\033[0m\n\n");
            puts(separator);

            size = 0;
            for (int j = 0; j < 3; j++) {
                if(!(game->used_powerup[pow_index[j]])) {
                    size += strlen(game->powerups[pow_index[j]].name) + 8;
                } 
            }
            center_item(size);
            for (int j = 0; j < 3; j++) {
                if(!(game->used_powerup[pow_index[j]])) {
                    printf("(%d) %s    ", j + 1, game->powerups[pow_index[j]].name);
                } 
            }
            printf("\n");
            puts(separator);

            if (kbhit()) {
                char user_answer = toupper(getch());

                if ((user_answer >= 'A') && (user_answer <= 'D')) {
                    char answer;

                    if (user_answer == 'A') {
                        answer = opt[opt_index[0]];
                    } else if (user_answer == 'B') {
                        answer = opt[opt_index[1]];
                    } else if (user_answer == 'C') {
                        answer = opt[opt_index[2]];
                    } else if (user_answer == 'D') {
                        answer = opt[opt_index[3]];
                    }

                    if(is_correctanswer(answer, game->quizzes[que_index[i] + start_index])) {
                        PlaySound(TEXT("assets/correct.wav"), NULL, SND_FILENAME | SND_ASYNC);
                        system("COLOR A0");

                        if(!(game->clicked_powerup[2]) && (game->used_powerup[2])) {
                            game->player.score *= 2;
                        } else {
                            if(game->difficulty == EASY) {
                                game->player.score += (3 * time_left);
                            } else if (game->difficulty == AVERAGE) {
                                game->player.score += (5 * time_left);
                            } else if (game->difficulty == DIFFICULT) {
                                game->player.score += (7 * time_left);
                            }
                            if (!(game->clicked_powerup[4] && (game->used_powerup[4]))) {
                                game->clicked_powerup[4] = 1;
                            }
                        }
                        answer_chosen = 1;
                    } else {
                        system("COLOR 40");
                        PlaySound(TEXT("assets/wrong.wav"), NULL, SND_FILENAME | SND_ASYNC);

                        if (!(game->clicked_powerup[2]) && (game->used_powerup[2])) {
                            game->player.score *= 0;
                        }

                        if (!(game->clicked_powerup[4]) && (game->used_powerup[4])) {
                            game->clicked_powerup[4] = 1;
                        } else {
                            game->player.lives--;
                            answer_chosen = 1;
                        }
                    }
                } else if ((user_answer >= '1') && (user_answer <= '3')) {
                    if (user_answer == '1') {
                        game->used_powerup[pow_index[0]] = 1;  
                    } else if (user_answer == '2') {
                        game->used_powerup[pow_index[1]] = 1;  
                    } else if (user_answer == '3') {
                        game->used_powerup[pow_index[2]] = 1;  
                    }
                } else if (user_answer == 1) {
                    PlaySound(NULL, 0, 0);
                    system("cls");
                    printf("Enter cheat code: ");
                    char cheat[20];
                    scanf("%19s", cheat);

                    for (int i = 0; cheat[i] != '\0'; i++) {
                        cheat[i] = toupper((unsigned char)cheat[i]);
                    }
                    
                    if (strcmp(cheat, "CLAIRVOYANCE") == 0) {
                        cht1 = 1;
                        printf("CLAIRVOYANCE ACTIVATED. CLICK ANY KEY TO CONTINUE.");
                        getch();
                    } else if (strcmp(cheat, "TIMEPAUSE") == 0) {
                        cht2 = 1;
                        printf("TIMEPAUSE ACTIVATED. CLICK ANY KEY TO CONTINUE");
                        getch();
                    } else if (strcmp(cheat, "DEACTIVATE") == 0) {
                        cht1 = 0, cht2 = 0;
                        printf("Cheats deactivated. Click any key to continue.");
                        getch();
                    }
                    PlaySound(TEXT("assets/play.wav"), NULL, SND_FILENAME | SND_ASYNC);
                    continue;                     
                }

            }

            Sleep(1000);

            if ((!(game->clicked_powerup[0]) && (game->used_powerup[0])) || cht2) {
                time_left;
            } else {
                time_left--;
            }

            system("COLOR 07");

            if (time_left == 0) {
                game->player.lives--;
            }

            if (game->player.lives == 0) {
                return;
            }
        }

        if (!(game->clicked_powerup[0]) && (game->used_powerup[0])) {
            game->clicked_powerup[0] = 1;
        } 
        if (!(game->clicked_powerup[1]) && (game->used_powerup[1])) {
            game->clicked_powerup[1] = 1;
        } 
        if (!(game->clicked_powerup[2]) && (game->used_powerup[2])) {
            game->clicked_powerup[2] = 1;
        }
    }
}

int is_correctanswer(char user_answer, Quiz quiz) {
    for(int i = 0; i < 4; i++) {
        if(user_answer == quiz.correct_answer[i][0]) {
            return 1;
        } else {
            break;
        }
        
    }
    return 0;
}

int init_timer(Game* game) {
    return game->difficulty == EASY ? 30 : (game->difficulty == AVERAGE ? 45 : 60);
}

