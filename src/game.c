
#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <windows.h>

#include "game.h"

int cat_flg1 = 0, cat_flg2 = 0, cat_flg3 = 0;
int n1 = 0, n2 = 0, n3 = 0;
int cht1 = 0, cht2 = 0;
int opt[4] = {'A', 'B', 'C', 'D'};
int dif1 = 0, dif2 = 0, dif3 = 0;
int c_flg1 = 0, c_flg2 = 0, c_flg3 = 0;
int reset_flg = 0;

int used_powerup[3] = {0};

bool used_easy_category[10] = {false};
bool used_average_category[10] = {false};
bool used_hard_category[10] = {false};


void write_score(char *name, int score) {
    FILE *file = fopen("assets/scores.txt", "a");
    fprintf(file, "%s %d\n", name, score);
    fclose(file);
}

void print_top_scores(Score scores[], int numScores) {
    puts(separator);
    printf("\n\n");
    puts(leaderboards);
    printf("\n");
    if (numScores == 1) {
        center_text("----TOP 1 SCORER----------------------------------", 110);
    } else if (numScores < 5) {
        char text[50];
        sprintf(text, "----TOP %d SCORERS----------------------------------",numScores);
        center_text(text, 110);

    } else {
        center_text("----TOP 5 SCORERS----------------------------------", 110);
    }

    char rankings[100];
    sprintf(rankings, "RANK%*sNAME%*sSCORE%*s", 6, "", 16, "", 10, "");
    center_text(rankings, 110);
    

    
    center_text("---------------------------------------------------", 110);

    
    for (int i = 0; (numScores > 5) ? (i < 5) : (i < numScores); i++) {
        char rankings[100];
        sprintf(rankings, "[%d]%*s%s%*s%05d%*s", i + 1, 7, "", scores[i].name, 20 - strlen(scores[i].name), "",scores[i].score, 11, "");
        center_text(rankings, 110);
    }

    center_text("---------------------------------------------------", 110);
    printf("\n");
    puts(separator);
    center_text("PRESS 1 TO GO BACK",110);
    puts(separator);
}


