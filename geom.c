#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define RED_FLASH "\e[5;31m"
#define END "\e[0m"
#define MAX 100

struct Dot {
    double x;
    double y;
};

struct Circle {
    struct Dot center;
    double radius;
};
// Изменены названия 
enum ERROR {
    ERROR_NAME,
    ERROR_STAPLES,
    ERROR_NUMBER,
    ERROR_UNIDENTIFIED_VARIABLES,
    ERROR_EXPECT_COMMA,
    ERROR_STAPLES_2,
    ERROR_EXTRA_POINT,
};

void output_bugs(int errors, int num, char* ch)
{
    for (int i = 0; i < num; i++) {
        putchar(' ');
    }
    printf("^\n");

    switch (errors) {
    case ERROR_NAME:
        printf("An error %d was found in the input line %s'Circle'%s\n",
               num,
               RED_FLASH,
               END);
        break;
    case ERROR_STAPLES:
        if (*ch == ')') {
            printf("An error %d was found in the input line %s')'%s\n",
                   num,
                   RED_FLASH,
                   END);
            break;
        } else {
            printf("An error %d was found in the input line %s'('%s\n",num,RED_FLASH,END);
            break;
        }
    case ERROR_STAPLES_2:
    	printf("An error %d was found in the input line %s')'%s\n",num,RED_FLASH,END);
        break;
    case ERROR_NUMBER:
        printf("An error %d was found in the input line %s'double'%s\n",num,RED_FLASH,END);
        break;
    case ERROR_UNIDENTIFIED_VARIABLES:
        printf("An error %d was found in the input line %s'variable'%s\n",num,RED_FLASH,END);
        break;
    case ERROR_EXPECT_COMMA:
        printf("An error %d was found in the input line %s','%s\n",num,RED_FLASH,END);
        break;
    case ERROR_EXTRA_POINT:
    	printf("An error %d was found in the input line %s'.'%s\n",num,RED_FLASH,END);
        break;
    }
}

void to_lower(char* str, int ch)
{
    for (int i = 0; i < ch; i++) {
        str[i] = tolower(str[i]);
    }
}

double x_data(char* arr, int* num)
{
    char free_space[20];
    int i = 0;

    while (!isdigit(arr[*num]) && arr[*num] != '-') {
        if (arr[*num] == '(') {
            *num += 1;
        } else {
            if (arr[*num] == ')') {
                output_bugs(ERROR_STAPLES, *num, &arr[*num]);
                exit(1);
            }
            if (arr[*num] == ' '){
            	output_bugs(ERROR_STAPLES, *num, &arr[*num]);
                exit(1);            
            } else {
                output_bugs(ERROR_NUMBER, *num, NULL);
                exit(1);
            }
        }
    }

    while (isdigit(arr[*num]) || arr[*num] == '-' || arr[*num] == '.') {
        free_space[i] = arr[*num];
        i++;
        *num += 1;
    }
    
    if (arr[*num] != ' ') {
        output_bugs(ERROR_UNIDENTIFIED_VARIABLES, *num, NULL);
        exit(1);
    }
    char* dumpster;
    return strtod(free_space, &dumpster);
}

double y_data(char* arr, int* num)
{
    char free_space[20];
    int i = 0;

    while (!isdigit(arr[*num]) && arr[*num] != '-') {
        if (arr[*num] == ' ') {
            *num += 1;
        } else {
            output_bugs(ERROR_NUMBER, *num, NULL);
            exit(1);
        }
    }

    while (isdigit(arr[*num]) || arr[*num] == '-' || arr[*num] == '.') {
        free_space[i] = arr[*num];
        i++;
        *num += 1;
    }

    while (arr[*num] != ',') {
        if (arr[*num] == ' ') {
            *num += 1;
        } else {
            output_bugs(ERROR_EXPECT_COMMA, *num, NULL);
            exit(1);
        }
    }
    char* dumpster;
    return strtod(free_space, &dumpster);
}

double radius_data(char* arr, int* num)
{
    char free_space[20];
    int i = 0;
    int extra_point_count = 0;

    while (!isdigit(arr[*num])) {
        if (arr[*num] == ' ' || arr[*num] == ',') {
            *num += 1;
        } else {
            output_bugs(ERROR_NUMBER, *num, NULL);
            exit(1);
        }
    }

    while (isdigit(arr[*num]) || arr[*num] == '.') {
        free_space[i] = arr[*num];
        i++;
        *num += 1;
        if (arr[*num] == '.'){
            extra_point_count += 1;
        }
        if (extra_point_count >= 2){
            output_bugs(ERROR_EXTRA_POINT, *num, &arr[*num]);
            exit(1);
        }
    }

    while (arr[*num] != ')') {
        if (arr[*num] == ' ') {
            *num += 1;
        } else {
            if (arr[*num] == '(') {
                output_bugs(ERROR_STAPLES, *num, &arr[*num]);
                exit(1);
            } else {
                output_bugs(ERROR_STAPLES_2, *num, &arr[*num]);
                exit(1);
            }
        }
    }
    char* dumpster;
    return strtod(free_space, &dumpster);
}

void empty(char* arr, int* num)
{
    *num += 1;
    while (arr[*num] != '\n' && arr[*num] != EOF) {
        if (arr[*num] == ' ') {
            *num += 1;
        } else {
            output_bugs(ERROR_UNIDENTIFIED_VARIABLES, *num, NULL);
            exit(1);
        }
    }
}

struct Dot find_center(char* arr, int* num)
{
    struct Dot Center;

    Center.x = x_data(arr, num);
    Center.y = y_data(arr, num);

    return Center;
}

struct Circle find_out_circle(struct Dot* Center, char* arr, int* num)
{
    struct Circle circle;

    circle.center.x = Center->x;
    circle.center.y = Center->y;
    circle.radius = radius_data(arr, num);

    return circle;
}

void output_circle_message(struct Circle* circle)
{
    printf("\ncircle(%.2f %.2f, %.2f)\n",
           circle->center.x,
           circle->center.y,
           circle->radius);
    printf("perimeter: %.4f\n", (2 * M_PI * circle->radius));
    printf("area: %.4f\n", ((circle->radius * circle->radius) * M_PI));
}

int main()
{
    char enter[MAX], shape[MAX];
    int num = 0;

    printf("Enter the name and coordinates of the shape (or press Enter for "
           "exit):\n");
    fgets(enter, MAX, stdin);

    for (int i = 0; i < strlen(enter); i++) {
        if (enter[i] == '(' || enter[i] == ' ') {
            to_lower(shape, num);
            if (strcmp(shape, "circle") == 0) {
                struct Dot center = find_center(enter, &num);
                struct Circle circle = find_out_circle(&center, enter, &num);
                empty(enter, &num);
                output_circle_message(&circle);
                break;
            } else {
                output_bugs(ERROR_NAME, 0, NULL);
                exit(1);
            }
        } else if (enter[i] == ')') {
            output_bugs(ERROR_STAPLES, num, &enter[i]);
            exit(1);
        }

        shape[num] = enter[i];
        num++;
    }

    return 0;
}