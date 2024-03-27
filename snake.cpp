#include "console.h"
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace console;

#define BOARD_SIZE 20
#define MOVE_DELAY 15
#define WALL_VERTICAL_STRING "┃"
#define WALL_HORIZONTAL_STRING "━"
#define WALL_RIGHT_TOP_STRING "┓"
#define WALL_LEFT_TOP_STRING "┏"
#define WALL_RIGHT_BOTTOM_STRING "┛"
#define WALL_LEFT_BOTTOM_STRING "┗"
#define SNAKE_STRING "■"
#define APPLE_STRING "●"

struct snake_xy { int x, y; }; // 뱀의 몸 한 칸의 좌표를 저장하기 위한 구조체

void game(void);
void new_apple(int*, int*, snake_xy[], int snake_length);
void handle_input(char, char*);
void move(snake_xy[], snake_xy[2], int);
void draw_game(snake_xy[], int, int, int);

int main(void)
{
    init();
    srand((time(NULL)));
    while(true)
    {
        game();
    }
}

void game(void)
{
    snake_xy snake[(BOARD_SIZE - 2) * (BOARD_SIZE - 2)]; // 뱀의 몸 한 칸씩의 배열
    if (BOARD_SIZE == 4)
    {
        snake[0] = {1, 1};
    } else
    {
        snake[0] = {BOARD_SIZE / 2, BOARD_SIZE / 2};
    }
    snake_xy tmp[2] = {}; // 뱀이 이동할 때에 원래 몸의 좌표를 저장하기 위한 공간
    int head_index = 0; // 뱀 머리의 인덱스, 또는 (뱀 길이 - 1), 또는 (점수 / 10)

    char prev_dir = 'R'; // 뱀의 이전 이동 방향. U(위), D(아래), L(왼쪽), R(오른쪽) 값을 가지도록 함, 초기값 = R
    char next_dir = 'R'; // 뱀의 다음 이동 방향. U(위), D(아래), L(왼쪽), R(오른쪽) 값을 가지도록 함, 초기값 = R
    
    int apple_x; // 사과의 x좌표
    int apple_y; // 사과의 y좌표
    new_apple(&apple_x, &apple_y, snake, head_index + 1); // 사과 위치 초기화

    bool win = false;
    bool lose = false;
    
    clear();
    draw_game(snake, head_index, apple_x, apple_y); // 처음에 모든 요소를 그려주고 루프로 들어간다

    for (int i = 0; i < MOVE_DELAY; i++) // 처음에 입력을 받고 루프로 들어간다
    {
        handle_input(prev_dir, &next_dir);
        wait();
    }
    
    while (win == false && lose == false) // 승리 혹은 패배 시 탈출, ESC 누르면 프로그램 전체 종료
    {
        clear(); // 이전 프레임 클리어

        switch (next_dir)
        {
            case 'U': // 방향이 위 일 때
                if (snake[head_index].y == 1) // 벽에 닿을 경우 체크
                {
                    lose = true;
                    break;
                }
                for (int i = 1; i < head_index; i++) // 자기 몸에 닿을 경우 체크(꼬리는 한칸 이동하면 안닿으므로 제외)
                {
                    if (snake[head_index].x == snake[i].x && snake[head_index].y == snake[i].y + 1)
                    {
                        lose = true;
                        break;
                    }
                }
                if (snake[head_index].x == apple_x && snake[head_index].y == apple_y + 1) // 사과를 먹을 경우
                {
                    // 사과의 위치로 머리통 하나를 더 붙이고 그것을 머리로 한다.
                    head_index++; 
                    snake[head_index].x = apple_x;
                    snake[head_index].y = apple_y;
                    if ((BOARD_SIZE - 2) * (BOARD_SIZE - 2) - 1 <= head_index) // 이길 경우
                    {
                        win = true;
                        break;
                    }
                    new_apple(&apple_x, &apple_y, snake, head_index + 1); // 사과 초기화
                } else // 갈 곳이 빈 칸일 경우
                { 
                    tmp[0].x = snake[head_index].x;
                    tmp[0].y = snake[head_index].y - 1;
                    move(snake, tmp, head_index);
                }
                break;

            case 'D': // 방향이 아래일 때
                if (snake[head_index].y == BOARD_SIZE - 2) // 벽에 닿을 경우 체크
                {
                    lose = true;
                    break;
                }
                for (int i = 1; i < head_index; i++) // 자기 몸에 닿을 경우 체크(꼬리는 한칸 이동하면 안닿으므로 제외)
                {
                    if (snake[head_index].x == snake[i].x && snake[head_index].y == snake[i].y - 1)
                    {
                        lose = true;
                        break;
                    }
                }
                if (snake[head_index].x == apple_x && snake[head_index].y == apple_y - 1) // 사과를 먹을 경우
                {
                    // 사과의 위치로 머리통 하나를 더 붙이고 그것을 머리로 한다.
                    head_index++; 
                    snake[head_index].x = apple_x;
                    snake[head_index].y = apple_y;
                    if ((BOARD_SIZE - 2) * (BOARD_SIZE - 2) - 1 <= head_index) // 이길 경우
                    {
                        win = true;
                        break;
                    }
                    new_apple(&apple_x, &apple_y, snake, head_index + 1); // 사과 초기화
                } else // 갈 곳이 빈 칸일 경우
                { 
                    tmp[0].x = snake[head_index].x;
                    tmp[0].y = snake[head_index].y + 1;
                    move(snake, tmp, head_index);
                }
                break;

            case 'L': // 방향이 왼쪽일 때
                if (snake[head_index].x == 1) // 벽에 닿을 경우 체크
                {
                    lose = true;
                    break;
                }
                for (int i = 1; i < head_index; i++) // 자기 몸에 닿을 경우 체크(꼬리는 한칸 이동하면 안닿으므로 제외)
                {
                    if (snake[head_index].y == snake[i].y && snake[head_index].x == snake[i].x + 1)
                    {
                        lose = true;
                        break;
                    }
                }
                if (snake[head_index].y == apple_y && snake[head_index].x == apple_x + 1) // 사과를 먹을 경우
                {
                    // 사과의 위치로 머리통 하나를 더 붙이고 그것을 머리로 한다.
                    head_index++; 
                    snake[head_index].x = apple_x;
                    snake[head_index].y = apple_y;
                    if ((BOARD_SIZE - 2) * (BOARD_SIZE - 2) - 1 <= head_index) // 이길 경우
                    {
                        win = true;
                        break;
                    }
                    new_apple(&apple_x, &apple_y, snake, head_index + 1); // 사과 초기화
                } else // 갈 곳이 빈 칸일 경우
                { 
                    tmp[0].x = snake[head_index].x - 1;
                    tmp[0].y = snake[head_index].y;
                    move(snake, tmp, head_index);
                }
                break;

            case 'R': // 방향이 오른쪽일 때
                if (snake[head_index].x == BOARD_SIZE - 2) // 벽에 닿을 경우 체크
                {
                    lose = true;
                    break;
                }
                for (int i = 1; i < head_index; i++) // 자기 몸에 닿을 경우 체크(꼬리는 한칸 이동하면 안닿으므로 제외)
                {
                    if (snake[head_index].y == snake[i].y && snake[head_index].x == snake[i].x - 1)
                    {
                        lose = true;
                        break;
                    }
                }
                if (snake[head_index].y == apple_y && snake[head_index].x == apple_x - 1) // 사과를 먹을 경우
                {
                    // 사과의 위치로 머리통 하나를 더 붙이고 그것을 머리로 한다.
                    head_index++; 
                    snake[head_index].x = apple_x;
                    snake[head_index].y = apple_y;
                    if ((BOARD_SIZE - 2) * (BOARD_SIZE - 2) - 1 <= head_index) // 이길 경우
                    {
                        win = true;
                        break;
                    }
                    new_apple(&apple_x, &apple_y, snake, head_index + 1); // 사과 초기화
                } else // 갈 곳이 빈 칸일 경우
                { 
                    tmp[0].x = snake[head_index].x + 1;
                    tmp[0].y = snake[head_index].y;
                    move(snake, tmp, head_index);
                }
                break;

                default:
                    break;
        }

        draw_game(snake, head_index, apple_x, apple_y); // 요소 계산 후 프레임을 그려준다.
        prev_dir = next_dir;
        for (int i = 0; i < MOVE_DELAY; i++)
        {
            handle_input(prev_dir, &next_dir);
            wait();
        }
    }
    
    if (win == true) // 이길 경우
    {
        draw(BOARD_SIZE / 2 - 4, BOARD_SIZE / 2 - 1, "YOU WIN!");
        draw(BOARD_SIZE / 2 - 9, BOARD_SIZE / 2, "Try Again? (Enter)");
        while (key(K_ENTER) == false)
        {
            handle_input(prev_dir, &next_dir); // ESC 눌러서 종료 가능하게
	        wait();
        }
    }
    if (lose == true) // 질 경우
    {
        draw(BOARD_SIZE / 2 - 4, BOARD_SIZE / 2 - 1, "YOU LOSE!");
        draw(BOARD_SIZE / 2 - 9, BOARD_SIZE / 2, "Try Again? (Enter)");
        while (key(K_ENTER) == false)
        {
            handle_input(prev_dir, &next_dir); // ESC 눌러서 종료 가능하게
	        wait();
        }
    }
}