void read_score(Score **scores, int *numScores) {
    char names[100];
    int score;
    FILE *file = fopen("assets/scores.txt", "r");

    *numScores = 0; 

    if (file == NULL) {
        return;
    }

    while ((fscanf(file, "%s %d", names, &score) == 2)) {
        (*numScores)++;
    }

    *scores = (Score *)malloc((*numScores) * sizeof(Score));

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

    for (int i = 0; i < *numScores - 1; i++) {
        for (int j = i + 1; j < *numScores; j++) {
            if ((*scores)[i].score < (*scores)[j].score) {
                Score temp = (*scores)[i];
                (*scores)[i] = (*scores)[j];
                (*scores)[j] = temp;
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

        {"EASY", "HISTORY AND LITERATURE", "WHO IS THE FIRST PRESIDENT OF THE PHILIPPINES?", {"JOSE P. LAUREL", "MANUEL L. QUEZON", "SERGIO OSMENA", "EMILIO AGUINALDO"}, {"D", "\0", "\0", "\0"}},
        {"EASY", "HISTORY AND LITERATURE", "IN WHICH YEAR DID THE PHILIPPINES GAIN INDEPENDENCE FROM SPAIN?", {"1898", "1899", "1897", "1896"}, {"A", "\0", "\0", "\0"}},
        {"EASY", "HISTORY AND LITERATURE", "WHO IS KNOWN AS THE MOTHER OF THE PHILIPPINE REVOLUTION?", {"KRIS AQUINO", "GABRIELA SILANG", "MELCHORA AQUINO", "IMELDA MARCOS"}, {"C", "\0", "\0", "\0"}},
        {"EASY", "HISTORY AND LITERATURE", "WHO WROTE ROMEO AND JULIET?", {"WILLIAM SHAKESPEARE", "J.K ROWLING", "MARK TWAIN", "CHARLES DICKENS"}, {"A", "\0", "\0", "\0"}},
        {"EASY", "HISTORY AND LITERATURE", "IN WHAT YEAR DID THE SPANIARDS REACH THE PHILIPPINE ISLANDS?", {"1521", "1522", "1523", "1520"}, {"A", "\0", "\0", "\0"}},
        {"EASY", "HISTORY AND LITERATURE", "WHAT IS THE NAME OF THE MOTHER OF JOSE RIZAL?", {"MARIA", "TEODORA", "ALONZA", "LOLITA"}, {"B", "\0", "\0", "\0"}},
        {"EASY", "HISTORY AND LITERATURE", "WHO COMPLETED THE VOYAGE OF MAGELLAN AFTER HE DIED IN THE BATTLE OF MACTAN?", {"JUAN SEBASTIAN ELCANO", "RUY DE VILLALOBOS", "VASCO DE GAMA", "BERNARDO DE LA TORRE"}, {"A", "\0", "\0", "\0"}},
        {"EASY", "HISTORY AND LITERATURE", "WHICH OF THE FOLLOWING IS CONSIDERED AS THE FOUNDATIONAL NOVEL OF THE PHILIPPINES?", {"EL FILIBUSTERISMO", "NOLI ME TANGERE", "FLORANTE AT LAURA", "IBONG ADARNA"}, {"B", "\0", "\0", "\0"}},
        {"EASY", "HISTORY AND LITERATURE", "WHAT COUNTRY GIFTED THE STATUE OF LIBERTY TO THE UNITED STATES?", {"UNITED KINGDOM", "GERMANY", "FRANCE", "RUSSIA"}, {"C", "\0", "\0", "\0"}},
        {"EASY", "HISTORY AND LITERATURE", "THE ADVENTURES OF TOM SAWYER WAS WRITTEN BY WHICH AMERICAN AUTHOR?", {"ERNEST HEMINGWAY", "NATHANIEL HAWTHORNE", "OLIVIA CLEMENS", "MARK TWAIN"}, {"D", "\0", "\0", "\0"}},

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

        {"AVERAGE", "HISTORY AND LITERATURE", "IN THE ORIGINAL STORY PUBLISHED IN 1812, HOW OLD WAS SNOW WHITE?", {"14 YEARS OLD", "12 YEARS OLD", "9 YEARS OLD", "7 YEARS OLD"}, {"D", "\0", "\0", "\0"}},
        {"AVERAGE", "HISTORY AND LITERATURE", "WHICH HISTORICAL EVENT MARKED THE START OF WORLD WAR II?", {"ATTACH OF PEARL HARBOR", "BATTLE OF BRITAIN", "THE INVASION OF POLAND", "D-DAY LANDINGS"}, {"C", "\0", "\0", "\0"}},
        {"AVERAGE", "HISTORY AND LITERATURE", "WHO IS THE FIRST PRESIDENT OF THE THIRD REPUBLIC OF THE PHILIPPINES?", {"MANUEL QUEZON", "MANUEL ROXAS", "SERGIO OSMENA", "RAMON MAGSAYSAY"}, {"B", "\0", "\0", "\0"}},
        {"AVERAGE", "HISTORY AND LITERATURE", "WHICH TREATY ENDED THE SPANISH-AMERICAN WAR, LEADING TO THE CESSION OF THE PHILIPPINES TO THE UNITED STATES?", {"TREATY OF VERSAILLES", "TREATY OF WESTPHALIA", "TREATY OF TORDESILLAS", "TREATY OF PARIS"}, {"D", "\0", "\0", "\0"}},
        {"AVERAGE", "HISTORY AND LITERATURE", "WHO WROTE PRIDE AND PREJUDICE?", {"JANE AUSTEN", "MARY SHELLEY", "EMILY BRONTE", "GEORGE ELIOT"}, {"A", "\0", "\0", "\0"}},
        {"AVERAGE", "HISTORY AND LITERATURE", "WHO WAS THE FIRST PRINCESS DON JUAN MET IN THE EPIC IBONG ADARNA?", {"JUANA", "MARIA", "LEONORA", "TERESA"}, {"A", "\0", "\0", "\0"}},
        {"AVERAGE", "HISTORY AND LITERATURE", "WHICH FILIPINO POET WROTE THE EPIC FLORANTE AT LAURA?", {"JOSE RIZAL", "KRIZ BIHAG", "FRANCISCO BALAGTAS", "LUALHATI BAUTISTA"}, {"C", "\0", "\0", "\0"}},
        {"AVERAGE", "HISTORY AND LITERATURE", "WHICH OF THE FOLLOWING HAPPENED FIRST?", {"THE CREATION OF TOM AND JERRY", "THE FOUNDATION OF DISNEY", "THE PUBLISHMENT OF THE NOVEL HOW TO KILL A MOCKINGBIRD", "THE OPENING OF THE SOUND OF MUSIC STAGE MUSICAL"}, {"B", "\0", "\0", "\0"}},
        {"AVERAGE", "HISTORY AND LITERATURE", "WHAT IS THE TITLE OF THE FIFTH BOOK IN J.K ROWLING'S HARRY POTTER SERIES", {"HARRY POTTER AND THE CHAMBER OF SECRETS", "HARRY POTTER AND THE SORCERER'S STONE", "HARRY POTTER AND THE GOBLET OF FIRE", "HARRY POTTER AND THE ORDER OF THE PHOENIX"}, {"D", "\0", "\0", "\0"}},
        {"AVERAGE", "HISTORY AND LITERATURE", "WHO IS THE PROTAGONIST IN THE NOVEL LITTLE WOMEN?", {"JOSEPHINE MARCH", "BETH MARCH", "THEODORE LAURENCE", "SALLIE MOFFAT"}, {"A", "\0", "\0", "\0"}},

        {"DIFFICULT", "MATHEMATICS", "WHAT IS THE COMPLEX NUMBER E RAISED TO (I)(PI) EQUAL TO?", {"1", "-1", "PI", "E"}, {"B", "\0", "\0", "\0"}},
        {"DIFFICULT", "MATHEMATICS", "IF ANY NUMBER RAISED TO ZERO IS 1, THEN WHAT IS ZERO RAISED TO ZERO?", {"1", "0", "UNDEFINED", "THE ANSWER IS LEFT AS AN EXERCISE!"}, {"D", "\0", "\0", "\0"}},
        {"DIFFICULT", "MATHEMATICS", "ARE THE DERIVATIVES OF -1/(X+1) AND X/(X+1) THE SAME?", {"YES", "MAYBE", "NO", "SECRET"}, {"A", "\0", "\0", "\0"}},
        {"DIFFICULT", "MATHEMATICS", "WHAT IS THE SMALLEST NUMBER THAT IS DIVISIBLE BY 1, 2, 3, 4, 5, 6, 7, 8, 9, AND 10?", {"2510", "2490", "2480", "2520"}, {"D", "\0", "\0", "\0"}},
        {"DIFFICULT", "MATHEMATICS", "WHAT IS 10!/7! EQUAL TO?", {"720", "910", "820", "610"}, {"A", "\0", "\0", "\0"}},
        {"DIFFICULT", "MATHEMATICS", "LESTER IS 14 YEARS OLDER THAN JEN. IN 6 YEARS, LESTER WILL BE THREE TIMES AS OLD AS JEN. HOW OLD IS LESTER NOW?", {"12", "13", "14", "15"}, {"D", "\0", "\0", "\0"}},
        {"DIFFICULT", "MATHEMATICS", "WHAT IS THE MATHEMATICAL NAME FOR THE SYMBOL #?", {"HASHTAG", "OCTOTHORPE", "HOTLINE", "OCTOTHROPE"}, {"B", "\0", "\0", "\0"}},
        {"DIFFICULT", "MATHEMATICS", "A CIRCLE IS CENTERED AT THE ORIGIN. GIVEN POINT (4,3) ON THE CIRCLE, WHAT IS THE RADIUS OF THE CIRCLE?", {"4", "6", "8", "5"}, {"D", "\0", "\0", "\0"}},
        {"DIFFICULT", "MATHEMATICS", "WHAT IS THE VALUE OF PI TO TEN DECIMAL PLACES?", {"3.1415926297", "3.1415926815", "3.1415926535", "3.1415629535"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "MATHEMATICS", "HOW MANY YEARS ARE THERE IN ONE BILLION SECONDS?", {"32 YEARS", "33 YEARS", "30 YEARS", "31 YEARS"}, {"D", "\0", "\0", "\0"}},

        {"DIFFICULT", "SCIENCE", "WHAT IS THE ONLY MAMMAL THAT HAS SCALES?", {"ARMADILLO", "PANGOLIN", "PORCUPINE", "PLATYPUS"}, {"B", "\0", "\0", "\0"}},
        {"DIFFICULT", "SCIENCE", "THE LONGEST WORD IN ENGLISH PERTAINS TO WHAT KIND OF DISEASE?", {"SKIN DISEASES", "LUNG DISEASE", "HEART DISEASE", "LIVER DISEASE"}, {"B", "\0", "\0", "\0"}},
        {"DIFFICULT", "SCIENCE", "THE HG SYMBOL FOR MERCURY CAME FROM WHAT WORD?", {"HYDRARGYRUM", "HYDRAGERAM", "HYDRAGOS", "HYDRAGORUM"}, {"A", "\0", "\0", "\0"}},
        {"DIFFICULT", "SCIENCE", "WHAT IS THE FIRST METAL USED BY ANCIENT PEOPLE?", {"TIN", "COPPER", "GOLD", "SILVER"}, {"B", "\0", "\0", "\0"}},
        {"DIFFICULT", "SCIENCE", "WHAT IS DIAMOND MADE OF?", {"MAGNESIUM", "BORON", "CARBON", "SODIUM"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "SCIENCE", "WHAT IS THE 100TH ELEMENT ON THE PERIODIC TABLE?", {"MENDELEVIUM", "RUTHERFORDIUM", "AMERICIUM", "FERMIUM"}, {"D", "\0", "\0", "\0"}},
        {"DIFFICULT", "SCIENCE", "IS WATER WET?", {"SECRET", "SOMETIMES", "YES", "NO"}, {"D", "\0", "\0", "\0"}},
        {"DIFFICULT", "SCIENCE", "HOW MANY LETTERS DOES THE FEAR OF LONG WORDS HAVE?", {"41", "36", "29", "44"}, {"B", "\0", "\0", "\0"}},
        {"DIFFICULT", "SCIENCE", "STARS ARE MOSTLY MADE OF WHAT ELEMENT?", {"HYDROGEN AND NITROGEN", "CARBON AND NITROGEN", "HYDROGEN AND HELIUM", "NITROGEN AND HELIUM"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "SCIENCE", "WHAT IS THE ELECTRONIC CONFIGURATION OF OXYGEN?", {"1S 2S 2P^2", "1S^2 2S^2 2P", "1S^2 2S^2 2P^6", "1S^2 2S^2 2P^4"}, {"D", "\0", "\0", "\0"}},

        {"DIFFICULT", "COMPUTER SCIENCE", "WHAT IS THE BINARY EQUIVALENT OF 12345?", {"11000000111001", "11000000111011", "10000000111001", "11000000101001"}, {"A", "\0", "\0", "\0"}},
        {"DIFFICULT", "COMPUTER SCIENCE", "IF I HAVE A 2-GIGABYTE MEMORY CARD, APPROXIMATELY HOW MANY PHOTOS CAN IT HOLD IF ONE PHOTO TAKES UP 20 MEGABYTES?", {"200", "100", "150", "250"}, {"B", "\0", "\0", "\0"}},
        {"DIFFICULT", "COMPUTER SCIENCE", "WHAT IS THE CORRECT SYNTAX TO DEFINE A FUNCTION POINTER IN C THAT POINTS TO A FUNCTION TAKING TWO INTEGERS AND RETURNING AN INTEGER?", {"INT(*FUNCPTR)(INT,INT)", "INT*FUNCPTR(INT,INT)", "INT(*FUNCPTR)()", "INT*(*FUNCPTR)(INT,INT)"}, {"A", "\0", "\0", "\0"}},
        {"DIFFICULT", "COMPUTER SCIENCE", "WHAT COMPUTER TERM DID WEB BROWSER PROGRAMMER LOU MONTULLI COINED TO REFER TO INFORMATION THAT IS SENT FROM THE BROWSER TO THE WEB SERVER?", {"COOKIE", "CACHE", "CAPTCHA", "BYTES"}, {"A", "\0", "\0", "\0"}},
        {"DIFFICULT", "COMPUTER SCIENCE", "WHAT YEAR WAS THE FIRST IPHONE RELEASED?", {"2008", "2007", "2009", "2011"}, {"B", "\0", "\0", "\0"}},
        {"DIFFICULT", "COMPUTER SCIENCE", "HOW MANY COLORS ARE USED IN THE LETTERING OF THE GOOGLE LOGO?", {"3", "4", "5", "6"}, {"B", "\0", "\0", "\0"}},
        {"DIFFICULT", "COMPUTER SCIENCE", "WHAT IS THE NICKNAME OF ANDROID 5?", {"GINGERBREAD", "OREO", "LOLLIPOP", "ECLAIR"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "COMPUTER SCIENCE", "WHICH HAPPENED FIRST?", {"THE SENDING OF THE FIRST EMAIL", "THE FOUNDING OF GOOGLE", "THE INVENTION OF THE FIRST CELLPHONE", "THE LAUNCHING OF THE FIRST WEBSITE"}, {"A", "\0", "\0", "\0"}},
        {"DIFFICULT", "COMPUTER SCIENCE", "WHAT PROGRAMMING LANGUAGE IS KNOWN FOR ITS SIMPLICITY AND IS OFTEN RECOMMENDED AS A FIRST PROGRAMMING LANGUAGE?", {"C", "CPP", "PYTHON", "JAVASCRIPT"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "COMPUTER SCIENCE", "WHAT WAS THE NAME OF THE FIRST MECHANICAL COMPUTER DESIGNED BY CHARLES BABBAGE?", {"ANALYTICAL ENGINE", "ENIAC", "ABACUS", "PASCALINE"}, {"A", "\0", "\0", "\0"}},

        {"DIFFICULT", "GEOGRAPHY", "IN WHAT LAYER OF THE EARTH'S ATMOSPHERE DO THE NORTHERN LIGHTS DEVELOP?", {"THERMOSPHERE", "TROPOSPHERE", "STRATOSPHERE", "EXOSPHERE"}, {"A", "\0", "\0", "\0"}},
        {"DIFFICULT", "GEOGRAPHY", "IN THIS LAYER OF THE ATMOSPHERE, METEORS BURN?", {"THERMOSPHERE", "MESOSPHERE", "EXOSPHERE", "STRATOSPHERE"}, {"B", "\0", "\0", "\0"}},
        {"DIFFICULT", "GEOGRAPHY", "WHICH OF THE FOLLOWING HAS THE LEAST POPULATION?", {"QUEZON CITY", "CEBU CITY", "PASIG CITY", "MANILA"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "GEOGRAPHY", "WHICH COUNTRY IS CONSIDERED TO LIE CLOSE TO WHERE THE SUN RISES?", {"KOREA", "NEW ZEALAND", "JAPAN", "THE PHILIPPINES"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "GEOGRAPHY", "WHICH OF THE FOLLOWING IS BOTH AN INDEPENDENT COUNTRY AND A CITY STATE?", {"SINGAPORE", "HONG KONG", "PALAU", "FIJI"}, {"A", "\0", "\0", "\0"}},
        {"DIFFICULT", "GEOGRAPHY", "HOW MANY ARE THERE IN ASIA?", {"50", "48", "47", "44"}, {"B", "\0", "\0", "\0"}},
        {"DIFFICULT", "GEOGRAPHY", "WHAT IS THE LARGEST DESERT IN THE WORLD?", {"SAHARA", "ARCTIC", "ANTARCTIC", "GOBI"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "GEOGRAPHY", "WHAT IS THE DRIEST PLACE ON EARTH?", {"SAHARA", "GOBI", "ATACAMA", "ANTARCTIC"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "GEOGRAPHY", "WHICH OF THE FOLLOWING IS NOT A REAL CURRENCY?", {"MANAT", "LEV", "RENMINBI", "DALES"}, {"D", "\0", "\0", "\0"}},
        {"DIFFICULT", "GEOGRAPHY", "APPROXIMATELY HOW MANY SOVEREIGN COUNTRIES BORDER THE PACIFIC OCEAN?", {"MORE THAN 20 BUT LESS THAN 40", "LESS THAN 40", "MORE THAN 40 BUT LESS THAN 60", "MORE THAN 60"}, {"C", "\0", "\0", "\0"}},

        {"DIFFICULT", "HISTORY AND LITERATURE", "WHO IS THE ONLY PRESIDENT OF THE FOURTH REPUBLIC OF THE PHILIPPINES?", {"CORAZON AQUINO", "FERDINAND MARCOS SR.", "ELPIDIO QUIRINO", "FIDEL RAMOS"}, {"B", "\0", "\0", "\0"}},
        {"DIFFICULT", "HISTORY AND LITERATURE", "WHAT IS CONSIDERED TO BE THE FIRST BOOK EVER WRITTEN?", {"KESH TEMPLE HYMN", "THE BIBLE", "A FIRST COURSE IN ABSTRACT ALGBERA", "THE EPIC OF GILGAMESH"}, {"D", "\0", "\0", "\0"}},
        {"DIFFICULT", "HISTORY AND LITERATURE", "WHAT IS THE LONGEST RUNNING ANIMATED TELEVISION SERIES IN THE WORLD?", {"POKEMON", "DORAEMON", "SAZAE-SAN", "DETECTIVE CONAN"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "HISTORY AND LITERATURE", "WHICH ANCIENT CIVILIZATION BUILT THE CITY OF MACHU PICCHU?", {"THE AZTECS", "THE MAYA", "THE OLMEC", "THE INCA"}, {"D", "\0", "\0", "\0"}},
        {"DIFFICULT", "HISTORY AND LITERATURE", "IN THE DIVINE COMEDY, WHO SERVES AS DANTE'S GUIDE THROUGH HELL?", {"LUCIFER", "BEATRICE", "VIRGIL", "CHARON"}, {"C", "\0", "\0", "\0"}},
        {"DIFFICULT", "HISTORY AND LITERATURE", "WHICH NOVEL BY GEORGE ORWELL DEPICTS A DYSTOPIAN FUTURE UNDER A TOTALITARIAN REGIME?", {"ANIMAL FARM", "BRAVE NEW WORLD", "FAHRENHEIT 451", "NINETEEN EIGHTY-FOUR"}, {"D", "\0", "\0", "\0"}},
        {"DIFFICULT", "HISTORY AND LITERATURE", "WHAT IS THE LONGEST RUNNING PHILIPPINE TV SHOW?", {"KAPWA KO, MAHAL KO", "TV PATROL", "EAT BULAGA", "SALAMAT DOC"}, {"A", "\0", "\0", "\0"}},
        {"DIFFICULT", "HISTORY AND LITERATURE", "WHO IS THE FIRST PRESIDENT OF THE FIFTH REPUBLIC OF THE PHILIPPINES?", {"FERDINAND MARCOS SR.", "CORAZON AQUINO", "FIDEL RAMOS", "GLORIA MACAPAGAL-ARROYO"}, {"B", "\0", "\0", "\0"}},
        {"DIFFICULT", "HISTORY AND LITERATURE", "WHO WROTE THE ORIGINAL THE INCREDIBLE HULK COMIC SERIES?", {"STAN LEE AND JACK KIRBY", "STEVE DIKTO AND STAN LEE", "JACK KIRBY AND JOE SIMON", "STAN LEE AND JOHN ROMITA SR."}, {"A", "\0", "\0", "\0"}},
        {"DIFFICULT", "HISTORY AND LITERATURE", "WHO IS MARVEL'S FIRST SUPERHERO?", {"DARNA", "ANGEL", "NAMOR THE SUB-MARINER", "HUMAN TORCH"}, {"D", "\0", "\0", "\0"}},
        
       
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

void mark_category_used(int categoryIndex, Game* game) {
    switch (game->difficulty) {
        case EASY:
            used_easy_category[categoryIndex] = true;
            break;
        case AVERAGE:
            used_average_category[categoryIndex] = true;
            break;
        case DIFFICULT:
            used_hard_category[categoryIndex] = true;
            break;
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

void clear_screen() {
    system("cls");
}

void init_player(Game* game) {
    clear_screen();

    printf("\n");
    center_text("BEFORE WE START, PLEASE ENTER YOUR NAME\n", 110);
    center_text("PRESS [ENTER] IF DONE\n", 110);
    printf("                                                ┌──────────\n");
    printf("                                                │ ");

    game->player.name = malloc((MAX_NAME_LENGTH + 1) * sizeof(char)); 
    scanf("%10s", game->player.name); 

    for (int i = 0; game->player.name[i]; i++) {
        game->player.name[i] = toupper(game->player.name[i]);
    }

    game->player.score = 16500;
    game->player.lives = 3;

    memset(game->clicked_powerup, 0, sizeof(game->clicked_powerup));
    memset(game->used_powerup, 0, sizeof(game->used_powerup));
    for (int i = 0; i < 10; i++) {
        used_easy_category[i] = false;
        used_average_category[i] = false;
        used_hard_category[i] = false;
    }
    cat_flg1 = 0;
    cat_flg2 = 0;
    cat_flg3 = 0;
}

void confirm_name(Game* game) {
    clear_screen();

    printf("\n");
    char arr[100];

    snprintf(arr, sizeof(arr), "IS [%s] THE NAME YOU'D LIKE TO USE THROUGHOUT THE GAME?", game->player.name);
    center_text(arr,100);
    center_text("[1] YES    [2] NO\n", 100);
}

void add_spaces(int size, int max_size) {
    int spaces = max_size - size;
    if (spaces > 0) {
        printf("%*s", spaces, "");  
    }
}

void init_difficulty(Game* game) { 
    int score = game->player.score;

    if (score < 2000) {
        game->difficulty = EASY;
    } else if (score < 6500) {
        game->difficulty = AVERAGE;
    } else {
        game->difficulty = DIFFICULT;
    }
}

myNode* get_node(SLList* list, int index) {
    myNode* current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current;
}

myNode* create_node(char* x) {
    myNode* a = (myNode*)malloc(sizeof(myNode));
    a->value = strdup(x);  // Allocate memory for the string and copy it
    a->next = 0;
    return a;
}

void init_list(SLList* list) {
    list->head = 0;
    list->tail = 0;
    list->size = 0;
}

void populate_list(SLList *list, int *index, Quiz *quizzes, int num_items, int current) {
    if (current < num_items) {
        insert_item(list, current, quizzes[index[current]].category);
        populate_list(list, index, quizzes, num_items, current + 1);
    }
}


char* get_item(SLList* list, int index) {
    if (index >= 0 && index <= list->size - 1) {
        myNode* node = get_node(list, index);
        return node->value;
    }
    return NULL;
}

void set_item(SLList* list, int index, char* item) {
    myNode* node = get_node(list, index);
    free(node->value);  
    node->value = strdup(item);  
}

void insert_item(SLList* list, int index, char* item) {
    myNode* toInsert = create_node(item);
    if (index == 0) {
        toInsert->next = list->head;
        list->head = toInsert;
    }
    if (index == list->size) {
        if (list->tail != 0) {
            list->tail->next = toInsert;
        }
        list->tail = toInsert;
    }
    if (index > 0 && index < list->size) {
        myNode* previous = get_node(list, index - 1);
        toInsert->next = previous->next;
        previous->next = toInsert;
    }
    list->size++;
}

void remove_tem(SLList* list, int index) {
    myNode* toRemove;
    if (index == 0) {
        toRemove = list->head;
        list->head = toRemove->next;
    } else {
        myNode* previous = get_node(list, index - 1);
        toRemove = previous->next;
        previous->next = toRemove->next;
        if (index == list->size - 1) {
            list->tail = previous;
        }
    }
    free(toRemove->value);  // Free the string
    free(toRemove);  // Free the node
    list->size--;
}

void printList(SLList* list) {
    if (list->size == 0) {
        printf("Nothing to print here!\n");
    } else {
        myNode* toPrint;
        for (int i = 0; i < 3; i++) {
            toPrint = get_node(list, i);
            printf("Item %d = %s\n", i, toPrint->value);
        }
    }
}

void clear_list(SLList* list) {
    myNode* current = list->head;
    myNode* nextNode;
    while (current != NULL) {
        nextNode = current->next;
        free(current->value);
        free(current);
        current = nextNode;
    }
    init_list(list); // Reinitialize the list
}

int choose_category(Game* game, SLList* list, int cat_index[MAX_CATEGORIES]) {

    
    if ((game->difficulty == EASY && !n1) || 
        (game->difficulty == AVERAGE && !n2) || 
        (game->difficulty == DIFFICULT && !n3)) {
        for (int i = 0; i < MAX_CATEGORIES; i++) {
            if (game->difficulty == EASY) {
                cat_index[i] += 0;
            } else if (game->difficulty == AVERAGE) {
                cat_index[i] += 50;
            } else if (game->difficulty == DIFFICULT) {
                cat_index[i] += 100;
            }
        }
    
        if (game->difficulty == EASY) {
            n1 = 1;
        } else if (game->difficulty == AVERAGE) {
            n2 = 1;
        } else if (game->difficulty == DIFFICULT) {
            n3 = 1;
        }
    }

   

    if (game->difficulty == EASY) {
        if (dif1 == 0) {
            clear_screen();
            puts(easy_remark1);
            press_continue();
            dif1 = 1;
        } else {
            clear_screen();
            puts(easy_remark2);
            press_continue();
        }
    } else if (game->difficulty == AVERAGE) {
        if (dif2 == 0) {
            clear_screen();
            puts(average_remark1);
            press_continue();
            clear_screen();
            puts(average_remark2);
            press_continue();
            dif2 = 1;
        } else {
            clear_screen();
            puts(average_remark3);
            press_continue();
        }
    } else if (game->difficulty == DIFFICULT) {
        if (dif3 == 0) {
            clear_screen();
            puts(difficult_remark1);
            press_continue();
            clear_screen();
            puts(difficult_remark2);
            press_continue();
            dif3 = 1;
        } else {
            clear_screen();
            puts(difficult_remark3);
            press_continue();
        }
    }

    

    

    

    
    clear_screen();

    char level[100]; 

    sprintf(level, "%s%*s", game->quizzes[cat_index[0]].level, 25 - strlen(game->quizzes[cat_index[0]].level), "");
    // snprintf(level, sizeof(level), "%s%*s", game->quizzes[cat_index[0]].level, 25 - strlen(game->quizzes[cat_index[0]].level), "");


    

    char category1[50], category2[50], category3[50];
    char cat1[50], cat2[50], cat3[50];
    myNode* toPrint;
    for (int i = 0; (list->size > 3) ? (i < 3) : (i < list->size); i++) {
        toPrint = get_node(list, i);
        if (i == 0) {
            strcpy(cat1, toPrint->value);
            int spaces_to_add = 22 - strlen(cat1);
            if (spaces_to_add < 0) spaces_to_add = 0; 

            

            sprintf(category1, "%s%*s", cat1, spaces_to_add, "");

            
        } else if (i == 1) {
            strcpy(cat2, toPrint->value);
            int spaces_to_add = 22 - strlen(cat2);
            if (spaces_to_add < 0) spaces_to_add = 0; 

            sprintf(category2, "%s%*s", cat2, spaces_to_add, "");
        } else if (i == 2) {
            strcpy(cat3, toPrint->value);
            int spaces_to_add = 22 - strlen(cat3);
            if (spaces_to_add < 0) spaces_to_add = 0; 

            sprintf(category3, "%s%*s", cat3, spaces_to_add, "");
        }
    }
    





    // char final_string[2048];
    printf(show_category, level, category1, category2, category3);

    



    char input = '0'; 
    do {
        input = getch();
    } while ((input != '1') && (input != '2') && (input != '3'));

    char item[50];
    if (input == '1') {
        strcpy(item, cat1);
    } else if (input == '2') {
        strcpy(item, cat2);
    } else if (input == '3') {
        strcpy(item, cat3);
    }

    remove_tem(list, input - '1');    

    for (int i = 0; i < MAX_CATEGORIES; i++) {
        if (strcmp(item, game->quizzes[cat_index[i]].category) == 0) {
            return cat_index[i];
        }
    }
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

void init_welcome_remarks(Game* game) {
    int name_length = strlen(game->player.name);
    int spaces_to_add = 10 - name_length;
    if (spaces_to_add < 0) spaces_to_add = 0; 
    char formatted_name[30]; 
    snprintf(formatted_name, sizeof(formatted_name), "%s!%*s", game->player.name, spaces_to_add, "");

    printf(welcome_screen1, formatted_name);
    press_continue();

    puts(welcome_screen2);
    press_continue();

    puts(welcome_screen3);
    press_continue();

    puts(welcome_screen4);
    press_continue();

    mechanics:

    puts(mechanics1);
    press_continue();
    puts(mechanics2);
    press_continue();
    puts(mechanics3);
            

    clear_screen();
    puts(welcome_screen5);
    char input = '0';
    do {
        input = getch();
    } while ((input != '2') && (input != '1'));

    if (input == '2') {
        clear_screen();
        goto mechanics;
    }

    
            
            
    clear_screen();
    puts(welcome_screen6);
    press_continue();
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

            clear_screen();
            puts(separator);
            
            char *header1 = NULL;
            int header1_size = 0;

            char *header2 = NULL;
            int header2_size = 0;

            format_header1(&header1, &header1_size, game->player.name, game->player.score, game->player.lives);
            format_header2(&header2, &header2_size, game->quizzes[que_index[i] + start_index].level, game->quizzes[que_index[i] + start_index].category, time_left, i+1);

            printf("%s\n", header1);
            printf("%s\n", header2);



        

            puts(separator);

            printf("\n"); 
            center_text(game->quizzes[que_index[i] + start_index].question, 75);

            printf("\n");

            if(!(game->clicked_powerup[1]) && (game->used_powerup[1])) {
                if (cht1) {
                    for(int j = 0; j < 4; j++) {
                        while(del_index == ((game->quizzes[que_index[i] + start_index].correct_answer[0][0]) - 'A')) {
                            del_index = rand() % 4;
                        } 
                        if(opt_index[j] != del_index) {
                            if(opt_index[j] == ((game->quizzes[que_index[i] + start_index].correct_answer[0][0]) - 'A')) {
                                printf("                       ─────────────────────────────────────────────────────────────\n");
                                char text[120];
                                char options[120];
                                snprintf(text, sizeof(text), "%*s\033[0;92m%s\033[0m%*s", (40 - strlen(game->quizzes[que_index[i] + start_index].option[opt_index[j]]))/2, "", game->quizzes[que_index[i] + start_index].option[opt_index[j]], (40 - strlen(game->quizzes[que_index[i] + start_index].option[opt_index[j]]))/2, "");
                                snprintf(options, sizeof(options), "[%c]%*s", 'A' + j, 60, "");
                                center_text(options, 70);
                                center_text(text, 70);
                            } else {
                                printf("                       ─────────────────────────────────────────────────────────────\n");
                                char text[120];
                                char options[120];
                                snprintf(text, sizeof(text), "%*s\033[0;91m%s\033[0m%*s", (40 - strlen(game->quizzes[que_index[i] + start_index].option[opt_index[j]]))/2, "", game->quizzes[que_index[i] + start_index].option[opt_index[j]], (40 - strlen(game->quizzes[que_index[i] + start_index].option[opt_index[j]]))/2, "");
                                snprintf(options, sizeof(options), "[%c]%*s", 'A' + j, 60, "");
                                center_text(options, 70);
                                center_text(text, 70);
                            }
                        }
                    }

                } else {
                    for(int j = 0; j < 4; j++) {
                        while(del_index == ((game->quizzes[que_index[i] + start_index].correct_answer[0][0]) - 'A')) {
                            del_index = rand() % 4;
                        } 
                        if(opt_index[j] != del_index) {
                            printf("                       ─────────────────────────────────────────────────────────────\n");
                            char text[120];
                            char options[120];
                            snprintf(text, sizeof(text), "%*s%s%*s", (40 - strlen(game->quizzes[que_index[i] + start_index].option[opt_index[j]]))/2, "", game->quizzes[que_index[i] + start_index].option[opt_index[j]], (40 - strlen(game->quizzes[que_index[i] + start_index].option[opt_index[j]]))/2, "");
                            snprintf(options, sizeof(options), "[%c]%*s", 'A' + j, 60, "");
                            center_text(options, 70);
                            center_text(text, 70);
                        }
                    }
                }
            } else {
                if (cht1) {
                    for(int j = 0; j < 4; j++) {
                        if(opt_index[j] == ((game->quizzes[que_index[i] + start_index].correct_answer[0][0]) - 'A')) {
                            printf("                       ─────────────────────────────────────────────────────────────\n");
                            char text[120];
                            char options[120];
                            snprintf(text, sizeof(text), "%*s\033[0;92m%s\033[0m%*s", (40 - strlen(game->quizzes[que_index[i] + start_index].option[opt_index[j]]))/2, "", game->quizzes[que_index[i] + start_index].option[opt_index[j]], (40 - strlen(game->quizzes[que_index[i] + start_index].option[opt_index[j]]))/2, "");
                            snprintf(options, sizeof(options), "[%c]%*s", 'A' + j, 60, "");
                            center_text(options, 70);
                            center_text(text, 70);
                        } else {
                            printf("                       ─────────────────────────────────────────────────────────────\n");
                            char text[120];
                            char options[120];
                            snprintf(text, sizeof(text), "%*s\033[0;91m%s\033[0m%*s", (40 - strlen(game->quizzes[que_index[i] + start_index].option[opt_index[j]]))/2, "", game->quizzes[que_index[i] + start_index].option[opt_index[j]], (40 - strlen(game->quizzes[que_index[i] + start_index].option[opt_index[j]]))/2, "");
                            snprintf(options, sizeof(options), "[%c]%*s", 'A' + j, 60, "");
                            center_text(options, 70);
                            center_text(text, 70);
                        }
                    }
                } else {                                                                                                                      
                    for(int j = 0; j < 4; j++) {
                        printf("                       ────────────────────────────────────────────────────────────────\n");
                        char text[120];
                        char options[120];
                        snprintf(text, sizeof(text), "%*s%s%*s", (40 - strlen(game->quizzes[que_index[i] + start_index].option[opt_index[j]]))/2, "", game->quizzes[que_index[i] + start_index].option[opt_index[j]], (40 - strlen(game->quizzes[que_index[i] + start_index].option[opt_index[j]]))/2, "");
                        snprintf(options, sizeof(options), "[%c]%*s", 'A' + j, 60, "");
                        center_text(options, 70);
                        center_text(text, 70);

                    }
                }
            }
            printf("                       ────────────────────────────────────────────────────────────────\n");
            puts(separator);


            char ar[150] = "";
            char ray[150];
            for (int j = 0; j < 3; j++) {
                if (!(game->used_powerup[pow_index[j]])) {
                    // Build the string with the current powerup
                    snprintf(ray, sizeof(ray), "%s    [%d]  %s", ar, j + 1, game->powerups[pow_index[j]].name);
                    // Copy the new string back to ar
                    strcpy(ar, ray);
                }
            }
            // Center the resulting string and print it
            center_text(ar, 110);
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

                    if(is_correct_answer(answer, game->quizzes[que_index[i] + start_index])) {
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
                        }
                        if (!(game->clicked_powerup[4]) && (game->used_powerup[4])) {
                            game->clicked_powerup[4] = 1;
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
                            Sleep(600);
                            system("COLOR 07");
                            Sleep(200);
                            PlaySound(TEXT("assets/play.wav"), NULL, SND_FILENAME | SND_ASYNC);
                        } else {
                            game->player.lives--;
                            answer_chosen = 1;
                        }
                    }
                    
                    goto reset;
                } else if ((user_answer == '1') || (user_answer == '2') || (user_answer == '3')) {
                    if (user_answer == '1') {
                        game->used_powerup[pow_index[0]] = 1;  
                    } else if (user_answer == '2') {
                        game->used_powerup[pow_index[1]] = 1;  
                    } else if (user_answer == '3') {
                        game->used_powerup[pow_index[2]] = 1;  
                    }
                    continue;
                } else if (user_answer == 1) {
                    PlaySound(NULL, 0, 0);
                    clear_screen();
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
                } else {
                    continue;
                }
            }

            reset:
            Sleep(1000);
            system("COLOR 07");


            if ((!(game->clicked_powerup[0]) && (game->used_powerup[0])) || cht2) {
                time_left;
            } else {
                time_left--;
            }

            

            if (time_left == 0) {
                game->player.lives--;
            }

            
            if ((game->player.lives == 0)) {
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

int is_correct_answer(char user_answer, Quiz quiz) {
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

void press_continue() {

    char input = '0';
    do {
        input = getch();
    } while ((input != ' '));

    clear_screen();
}

void format_header1(char **buffer, int *size, const char *player_name, int score, int lives) {
    const char *livesDisplay = lives == 3 ? "❤️ ❤️ ❤️" : (lives == 2 ? "❤️ ❤️ \e[0;93m💔\e[0m" : "❤️ 💔 💔");
    *size = snprintf(NULL, 0, "%*sPLAYER NAME : %s%*sSCORE : %04d%*sLIVES : %s",
                     4, "", player_name, 
                     49 - 4 - (int)strlen(player_name) - 14, "", 
                     score, 
                     42 - 9 - 4, "", 
                     livesDisplay) + 1; // +1 for null terminator

    *buffer = (char *)realloc(*buffer, *size);
    if (*buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    snprintf(*buffer, *size, "%*sPLAYER NAME : %s%*sSCORE : %04d%*sLIVES : %s",
             4, "", player_name, 
             49 - 4 - (int)strlen(player_name) - 14, "", 
             score, 
             42 - 9 - 4, "", 
             livesDisplay);
}
void format_header2(char **buffer, int *size, const char *level, const char *category, int time_left, int question_number) {
    *size = snprintf(NULL, 0, "%*s%s : %s%*sTIMER : %02d%*sQUESTION : %02d%*s", 
                     4, "", 
                     level, category, 
                     47 - 4 - (int)strlen(level) - (int)strlen(category), "", 
                     time_left, 
                     45 - 4 - 8, "", 
                     question_number, 4, "") + 1; // +1 for null terminator

    *buffer = (char *)realloc(*buffer, *size);
    if (*buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    snprintf(*buffer, *size, "%*s%s : %s%*sTIMER : %02d%*sQUESTION : %02d%*s", 
             4, "", 
             level, category, 
             47 - 4 - (int)strlen(level) - (int)strlen(category), "", 
             time_left, 
             45 - 4 - 8, "", 
             question_number, 4, "");
}

void center_text (char* text, int len) {
    int textLength = strlen(text);
    int screen_width = 110;
    int totalLines = (textLength + len - 1) / len; 

    for (int i = 0; i < totalLines; i++) {
        int start = i * len;
        int end = start + len;
        if (end > textLength) {
            end = textLength;
        }
        int lineLength = end - start;

        int spaces = (screen_width - lineLength) / 2;

        for (int j = 0; j < spaces; j++) {
            printf(" ");
        }

        for (int j = start; j < end; j++) {
            printf("%c", text[j]);
        }
        printf("\n");
    }
}

char* separator = "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m";
                                        
char* title_screen1 =    "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                        "\n"
                        "      \n"                                                                                                       
                        "      \e[0;93m✦\e[0m                                          J&J PRESENTS                                          \e[0;93m✦\e[0m      \n"
                        "      \n" 
                        "                     \e[0;95m██████\e[0;92m╗ \e[0;95m██\e[0;92m╗   \e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m╗\e[0;95m███████\e[0;92m╗    \e[0;95m███\e[0;92m╗   \e[0;95m███\e[0;92m╗ \e[0;95m██████\e[0;92m╗ \e[0;95m██\e[0;92m╗  \e[0;95m██\e[0;92m╗ \e[0;95m██████\e[0;92m╗                     \n"
                        "          \e[0;93m✦\e[0m         \e[0;95m██\e[0;92m╔═══\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║╚══\e[0;95m███\e[0;92m╔╝    \e[0;95m████\e[0;92m╗ \e[0;95m████\e[0;92m║\e[0;95m██\e[0;92m╔═══\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m║ \e[0;95m██\e[0;92m╔╝\e[0;95m██\e[0;92m╔═══\e[0;95m██\e[0;92m╗         \e[0;93m✦\e[0m\n"
                        "                    \e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║  \e[0;95m███\e[0;92m╔╝     \e[0;95m██\e[0;92m╔\e[0;95m████\e[0;92m╔\e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║\e[0;95m█████\e[0;92m╔╝ \e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║                    \n"
                        "                    \e[0;95m██\e[0;92m║\e[0;95m▄▄ ██\e[0;92m║\e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║ \e[0;95m███\e[0;92m╔╝      \e[0;95m██\e[0;92m║╚\e[0;95m██\e[0;92m╔╝\e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m╔═\e[0;95m██\e[0;92m╗ \e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║                    \n"
                        "                    \e[0;92m╚\e[0;95m██████\e[0;92m╔╝╚\e[0;95m██████\e[0;92m╔╝\e[0;95m██\e[0;92m║\e[0;95m███████\e[0;92m╗    \e[0;95m██\e[0;92m║ ╚═╝ \e[0;95m██\e[0;92m║╚\e[0;95m██████\e[0;92m╔╝\e[0;95m██\e[0;92m║  \e[0;95m██\e[0;92m╗╚\e[0;95m██████\e[0;92m╔╝                    \n"
                        "                     \e[0;92m╚══\e[0;95m▀▀\e[0;92m═╝  ╚═════╝ ╚═╝╚══════╝    ╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═╝ ╚═════╝\e[0m \n"
                        "   \e[0;93m✦\e[0m                                                                                                     \e[0;93m✦\e[0m   \n"
                        "                                                                                                     \e[0;93m_ \e[0;93m\"\e[0;93m _\e[0m \n"
                        "                                ┌───────────────────┐    ┌───────────────────┐                      \e[0;93m(_\\\e[0;93m|\e[0;93m/_)\e[0m \n"
                        "                                │ [1] START         │    │ [2] ABOUT         │         \e[0;91m_      _\e[0m      \e[0;93m(/\e[0;93m|\e[0;93m\\)\e[0m\n"
                        "     \e[0;93m                __  \e[0m       └───────────────────┘    └───────────────────┘       \e[0;91m_(_)_  _(_)_\e[0m\n"
                        "     \e[0;93m               / _) \e[0m       ┌───────────────────┐    ┌───────────────────┐      \e[0;91m(_)\e[0;93m@\e[0;91m(_)(_)\e[0;93m@\e[0;91m(_)\e[0m\n"
                        "     \e[0;93m      _.----._/ /   \e[0m       │ [3] LEADERBOARDS  │    │ [4] EXIT          │       \e[0;91m(_)\e[0;92m\\    \e[0;91m(_)\e[0;92m\\\e[0m    \n"
                        "     \e[0;93m     /         /    \e[0m       └───────────────────┘    └───────────────────┘      \e[0;91m{{}  \e[0;92m|  \e[0;91m{{}  \e[0;92m|  \e[0;91m{{}\e[0m\n"
                        "     \e[0;93m  __/ (  | (  |     \e[0m                                                           \e[0;92m~Y~(\\|/)~Y~(\\|/)~Y~\e[0m\n"
                        "     \e[0;93m /__.-'\\_\\-/_/ \\ \e[0m                                                              \e[0;92m\\|/ \\|/ \\|/ \\|/ \\|/\e[0m\n"
                        "\e[0;92m^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\e[0m\n"               
                        "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"   
                        "                                    \e[0mA CMSC 21 FINAL PROJECT BY JOHN & JHUN                                 \n"            
                        "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";

                        

char* title_screen2 =    "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                         "\n"
                         "      \n"                                                                                                       
                         "      \e[0;93m✧\e[0m                                          J&J PRESENTS                                          \e[0;93m✧\e[0m      \n"
                         "      \n" 
                         "                     \e[0;95m██████\e[0;92m╗ \e[0;95m██\e[0;92m╗   \e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m╗\e[0;95m███████\e[0;92m╗    \e[0;95m███\e[0;92m╗   \e[0;95m███\e[0;92m╗ \e[0;95m██████\e[0;92m╗ \e[0;95m██\e[0;92m╗  \e[0;95m██\e[0;92m╗ \e[0;95m██████\e[0;92m╗                     \n"
                         "          \e[0;93m✧\e[0m         \e[0;95m██\e[0;92m╔═══\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║╚══\e[0;95m███\e[0;92m╔╝    \e[0;95m████\e[0;92m╗ \e[0;95m████\e[0;92m║\e[0;95m██\e[0;92m╔═══\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m║ \e[0;95m██\e[0;92m╔╝\e[0;95m██\e[0;92m╔═══\e[0;95m██\e[0;92m╗         \e[0;93m✧\e[0m\n"
                         "                    \e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║  \e[0;95m███\e[0;92m╔╝     \e[0;95m██\e[0;92m╔\e[0;95m████\e[0;92m╔\e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║\e[0;95m█████\e[0;92m╔╝ \e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║                    \n"
                         "                    \e[0;95m██\e[0;92m║\e[0;95m▄▄ ██\e[0;92m║\e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║ \e[0;95m███\e[0;92m╔╝      \e[0;95m██\e[0;92m║╚\e[0;95m██\e[0;92m╔╝\e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m╔═\e[0;95m██\e[0;92m╗ \e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║                    \n"
                         "                    \e[0;92m╚\e[0;95m██████\e[0;92m╔╝╚\e[0;95m██████\e[0;92m╔╝\e[0;95m██\e[0;92m║\e[0;95m███████\e[0;92m╗    \e[0;95m██\e[0;92m║ ╚═╝ \e[0;95m██\e[0;92m║╚\e[0;95m██████\e[0;92m╔╝\e[0;95m██\e[0;92m║  \e[0;95m██\e[0;92m╗╚\e[0;95m██████\e[0;92m╔╝                    \n"
                         "                     \e[0;92m╚══\e[0;95m▀▀\e[0;92m═╝  ╚═════╝ ╚═╝╚══════╝    ╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═╝ ╚═════╝\e[0m \n"
                         "   \e[0;93m✧\e[0m                                                                                                     \e[0;93m✧\e[0m   \n"
                         "                                                                                                       \e[0;93m\"\e[0m \n"                                                                        
                         "                                ┌───────────────────┐    ┌───────────────────┐                       \e[0;93m(\\\e[0;93m|\e[0;93m/)\e[0m  \n"
                         "                                │ [1] START         │    │ [2] ABOUT         │              \e[0;91m_      _\e[0m  \e[0;93m⇂\e[0;93m|\e[0;93m⇃\e[0m\n"
                         "      \e[0;93m               __ \e[0m        └───────────────────┘    └───────────────────┘            \e[0;91m_(_)_  _(_)_\e[0m\n"
                         "      \e[0;93m              / _)\e[0m        ┌───────────────────┐    ┌───────────────────┐           \e[0;91m(_)\e[0;93m@\e[0;91m(_)(_)\e[0;93m@\e[0;91m(_)\e[0m\n"
                         "      \e[0;93m     _.----._/ /  \e[0m        │ [3] LEADERBOARDS  │    │ [4] EXIT          │            \e[0;92m/\e[0;91m(_)    \e[0;92m/\e[0;91m(_)\e[0m     \n"
                         "      \e[0;93m    /         /   \e[0m        └───────────────────┘    └───────────────────┘      \e[0;91m{{}  \e[0;92m|  \e[0;91m{{}  \e[0;92m|  \e[0;91m{{}\e[0m\n"
                         "      \e[0;93m __/ (  | (  |    \e[0m                                                            \e[0;92m~Y~(\\|/)~Y~(\\|/)~Y~\e[0m\n"
                         "      \e[0;93m/__.-/_/---\\_\\  \e[0m                                                              \e[0;92m\\|/ \\|/ \\|/ \\|/ \\|/\e[0m \n"
                         "\e[0;92m^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n"               
                         "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"   
                         "                                    \e[0mA CMSC 21 FINAL PROJECT BY JOHN & JHUN                               \n"            
                         "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";



char* leaderboards =    "\n"
                        "      \e[0;95m██\e[0;92m╗     \e[0;95m███████\e[0;92m╗ \e[0;95m█████\e[0;92m╗ \e[0;95m██████\e[0;92m╗ \e[0;95m███████\e[0;92m╗\e[0;95m██████\e[0;92m╗ \e[0;95m██████\e[0;92m╗  \e[0;95m██████\e[0;92m╗  \e[0;95m█████\e[0;92m╗ \e[0;95m██████\e[0;92m╗ \e[0;95m██████\e[0;92m╗ \e[0;95m███████\e[0;92m╗\n"
                        "      \e[0;95m██\e[0;92m║     \e[0;95m██\e[0;92m╔════╝\e[0;95m██\e[0;92m╔══\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m╔══\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m╔════╝\e[0;95m██\e[0;92m╔══\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m╔══\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m╔═══\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m╔══\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m╔══\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m╔══\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m╔════╝\n"
                        "      \e[0;95m██\e[0;92m║     \e[0;95m█████\e[0;92m╗  \e[0;95m███████\e[0;92m║\e[0;95m██\e[0;92m║  \e[0;95m██\e[0;92m║\e[0;95m█████\e[0;92m╗  \e[0;95m██████\e[0;92m╔╝\e[0;95m██████\e[0;92m╔╝\e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║\e[0;95m███████\e[0;92m║\e[0;95m██████\e[0;92m╔╝\e[0;95m██\e[0;92m║  \e[0;95m██\e[0;92m║\e[0;95m███████\e[0;92m╗\n"
                        "      \e[0;95m██\e[0;92m║     \e[0;95m██\e[0;92m╔══╝  \e[0;95m██\e[0;92m╔══\e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║  \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m╔══╝  \e[0;95m██\e[0;92m╔══\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m╔══\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m╔══\e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m╔══\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m║  \e[0;95m██\e[0;92m║╚════\e[0;95m██\e[0;92m║\n"
                        "      \e[0;95m███████\e[0;92m╗\e[0;95m███████\e[0;92m╗\e[0;95m██\e[0;92m║  \e[0;95m██\e[0;92m║\e[0;95m██████\e[0;92m╔╝\e[0;95m███████\e[0;92m╗\e[0;95m██\e[0;92m║  \e[0;95m██\e[0;92m║\e[0;95m██████\e[0;92m╔╝╚\e[0;95m██████\e[0;92m╔╝\e[0;95m██\e[0;92m║  \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║  \e[0;95m██\e[0;92m║\e[0;95m██████\e[0;92m╔╝\e[0;95m███████\e[0;92m║\n"
                        "      \e[0;92m╚══════╝╚══════╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝╚═════╝  ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ ╚══════╝\e[0m\n"
                        "\n";

char* welcome_screen1 =     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            " \n"                                                       
                            "                                                             #######\n"  
                            "                                                           #######  \n"  
                            "                                                          ######    \n"  
                            "                                                        ######      \n"  
                            "                                             ###       #####        \n"  
                            "                                             ####     ####          \n"  
                            "                                              ##### #####           \n"  
                            "                                               ########             \n"  
                            "                                                 #####              \n" 
                            " \n"
                            " \n" 
                            "                       ╔═*** *** *** ***** ** *****══════════════════════════════════╗\n"
                            "                       ║                                                             ║\n"
                            "                       ║                  HELLO, PLAYER %s                  ║\n"
                            "                       ║      I'M THE ONE WHO CAN'T BE WRONG, THE QUIZ MASTER.       ║\n"
                            "                       ║      AND I'M HERE TO SEE IF YOU HAVE WHAT IT TAKES TO       ║\n"
                            "                       ║                      CONQUER MY GAME.                       ║\n"
                            "                       ║                                                             ║\n"
                            "                       ╚═════════════════════════════════════════════════════════════╝\n"
                            "\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            "                                        PRESS [SPACEBAR] TO CONTINUE.\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";

char* welcome_screen2 =     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            " \n"                                        
                            "                                                             #######\n"  
                            "                                                           #######  \n"  
                            "                                                          ######    \n"  
                            "                                                        ######      \n"  
                            "                                             ###       #####        \n"  
                            "                                             ####     ####          \n"  
                            "                                              ##### #####           \n"  
                            "                                               ########             \n"  
                            "                                                 #####              \n"
                            " \n"
                            " \n" 
                            "                       ╔═THE ONE WHO CAN'T BE WRONG══════════════════════════════════╗\n"
                            "                       ║                                                             ║\n"
                            "                       ║                          HMMMM...                           ║\n"
                            "                       ║                                                             ║\n"
                            "                       ╚═════════════════════════════════════════════════════════════╝\n"
                            "\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            "                                        PRESS [SPACEBAR] TO CONTINUE.\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";

char* welcome_screen3 =     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            " \n"                                        
                            "                                                             #######\n"  
                            "                                                           #######  \n"  
                            "                                                          ######    \n"  
                            "                                                        ######      \n"  
                            "                                             ###       #####        \n"  
                            "                                             ####     ####          \n"  
                            "                                              ##### #####           \n"  
                            "                                               ########             \n"  
                            "                                                 #####              \n"
                            " \n"
                            " \n" 
                            "                       ╔═THE ONE WHO CAN'T BE WRONG══════════════════════════════════╗\n"
                            "                       ║                                                             ║\n"
                            "                       ║                          HMMMM...                           ║\n"
                            "                       ║                         NEVERMIND.                          ║\n"
                            "                       ║                                                             ║\n"
                            "                       ╚═════════════════════════════════════════════════════════════╝\n"
                            "\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            "                                        PRESS [SPACEBAR] TO CONTINUE.\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";

char* welcome_screen4 =     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            " \n"                                        
                            "                                                             #######\n"  
                            "                                                           #######  \n"  
                            "                                                          ######    \n"  
                            "                                                        ######      \n"  
                            "                                             ###       #####        \n"  
                            "                                             ####     ####          \n"  
                            "                                              ##### #####           \n"  
                            "                                               ########             \n"  
                            "                                                 #####              \n"
                            " \n"
                            " \n" 
                            "                       ╔═THE ONE WHO CAN'T BE WRONG══════════════════════════════════╗\n"
                            "                       ║                                                             ║\n"
                            "                       ║                          MOVING ON.                         ║\n"
                            "                       ║          BEFORE WE START, I WANT YOU TO READ ABOUT          ║\n"
                            "                       ║                     HOW MY GAME WORKS.                      ║\n"
                            "                       ║                                                             ║\n"
                            "                       ╚═════════════════════════════════════════════════════════════╝\n"
                            "\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            "                                        PRESS [SPACEBAR] TO CONTINUE.\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";

char* welcome_screen5 =     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            " \n"                                        
                            "                                                             #######\n"  
                            "                                                           #######  \n"  
                            "                                                          ######    \n"  
                            "                                                        ######      \n"  
                            "                                             ###       #####        \n"  
                            "                                             ####     ####          \n"  
                            "                                              ##### #####           \n"  
                            "                                               ########             \n"  
                            "                                                 #####              \n"
                            " \n"
                            " \n" 
                            "                       ╔═THE ONE WHO CAN'T BE WRONG══════════════════════════════════╗\n"
                            "                       ║                                                             ║\n"
                            "                       ║                HMM... YOU'VE READ QUITE FAST                ║\n"
                            "                       ║                                                             ║\n"
                            "                       ╚═════════════════════════════════════════════════════════════╝\n"
                            "\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            "                                        [1] CONTINUE    [2] READ AGAIN \n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";

char* welcome_screen6 =     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            " \n"                                        
                            "                                                             #######\n"  
                            "                                                           #######  \n"  
                            "                                                          ######    \n"  
                            "                                                        ######      \n"  
                            "                                             ###       #####        \n"  
                            "                                             ####     ####          \n"  
                            "                                              ##### #####           \n"  
                            "                                               ########             \n"  
                            "                                                 #####              \n"
                            " \n"
                            " \n" 
                            "                       ╔═THE ONE WHO CAN'T BE WRONG══════════════════════════════════╗\n"
                            "                       ║                                                             ║\n"
                            "                       ║           YOU ARE NOW READY TO TAKE MY CHALLENGE.           ║\n"
                            "                       ║   UNLOCK YOUR POTENTIAL AND BEAT ALL OF MY TOP 3 SCORERS.   ║\n"
                            "                       ║                          GOODLUCK!                          ║\n"
                            "                       ║                                                             ║\n"
                            "                       ╚═════════════════════════════════════════════════════════════╝\n"
                            "\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            "                                        PRESS [SPACEBAR] TO CONTINUE.\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";


char* easy_remark1 =     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            " \n"                                        
                            "                                                             #######\n"  
                            "                                                           #######  \n"  
                            "                                                          ######    \n"  
                            "                                                        ######      \n"  
                            "                                             ###       #####        \n"  
                            "                                             ####     ####          \n"  
                            "                                              ##### #####           \n"  
                            "                                               ########             \n"  
                            "                                                 #####              \n"
                            " \n"
                            " \n" 
                            "                       ╔═THE ONE WHO CAN'T BE WRONG══════════════════════════════════╗\n"
                            "                       ║                                                             ║\n"
                            "                       ║  SINCE YOU ARE BEGINNER, I'LL START AN EASY ROUND FOR YOU   ║\n"
                            "                       ║                                                             ║\n"
                            "                       ╚═════════════════════════════════════════════════════════════╝\n"
                            "\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            "                                        PRESS [SPACEBAR] TO CONTINUE.\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";

char* show_category =     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            " \n"                                        
                            "                                                             #######\n"  
                            "                                                           #######  \n"  
                            "                                                          ######    \n"  
                            "                                                        ######      \n"  
                            "                                             ###       #####        \n"  
                            "                                             ####     ####          \n"  
                            "                                              ##### #####           \n"  
                            "                                               ########             \n"  
                            "                                                 #####              \n"
                            " \n"
                            " \n" 
                            "                       ╔═THE ONE WHO CAN'T BE WRONG══════════════════════════════════╗\n"
                            "                       ║                                                             ║\n"
                            "                       ║                CHOOSE A CATEGORY - %s║\n"
                            "                       ║            [1] %s                       ║\n"
                            "                       ║            [2] %s                       ║\n"
                            "                       ║            [3] %s                       ║\n"
                            "                       ║                                                             ║\n"
                            "                       ╚═════════════════════════════════════════════════════════════╝\n"
                            "\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            "                                              CHOOSE A CATEGORY.\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";

char* easy_remark2 =     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            " \n"                                        
                            "                                                             #######\n"  
                            "                                                           #######  \n"  
                            "                                                          ######    \n"  
                            "                                                        ######      \n"  
                            "                                             ###       #####        \n"  
                            "                                             ####     ####          \n"  
                            "                                              ##### #####           \n"  
                            "                                               ########             \n"  
                            "                                                 #####              \n"
                            " \n"
                            " \n" 
                            "                       ╔═THE ONE WHO CAN'T BE WRONG══════════════════════════════════╗\n"
                            "                       ║                                                             ║\n"
                            "                       ║          WOAAHHH! IS THIS REALLY YOUR FIRST TIME?           ║\n"
                            "                       ║      I THINK YOU ARE NO BEGINNER AT ALL. LET'S PROCEED      ║\n"
                            "                       ║      TO THE NEXT CATEGORY AND TEST THAT LUCK OF YOURS.      ║\n"
                            "                       ║                                                             ║\n"
                            "                       ╚═════════════════════════════════════════════════════════════╝\n"
                            "\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            "                                        PRESS [SPACEBAR] TO CONTINUE.\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";

char* average_remark1 =     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            " \n"                                        
                            "                                                             #######\n"  
                            "                                                           #######  \n"  
                            "                                                          ######    \n"  
                            "                                                        ######      \n"  
                            "                                             ###       #####        \n"  
                            "                                             ####     ####          \n"  
                            "                                              ##### #####           \n"  
                            "                                               ########             \n"  
                            "                                                 #####              \n"
                            " \n"
                            " \n" 
                            "                       ╔═THE ONE WHO CAN'T BE WRONG══════════════════════════════════╗\n"
                            "                       ║                                                             ║\n"
                            "                       ║        CONGRATULATIONS ON REACHING A SCORE OF 2000!         ║\n"
                            "                       ║             I GUESS LADY'S LUCK ON YOUR SIDE :)             ║\n"
                            "                       ║                                                             ║\n"
                            "                       ╚═════════════════════════════════════════════════════════════╝\n"
                            "\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            "                                        PRESS [SPACEBAR] TO CONTINUE.\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";

char* average_remark2 =     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            " \n"                                        
                            "                                                             #######\n"  
                            "                                                           #######  \n"  
                            "                                                          ######    \n"  
                            "                                                        ######      \n"  
                            "                                             ###       #####        \n"  
                            "                                             ####     ####          \n"  
                            "                                              ##### #####           \n"  
                            "                                               ########             \n"  
                            "                                                 #####              \n"
                            " \n"
                            " \n" 
                            "                       ╔═THE ONE WHO CAN'T BE WRONG══════════════════════════════════╗\n"
                            "                       ║                                                             ║\n"
                            "                       ║    AS A REWARD FOR YOUR WIT, I'LL ADJUST THE FOLLOWING      ║\n"
                            "                       ║                   LEVEL : EASY -> AVERAGE                   ║\n"
                            "                       ║                   TIMER : 30   -> 45                        ║\n"
                            "                       ║                                                             ║\n"
                            "                       ╚═════════════════════════════════════════════════════════════╝\n"
                            "\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            "                                        PRESS [SPACEBAR] TO CONTINUE.\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";

char* average_remark3 =     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            " \n"                                        
                            "                                                             #######\n"  
                            "                                                           #######  \n"  
                            "                                                          ######    \n"  
                            "                                                        ######      \n"  
                            "                                             ###       #####        \n"  
                            "                                             ####     ####          \n"  
                            "                                              ##### #####           \n"  
                            "                                               ########             \n"  
                            "                                                 #####              \n"
                            " \n"
                            " \n" 
                            "                       ╔═THE ONE WHO CAN'T BE WRONG══════════════════════════════════╗\n"
                            "                       ║                                                             ║\n"
                            "                       ║          YOU REALLY WANT TO DOMINATE MY GAME, HUH?          ║\n"
                            "                       ║      LET'S PROCEED TO THE NEXT CATEGORY AND SEE IF YOU      ║\n"
                            "                       ║                       CAN STILL SMILE.                      ║\n"
                            "                       ║                                                             ║\n"
                            "                       ╚═════════════════════════════════════════════════════════════╝\n"
                            "\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            "                                        PRESS [SPACEBAR] TO CONTINUE.\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";

char* difficult_remark1 =     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            " \n"                                        
                            "                                                             #######\n"  
                            "                                                           #######  \n"  
                            "                                                          ######    \n"  
                            "                                                        ######      \n"  
                            "                                             ###       #####        \n"  
                            "                                             ####     ####          \n"  
                            "                                              ##### #####           \n"  
                            "                                               ########             \n"  
                            "                                                 #####              \n"
                            " \n"
                            " \n" 
                            "                       ╔═THE ONE WHO CAN'T BE WRONG══════════════════════════════════╗\n"
                            "                       ║                                                             ║\n"
                            "                       ║        CONGRATULATIONS ON REACHING A SCORE OF 6000!         ║\n"
                            "                       ║        I NEVER THOUGHT YOU CAN GO THIS FAR. MY BAD.         ║\n"
                            "                       ║                                                             ║\n"
                            "                       ╚═════════════════════════════════════════════════════════════╝\n"
                            "\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            "                                        PRESS [SPACEBAR] TO CONTINUE.\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";


char* difficult_remark2 =     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            " \n"                                        
                            "                                                             #######\n"  
                            "                                                           #######  \n"  
                            "                                                          ######    \n"  
                            "                                                        ######      \n"  
                            "                                             ###       #####        \n"  
                            "                                             ####     ####          \n"  
                            "                                              ##### #####           \n"  
                            "                                               ########             \n"  
                            "                                                 #####              \n"
                            " \n"
                            " \n" 
                            "                       ╔═THE ONE WHO CAN'T BE WRONG══════════════════════════════════╗\n"
                            "                       ║                                                             ║\n"
                            "                       ║    AS A REWARD FOR YOUR WIT, I'LL ADJUST THE FOLLOWING      ║\n"
                            "                       ║                   LEVEL : AVERAGE -> DIFFICULT              ║\n"
                            "                       ║                   TIMER : 45      -> 60                     ║\n"
                            "                       ║                                                             ║\n"
                            "                       ╚═════════════════════════════════════════════════════════════╝\n"
                            "\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            "                                        PRESS [SPACEBAR] TO CONTINUE.\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";

char* difficult_remark3 =     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            " \n"                                        
                            "                                                             #######\n"  
                            "                                                           #######  \n"  
                            "                                                          ######    \n"  
                            "                                                        ######      \n"  
                            "                                             ###       #####        \n"  
                            "                                             ####     ####          \n"  
                            "                                              ##### #####           \n"  
                            "                                               ########             \n"  
                            "                                                 #####              \n"
                            " \n"
                            " \n" 
                            "                       ╔═THE ONE WHO CAN'T BE WRONG══════════════════════════════════╗\n"
                            "                       ║                                                             ║\n"
                            "                       ║    ONCE YOU YOU'VE REACHED 17,500. I'LL RECOGNISE YOU AS    ║\n"
                            "                       ║                   A NEW WINNER. HANG IN THERE!              ║\n"
                            "                       ║                                                             ║\n"
                            "                       ╚═════════════════════════════════════════════════════════════╝\n"
                            "\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                            "                                        PRESS [SPACEBAR] TO CONTINUE.\n"
                            "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";

char* lose_screen = "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                    "\n"
                    "\n"
                    "                    \e[0;95m██████\e[0;92m╗  \e[0;95m█████\e[0;92m╗ \e[0;95m███\e[0;92m╗   \e[0;95m███\e[0;92m╗\e[0;95m███████\e[0;92m╗ \e[0;95m██████\e[0;92m╗ \e[0;95m██\e[0;92m╗   \e[0;95m██\e[0;92m╗\e[0;95m███████\e[0;92m╗\e[0;95m██████\e[0;92m╗ \e[0;95m██\e[0;92m╗\n"
                    "                   \e[0;95m██\e[0;92m╔════╝ \e[0;95m██\e[0;92m╔══\e[0;95m██\e[0;92m╗\e[0;95m████\e[0;92m╗ \e[0;95m████\e[0;92m║\e[0;95m██\e[0;92m╔════╝\e[0;95m██\e[0;92m╔═══\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m╔════╝\e[0;95m██\e[0;92m╔══\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m║\n"
                    "                   \e[0;95m██\e[0;92m║  \e[0;95m███\e[0;92m╗\e[0;95m███████\e[0;92m║\e[0;95m██\e[0;92m╔\e[0;95m████\e[0;92m╔\e[0;95m██\e[0;92m║\e[0;95m█████\e[0;92m╗  \e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║\e[0;95m█████\e[0;92m╗  \e[0;95m██████\e[0;92m╔╝\e[0;95m██\e[0;92m║\n"
                    "                   \e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m╔══\e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║╚\e[0;95m██\e[0;92m╔╝\e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m╔══╝  \e[0;95m██\e[0;92m║   \e[0;95m██\e[0;92m║╚\e[0;95m██\e[0;92m╗ \e[0;95m██\e[0;92m╔╝\e[0;95m██\e[0;92m╔══╝  \e[0;95m██\e[0;92m╔══\e[0;95m██\e[0;92m╗╚═╝\n"
                    "                   \e[0;92m╚\e[0;95m██████\e[0;92m╔╝\e[0;95m██\e[0;92m║  \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║ ╚═╝ \e[0;95m██\e[0;92m║\e[0;95m███████\e[0;92m╗╚\e[0;95m██████\e[0;92m╔╝ ╚\e[0;95m████\e[0;92m╔╝ \e[0;95m███████\e[0;92m╗\e[0;95m██\e[0;92m║  \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m╗\n"
                    "                    \e[0;92m╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝ ╚═════╝   ╚═══╝  ╚══════╝╚═╝  ╚═╝╚═╝\e[0m\n"
                    "\n"
                    "\n"
                    "                              \"SCORES DOES NOT DEFINE WHO WE ARE. JUST GIVE UP.\"\n\n"
                    "                         --SUMMARY---------------------------------------------\n"
                    "                           NAME    :   %s\n"
                    "                           SCORE   :   %d\n"
                    "                           RANKING :   %d / %d\n"
                    "                         ------------------------------------------------------\n"
                    "\n"
                    "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                    "                                        [1] ISA PA     [2] SUKO NA\n"
                    "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";

char* win_screen = "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                    "\n"
                    "\n"
                    "                           \e[0;95m██\e[0;92m╗    \e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m╗\e[0;95m███\e[0;92m╗   \e[0;95m██\e[0;92m╗\e[0;95m███\e[0;92m╗   \e[0;95m██\e[0;92m╗\e[0;95m███████\e[0;92m╗\e[0;95m██████\e[0;92m╗ \e[0;95m██\e[0;92m╗\n"
                    "                           \e[0;95m██\e[0;92m║    \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║\e[0;95m████\e[0;92m╗  \e[0;95m██\e[0;92m║\e[0;95m████\e[0;92m╗  \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m╔════╝\e[0;95m██\e[0;92m╔══\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m║\n"
                    "                           \e[0;95m██\e[0;92m║ \e[0;95m█\e[0;92m╗ \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m╔\e[0;95m██\e[0;92m╗ \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m╔\e[0;95m██\e[0;92m╗ \e[0;95m██\e[0;92m║\e[0;95m█████\e[0;92m╗  \e[0;95m██████\e[0;92m╔╝\e[0;95m██\e[0;92m║\n"
                    "                           \e[0;95m██\e[0;92m║\e[0;95m███\e[0;92m╗\e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║╚\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║╚\e[0;95m██\e[0;92m╗\e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m╔══╝  \e[0;95m██\e[0;92m╔══\e[0;95m██\e[0;92m╗╚═╝\n"
                    "                           \e[0;92m╚\e[0;95m███\e[0;92m╔\e[0;95m███\e[0;92m╔╝\e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m║ ╚\e[0;95m████\e[0;92m║\e[0;95m██\e[0;92m║ ╚\e[0;95m████\e[0;92m║\e[0;95m███████\e[0;92m╗\e[0;95m██\e[0;92m║  \e[0;95m██\e[0;92m║\e[0;95m██\e[0;92m╗\n"
                    "                           \e[0;92m ╚══╝╚══╝ ╚═╝╚═╝  ╚═══╝╚═╝  ╚═══╝╚══════╝╚═╝  ╚═╝╚═╝\e[0m\n"
                    "\n"
                    "                                           \"WELL DONE. WINNERRRRR!\"\n"
                    "                         --SUMMARY---------------------------------------------\n"
                    "                           NAME    :   %s\n"
                    "                           SCORE   :   %d\n"
                    "                           RANKING :   %d / %d\n"
                    "                         ------------------------------------------------------\n"
                    "\n"
                    "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                    "                                        [1] ISA PA    [2] AYAW NA\n"
                    "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";  

char* about_screen1 = "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                     "                                                                                                              \n"
                     "                                                                                                              \n"
                     "                                                 HEY Y'ALL                                                    \n"
                     "                         THIS GAME WAS MADE BY LESTER AND MERL AS PROJECT IN CMSC 21                          \n"
                     "                                                                                                              \n"
                     "                                                                                                              \n"
                     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                     "                                        PRESS [SPACEBAR] TO CONTINUE.\n"
                     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";

char* no_score_leaderboard =    "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                                "                                                                                                              \n"
                                "                           NO ONE PLAYED SO FAR. PLAY NOW TO BE GAME FIRST RANKER.                            \n"
                                "                                                                                                              \n"
                                "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                                "                                        PRESS [SPACEBAR] TO CONTINUE.\n"
                                "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";


                     

char* about_screen2 = "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                     "                                                                                                              \n"
                     "                                           WHAT IS THIS ALL ABOUT?                                            \n"
                     "\n"
                     "        ╔════════════════════════════════════════════════════════════════════════════════════════════╗        \n"
                     "        ║                                                                                            ║        \n"
                     "        ║    QUIZ MO KO CHALLENGES YOUR KNOWLEDGE AND INTUITION WITH MULTIPLE CHOICES ACROSS         ║        \n"
                     "        ║    VARIOUS CATEGORIES. AND GUESS WHAT? YOU'RE NOT ALONE IN THIS. A ZEALOUS GAME MASTER     ║        \n"
                     "        ║    IS ALWAYS THERE TO WELCOME YOU AND KEEP THE ENERGY HIGH THROUGHOUT THE GAME.            ║        \n"
                     "        ║                                                                                            ║        \n"
                     "        ╚════════════════════════════════════════════════════════════════════════════════════════════╝        \n"
                     "                                                                                                              \n"
                     "                                                                                                              \n"
                     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                     "                                        PRESS [SPACEBAR] TO CONTINUE.\n"
                     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";

char* mechanics1 =  "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                     "                                                                                                              \n"
                     "                                              GENERAL MECHANICS                                               \n"
                     "                                                                                                              \n"
                     "        ╔════════════════════════════════════════════════════════════════════════════════════════════╗        \n"
                     "        ║                                                                                            ║        \n"
                     "        ║    1) THE GAME SHALL CONSIST OF THREE CONSECUTIVE LEVELS: EASY, AVERAGE, AND DIFFICULT.    ║        \n"
                     "        ║    2) AT THE START OF THE GAME, THE PLAYERS ARE ASKED TO INPUT THEIR NAME. THIS WILL BE    ║        \n"
                     "        ║       USE TO RECOGNIZE ACHIEVEMENT OF THE PLAYERS AT THE END OF THE GAME.                  ║        \n"
                     "        ║    3) AFTER THE PLAYER INPUT THERE NAME, THEY WILL BE GIVEN THREE RANDOM CATEGORIES TO     ║        \n"
                     "        ║       TO CHOOSE AT. AS OF NOW, THE  ONLY CATEGORY AVAILABLE ARE: MATHEMATICS, SCIENCE,     ║        \n"
                     "        ║       COMPUTER SCIENCE, GEOGRAPHY, AND HISTORY AND LITERATURE. FOR EACH CATEGORY THERE     ║        \n"
                     "        ║       EXISTS TEN MULTIPLE QUESTIONS THAT WILL BE ANSWERED BY THE PLAYERS.                  ║        \n"
                     "        ║    4) BY DEFAULT, THE PLAYER WILL HAVE THREE LIVES AND WILL BE BE GIVEN A THREE RANDOM     ║        \n"
                     "        ║       POWER-UPS THAT WILL HELP THEM THROUGHOUT THE GAME.                                   ║        \n"
                     "        ║                                                                                            ║        \n"
                     "        ╚════════════════════════════════════════════════════════════════════════════════════════════╝        \n"
                     "                                                                                                              \n"
                     "                                                                                                              \n"
                     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                     "                                        PRESS [SPACEBAR] TO CONTINUE.\n"
                     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";

char* mechanics2 =  "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                     "                                                                                                              \n"
                     "                                              GENERAL MECHANICS                                               \n"
                     "                                                                                                              \n"
                     "        ╔════════════════════════════════════════════════════════════════════════════════════════════╗        \n"
                     "        ║                                                                                            ║        \n"
                     "        ║    5) AFTER CHOOSING A CATEGORY, THE QUESTIONS UNDER EACH CATEGORY WILL BE PRESENTED,      ║        \n"
                     "        ║       AND THEY WILL PRESS THEIR ANSWERS FROM A TO D.                                       ║        \n"
                     "        ║    6) EACH CORRECT ANSWER WILL BE AWARDED WITH CORRESPONDING POINTS AND                    ║        \n"
                     "        ║       ANSWERING INCORRECTLY WILL RESULT IN LOSING A LIFE. EACH QUESTION SHALL BE TIMED,    ║        \n"
                     "        ║       AND THE LESS TIME CONSUMED BY THE PLAYER, THE HIGHER THE POINTS WILL BE GIVEN.       ║        \n"
                     "        ║    7) AFTER GARNERING A TOTAL OF 2000 POINTS AT THE EASY ROUND, THE PLAYER WILL NOW        ║        \n"
                     "        ║       MOVE TO THE AVERAGE ROUND. AFTER GARNERING A TOTAL OF 6500 POINTS, THE PLAYER        ║        \n"
                     "        ║       WILL THEN BE ABLE TO LEVEL UP TO THE DIFFICULT ROUND. FINALLY, THE PLAYER WINS       ║        \n"
                     "        ║       ONCE THEY ACHIEVE A TOTAL OF 17500 POINTS.                                           ║        \n"
                     "        ║                                                                                            ║        \n"
                     "        ╚════════════════════════════════════════════════════════════════════════════════════════════╝        \n"
                     "                                                                                                              \n"
                     "                                                                                                              \n"
                     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                     "                                        PRESS [SPACEBAR] TO CONTINUE.\n"
                     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";

char* mechanics3 =  "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                     "                                                                                                              \n"
                     "                                          GET TO KNOW YOUR POWER-UPS                                          \n"
                     "                                                                                                              \n"
                     "        ╔════════════════════════════════════════════════════════════════════════════════════════════╗        \n"
                     "        ║                                                                                            ║        \n"
                     "        ║    THESE POWER-UPS WILL GIVE YOU THE UPPER HAND IN WINNING THE GAME. USE THEM WISELY!      ║        \n"
                     "        ║                                                                                            ║        \n"
                     "        ║    TIME FREEZE     - THIS POWER-UP ALLOWS THE PLYER TO PAUSE THE TIMER OF THE GAME AND     ║        \n"
                     "        ║                      ALLOWS THE PLAYER TO ANSWER THE QUESTIONS WITH EASE.                  ║        \n"
                     "        ║    ERASER          - THIS POWER-UP ELIMINATES ONE WRONG OPTION AMOUNG THE CHOICES.         ║        \n"
                     "        ║    DOUBLE JEOPARDY - THIS POWER-UP DOUBLES THE PLAYER'S SCORE IF THE QUESTION IS           ║        \n"
                     "        ║                      ANSWERED INCORRECTLY.                                                 ║        \n"
                     "        ║    PASS            - THIS ALLOWS PLAYER TO PASS AND ANSWER THE QUESTION LATER.             ║        \n"
                     "        ║    IMMUNITY        - THIS ALLOWS THE PLAYER TO ANSWER THE SAME QUESTION TWICE WITHOUT      ║        \n"
                     "        ║                      LOSING A LIFE IN THE CASE OF A WRONG ANSWER.                          ║        \n"
                     "        ║                                                                                            ║        \n"
                     "        ╚════════════════════════════════════════════════════════════════════════════════════════════╝        \n"
                     "                                                                                                              \n"
                     "                                                                                                              \n"
                     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n"
                     "                                        PRESS [SPACEBAR] TO CONTINUE.\n"
                     "\e[0;94m══════════════════════════════════════════════════════════════════════════════════════════════════════════════\e[0m\n";
                   


