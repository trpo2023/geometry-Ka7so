#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100
#define RED "\e[1;31m"
#define CLOSE "\e[0m"

struct point {
    double x;
    double y;
};

struct circle {
    struct point center;
    double radius;
};

enum ERROR {
    ERROR_NAME,
    ERROR_BRACKET,
    ERROR_NUMBER,
    ERROR_UNEXPECT_TOKEN,
    ERROR_EXPECT_COMMA,
};

void show_error(int ERROR, int num, char* ch)
{
    for (int i = 0; i < num; i++) {
        putchar(' ');
    }
    printf("^\n");

    switch (ERROR) {
    case ERROR_NAME:
        printf("Ошибка в ожидаемом %d: столбце %s'круг'%s\n", num, RED, CLOSE);
        break;
    case ERROR_BRACKET:
        if (*ch == ')') {
            printf("Error at column %d: expected %s'('%s\n", num, RED, CLOSE);
            break;
        } else if (*ch == '(') {
            printf("Error at column %d: expected %s')'%s\n", num, RED, CLOSE);
            break;
        } else if (*ch == ' ' && num < 10) {
            printf("Error at column %d: expected %s'('%s\n", num, RED, CLOSE);
            break;
        } else {
            printf("Error at column %d: expected %s')'%s\n", num, RED, CLOSE);
            break;
        }
    case ERROR_NUMBER:
        printf("Error at column %d: expected %s'<double>'%s\n", num, RED, CLOSE);
        break;
    case ERROR_UNEXPECT_TOKEN:
        printf("Error at column %d: unexpected token\n", num);
        break;
    case ERROR_EXPECT_COMMA:
        printf("Error at column %d: expected %s','%s\n", num, RED, CLOSE);
        break;
    }
}

void to_lower(char* arr, int num)
{
    for (int i = 0; i < num; i++) {
        arr[i] = tolower(arr[i]);
    }
}

void skip_space(char* arr, int* num, char ch)
{
    while (arr[*num] != ch) {
        if (arr[*num] == ' ') {
            *num += 1;
        } else if ((ch == '0') && (isdigit(arr[*num]) || arr[*num] == '-' || arr[*num] == '.')){
            break;
        } else if (ch == ','){
            show_error(ERROR_EXPECT_COMMA, *num, NULL);
            exit(EXIT_FAILURE);
        } else if (ch == ')'){
            show_error(ERROR_BRACKET, *num, &arr[*num]);
            exit(EXIT_FAILURE);
        } else if (ch == '\n'){
            show_error(ERROR_UNEXPECT_TOKEN, *num, NULL);
            exit(EXIT_FAILURE);
        }
    }
}

double number_search(char* arr, int* num)
{
    char* exp = malloc(20 * sizeof(char));
    int i = 0;
    
    while (isdigit(arr[*num]) || arr[*num] == '-' || arr[*num] == '.') {
        if (arr[*num] == '.' && arr[*(num + 1)] == '.') {
            show_error(ERROR_NUMBER, *num, NULL);
            exit(EXIT_FAILURE);
        }
        exp[i] = arr[*num];
        i++;
        *num += 1;
    }
    
    char* trash;
    double Number = strtod(exp, &trash);
    free(exp);
    return Number;
}

struct circle data_of_figure(char* arr, int* num)
{
    struct circle Circle;

    *num += 1;
    skip_space(arr, num, '0');
    Circle.center.x = number_search(arr, num);

    skip_space(arr, num, '0');
    Circle.center.y = number_search(arr, num);
    skip_space(arr, num, ',');
    *num += 1;

    skip_space(arr, num, '0');
    Circle.radius = number_search(arr, num);
    skip_space(arr, num, ')');
    *num += 1;
    skip_space(arr, num, '\n');

    return Circle;
}

void show_figure(struct circle* Circle, char* figure)
{
    printf("\n%s(%.2f %.2f, %.2f)\n", 
        figure,
        Circle->center.x,
        Circle->center.y,
        Circle->radius);
}

int main()
{
    char enter[SIZE], figure[SIZE];
    int num = 0;

    printf("Введите геометрическую фигуру (или нажмите ENTER для выхода):\n");
    fgets(enter, SIZE, stdin);
    
    for (int i = 0; i < strlen(enter); i++) {
        if (enter[i] == '(') {
            to_lower(figure, num);
            if (strcmp(figure, "circle") == 0) {
                struct circle Circle = data_of_figure(enter, &num);
                show_figure(&Circle, figure);
                break;
            } else {
                show_error(ERROR_NAME, 0, NULL);
                exit(EXIT_FAILURE);
            }
        } else if (enter[i] == ')' || enter[i] == ' '){
            show_error(ERROR_BRACKET, num, &enter[i]);
            exit(EXIT_FAILURE);
        }

        figure[num] = enter[i];
        num++;
    }

    return 0;
}