void new_apple(int* apple_x, int* apple_y, snake_xy snake[], int snake_length) // 사과 위치 랜덤결정
{
    *apple_x = rand() % (BOARD_SIZE - 1);
    *apple_y = rand() % (BOARD_SIZE - 1);
    if (*apple_x == 0) ++*apple_x;
    if (*apple_y == 0) ++*apple_y;

    for (int i = 0; i < snake_length; i++)
    {
        if (*apple_x == snake[i].x && *apple_y == snake[i].y)
        {
            *apple_x = rand() % (BOARD_SIZE - 1);
            *apple_y = rand() % (BOARD_SIZE - 1);
            if (*apple_x == 0) ++*apple_x;
            if (*apple_y == 0) ++*apple_y;
            i = -1;
        }
    }
}

void handle_input(char prev_dir, char* next_dir)
{ // 키보드 입력받기. 방향키일 경우 방향을 조건에 맞게 바꿈, ESC일 경우 프로그램 종료
    if (key(K_UP) && prev_dir != 'D') *next_dir = 'U';
    if (key(K_DOWN) && prev_dir != 'U') *next_dir = 'D';
    if (key(K_LEFT) && prev_dir != 'R') *next_dir = 'L';
    if (key(K_RIGHT) && prev_dir != 'L') *next_dir = 'R';
    if (key(K_ESC)) exit(0);
}

