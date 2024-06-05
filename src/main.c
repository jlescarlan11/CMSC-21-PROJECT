#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#include "game.h"

int main() {

    SetConsoleOutputCP(CP_UTF8);
    srand(time(NULL));

    Game game;
    Score* scores = NULL;


    init_contents(&game);

    int c_index[MAX_CATEGORIES];
    SLList myList;
    

    start:
    init_list(&myList);
    clear_screen();
    char input = '0';
    while(1) {
    

        int ctr = 0;
        while (1) {
            clear_screen();
            if ((ctr % 28) == 0) {
                PlaySound(TEXT("assets/intro.wav"), 
                        NULL, 
                        SND_FILENAME | SND_ASYNC);

                ctr = 1;
            }
            if ((ctr % 2) == 0) {
                puts(title_screen1);
            } else {
                puts(title_screen2);
            }
            ctr++;
            if (kbhit()) {
                input = getch();
            }
            if ((input >= '1') && (input <= '4')) {
                break;
            }
            Sleep(900);
        }
    
        int pow_index[MAX_POWERUPS];
        init_index(pow_index, MAX_POWERUPS, TOTAL_POWERUPS, 1);


        switch (input)
        {
        case '1':
            clear_screen();
            printf("\n");
            PlaySound(TEXT("assets/init.wav"), NULL, SND_FILENAME | SND_ASYNC);
            center_text("DO YOU REALLY WANT TO PLAY?\n", 110);
            center_text("[1] YES    [2] NO\n",110);

            input = '0';
            do {
                input = getch();
            } while ((input != '1') && (input != '2'));

            if (input == '2') {
                goto start;
            }
            play:
            if (reset_flg) {
                clear_list(&myList);
                c_flg1 = 0;
                c_flg2 = 0; 
                c_flg3 = 0;
            }
            
            do {
                PlaySound(TEXT("assets/init.wav"), NULL, SND_FILENAME | SND_ASYNC);
                init_player(&game);
                confirm_name(&game);
                input = '0';
                do {
                    input = getch();
                } while ((input != '1') && (input != '2'));

            } while (input == '2');

            clear_screen();
            PlaySound(TEXT("assets/init.wav"), NULL, SND_FILENAME | SND_ASYNC);

            init_welcome_remarks(&game);
            

            while (1) {     

                int que_index[MAX_QUESTIONS];
                init_index(que_index, 
                            MAX_QUESTIONS, 
                            TOTAL_QUESTIONS, 1);

                init_difficulty(&game);

                PlaySound(TEXT("assets/init.wav"), 
                                NULL, 
                                SND_FILENAME | SND_ASYNC);


                
                if ((game.difficulty == EASY) && (!c_flg1)) {
                    init_index(c_index, MAX_CATEGORIES, TOTAL_CATEGORIES, 10);
                    populate_list(&myList, c_index, game.quizzes, 5, 0);
                    c_flg1 = 1;
                } else if ((game.difficulty == AVERAGE) && (!c_flg2)) {
                    clear_list(&myList);
                    init_index(c_index, MAX_CATEGORIES, TOTAL_CATEGORIES, 10);
                    populate_list(&myList, c_index, game.quizzes, 5, 0);
                    c_flg2 = 1;
                } else if ((game.difficulty == DIFFICULT) && (!c_flg3)) {
                    clear_list(&myList);
                    init_index(c_index, MAX_CATEGORIES, TOTAL_CATEGORIES, 10);
                    populate_list(&myList, c_index, game.quizzes, 5, 0);
                    c_flg3 = 1;
                }

                
                
                
                
                int cat_index = choose_category(&game, &myList, c_index);

                mark_category_used(cat_index, &game);

                play_quiz(&game, cat_index, 
                            que_index, 
                            pow_index, 
                            used_powerup);

                

                if (game.player.lives == 0) {
                    write_score(game.player.name, game.player.score);

                    int num_scores = 0;
                    read_score(&scores, &num_scores);
                    PlaySound(TEXT("assets/lose.wav"), NULL, SND_FILENAME | SND_ASYNC);
                

                    for (int j = 0; j < num_scores; j++) {
                        if ((strcmp(scores[j].name, game.player.name) == 0) && (scores[j].score == game.player.score)) {
                            clear_screen();
                            printf(lose_screen, game.player.name, game.player.score, j+1, num_scores);
                        }
                    }

                    input; 
                    do {
                        input = getch();
                    } while ((input != '1') && (input != '2'));

                    free(scores);
                    reset_flg = 1;
                    if(input == '1') {                        
                        goto play;
                    } else {
                        goto start;
                    }

                } else if (game.player.score >= 17500) {
                    PlaySound(TEXT("assets/win.wav"), NULL, SND_FILENAME | SND_ASYNC);
                    write_score(game.player.name, game.player.score);

                    int num_scores = 0;
                    read_score(&scores, &num_scores);

                    for (int j = 0; j < num_scores; j++) {
                        if ((strcmp(scores[j].name, game.player.name) == 0) && (scores[j].score == game.player.score)) {
                            clear_screen();
                            printf(win_screen, game.player.name, game.player.score, j+1, num_scores);
                            reset_flg = 1;
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
                PlaySound(TEXT("assets/play.wav"), NULL, SND_FILENAME | SND_ASYNC);
                read_again:
                clear_screen();
                puts(about_screen1);
                press_continue();
                puts(about_screen2);
                press_continue();
                puts(mechanics1);
                press_continue();
                puts(mechanics2);
                press_continue();
                puts(mechanics3);
                press_continue();
                goto start;
                
            break;



            case '3':
                PlaySound(TEXT("assets/leaderboard.wav"), 
                            NULL, 
                            SND_FILENAME | SND_ASYNC);
                clear_screen();

                int num_scores = 0;
                read_score(&scores, &num_scores);
                if (num_scores != 0) {
                    print_top_scores(scores, num_scores);
                } else {
                    puts(no_score_leaderboard);
                }
                
            
                do {
                    input = getch();
                } while (input!= '1');

                free(scores);

                goto start;
            break;

            case '4':
                clear_screen();
                return 0;
                break;
            
            default:
                goto start;
                break;
        }
        
    }

    return 0;
}