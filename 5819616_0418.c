#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 100

typedef int element;
typedef struct {
    element* data;
    int capacity;
    int top;
} StackType;

void init(StackType* s) {
    s->data = (element*)malloc(MAX_STACK_SIZE * sizeof(element));
    s->top = -1;
    s->capacity = MAX_STACK_SIZE;
}

int is_empty(StackType* s) {
    return s->top == -1;
}

int is_full(StackType* s) {
    return s->top == s->capacity - 1;
}

void push(StackType* s, element item) {
    if (is_full(s)) {
        fprintf(stderr, "Stack overflow\n");
        return;
    }
    s->data[++s->top] = item;
}

element pop(StackType* s) {
    if (is_empty(s)) {
        fprintf(stderr, "Stack is empty\n");
        exit(1);
    }
    return s->data[s->top--];
}

element peek(StackType* s) {
    if (is_empty(s)) {
        fprintf(stderr, "Stack is empty\n");
        exit(1);
    }
    return s->data[s->top];
}

int precedence(char op) {
    switch (op) {
    case '(': case ')': return 0;
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    }
    return -1;
}

void infix_to_postfix(char* expr, char* result) {
    int length = strlen(expr);
    int j = 0;
    StackType s;
    init(&s);

    for (int i = 0; i < length; i++) {
        char token = expr[i];
        if (token == ' ') continue;

        if (token >= '0' && token <= '9') { 
            result[j++] = token;
            while (i + 1 < length && expr[i + 1] >= '0' && expr[i + 1] <= '9') {
                result[j++] = expr[++i];
            }
            result[j++] = ' ';
        }
        else if (token == '(') {
            push(&s, token);
        }
        else if (token == ')') {
            while (!is_empty(&s) && peek(&s) != '(') {
                result[j++] = pop(&s);
                result[j++] = ' ';
            }
            pop(&s); 
        }
        else {
            while (!is_empty(&s) && precedence(token) <= precedence(peek(&s))) {
                result[j++] = pop(&s);
                result[j++] = ' ';
            }
            push(&s, token);
        }
    }
    while (!is_empty(&s)) {
        result[j++] = pop(&s);
        result[j++] = ' ';
    }
    result[j] = '\0';
}

int eval_postfix(char* expr) {
    StackType s;
    init(&s);
    char* token = strtok(expr, " ");

    while (token != NULL) {
        if (token[0] >= '0' && token[0] <= '9') {
            push(&s, atoi(token));
        }
        else {
            int val1 = pop(&s);
            int val2 = pop(&s);
            switch (token[0]) {
            case '+': push(&s, val2 + val1); break;
            case '-': push(&s, val2 - val1); break;
            case '*': push(&s, val2 * val1); break;
            case '/': push(&s, val2 / val1); break;
            }
        }
        token = strtok(NULL, " ");
    }
    return pop(&s);
}

int main(int argc, char* argv[]) {
    char infixExpression[MAX_STACK_SIZE];
    char postfixExpression[MAX_STACK_SIZE]; 

    while (1) {
        printf("다음과 같은 메뉴로 동작하는 프로그램입니다 : \n");
        printf("1. 중위식을 입력 받음\n");
        printf("2. 중위식을 후위식으로 변환\n");
        printf("3. 후위식을 계산\n");
        printf("4. 종료\n");
        printf("메뉴를 선택하세요 : ");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("중위식 입력하세요 : ");
            getchar();  
            fgets(infixExpression, sizeof(infixExpression), stdin);
            infixExpression[strcspn(infixExpression, "\n")] = 0;
            printf("입력된 중위식 : %s\n\n", infixExpression);
            break;
        case 2:
            infix_to_postfix(infixExpression, postfixExpression);
            printf("변환된 후위식 : %s\n\n", postfixExpression);
            break;
        case 3:
        {
            char tempExpression[MAX_STACK_SIZE];
            strcpy(tempExpression, postfixExpression);
            int result = eval_postfix(tempExpression);
            printf("후위식 계산 결과 : %d\n\n", result);
        }
        break;
        case 4:
            printf("프로그램을 종료합니다.\n");
            return 0;
        default:
            printf("잘못된 메뉴 선택입니다. 다시 선택하세요.\n");
            break;
        }
    }
    return 0;
}