void move(snake_xy snake[], snake_xy tmp[2], int head_index)
{ // 뱀의 머리부터 한칸씩 이동하기. 함수를 부를 때는 tmp[0] 에 갈 곳을 저장해 함수를 부른다.
    for (int i = head_index; 0 <= i; i--)
    {
        swap(tmp[0].x, tmp[1].x);
        swap(tmp[0].y, tmp[1].y);
        // 이제 tmp[0] = 가기 전 좌표 저장 공간, tmp[1] = 갈 곳이 된다.
        tmp[0].x = snake[i].x;
        tmp[0].y = snake[i].y;

        snake[i].x = tmp[1].x;
        snake[i].y = tmp[1].y;
    }
}

void draw_game(snake_xy snake[], int head_index, int apple_x, int apple_y) // 게임 전체 요소를 그린다.
{
    for (int i = 1; i < BOARD_SIZE - 1; i++) // 벽그리기
        {
            draw(0, i, WALL_VERTICAL_STRING);
            draw(BOARD_SIZE - 1, i, WALL_VERTICAL_STRING);
            
            draw(i, 0, WALL_HORIZONTAL_STRING);
            draw(i, BOARD_SIZE - 1, WALL_HORIZONTAL_STRING);
        }
        draw(0, 0, WALL_LEFT_TOP_STRING);
        draw(0, BOARD_SIZE - 1, WALL_LEFT_BOTTOM_STRING);
        draw(BOARD_SIZE - 1, 0, WALL_RIGHT_TOP_STRING);
        draw(BOARD_SIZE - 1, BOARD_SIZE - 1, WALL_RIGHT_BOTTOM_STRING);
        
        for (int i = 0; i <= head_index; i++) // 뱀그리기
        {
            draw(snake[i].x, snake[i].y, SNAKE_STRING);
        }
        
        draw(apple_x, apple_y, APPLE_STRING); // 사과그리기
        
        // 점수표시(단, BOARD_SIZE 가 25인 경우에는 (x, 25) 좌표에서 출력되므로 최대 좌표인 24를 넘어가 보이지 않음)
        string score = "Score: " + to_string(10 * head_index);
        if (BOARD_SIZE < score.length())
        {
            draw(0, BOARD_SIZE, score);
        } else
        {
            draw(BOARD_SIZE / 2 - score.length() / 2, BOARD_SIZE, score);
        }
}
