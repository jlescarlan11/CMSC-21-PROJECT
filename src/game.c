#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>

#include "game.h"
#include "graphics.h"

int main() {
    srand(time(NULL));

    Game game;
    Score* scores = NULL;

    init_contents(&game);

    while(1) {
        start:
        system("cls");

        puts(title_screen);

        char input;
        do {
            input = getch();
        } while ((input < '1') && (input > '4'));

        int pow_index[MAX_POWERUPS];
        init_index(pow_index, MAX_POWERUPS, TOTAL_POWERUPS, 1);


        switch (input)
        {
        case '1':
            system("cls");
            center_item(29);
            printf("DO YOU REALLY WANT TO PLAY?\n\n");
            center_item(19);
            printf("[1] YES    [2] NO\n");

            input = '0';
            do {
                input = getch();
            } while ((input != '1') && (input != '2'));

            if (input == '2') {
                goto start;
            }
            play:
            do {
                init_player(&game);
                input = '0';
                do {
                    confirm_name(&game);
                    input = getch();
                } while (input != '1' && input != '2');

            } while (input == '2');

            system("cls");
            printf(welcome_screen, game.player.name);

            input = '0';
            do {
                input = getch();
            } while ((input != '2') && (input != '1'));

            if (input == '2') {
                goto start;
            }

            while (1) {     

                int que_index[MAX_QUESTIONS];
                init_index(que_index, MAX_QUESTIONS, TOTAL_QUESTIONS, 1);

                init_difficulty(&game);


                int cat_index = choose_category(&game);

                play_quiz(&game, cat_index, que_index, pow_index, used_powerup);

                if (game.player.lives == 0) {
                    writeScore(game.player.name, game.player.score);

                    

                    int num_scores = 0;
                    readScore(&scores, &num_scores);
                    sortScores(scores, num_scores);

                    for (int j = 0; j < num_scores; j++) {
                        if ((strcmp(scores[j].name, game.player.name) == 0) && (scores[j].score == game.player.score)) {
                            system("cls");
                            printf(lose_screen, game.player.name, game.player.score, j+1, num_scores);
                        }
                    }

                    input; 
                    do {
                        input = getch();
                    } while ((input != '1') && (input != '2'));

                    free(scores);

                    if(input == '1') {
                        goto play;
                    } else {
                        goto start;
                    }

                }
            }

            break;

            case '2':
                system("cls");
                puts(about_screen);
            
                do {
                    input = getch();
                } while (input != '1');

                goto start;
            break;

            case '3':
                system("cls");

                int num_scores = 0;
                readScore(&scores, &num_scores);
                sortScores(scores, num_scores);
                printTopScores(scores, num_scores, 3);
            
                do {
                    input = getch();
                } while (input!= '1');

                free(scores);

                goto start;
            break;

            case '4':
                system("cls");
                return 0;
                break;
            
            default:
                goto start;
                break;
        }
        
    }

    return 0;
}