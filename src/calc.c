/**
 ******************************************************************************
 * @file    calc.c
 * @author  ykaidong (http://www.DevLabs.cn)
 * @version V0.1
 * @date    2014-12-22
 * @brief
 ******************************************************************************
 * Change Logs:
 * Date           Author       Notes
 * 2014-12-22     ykaidong     the first version
 *
 ******************************************************************************
 * @attention
 *
 * Copyright(C) 2013-2014 by ykaidong<ykaidong@126.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>          // getch()
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#ifdef UNIT_TESTING
#include "..\unit_test\unit_test.h"
#endif

/* Private typedef -----------------------------------------------------------*/
typedef enum {
    TOKENIZER_ERROR,        // Error
    TOKENIZER_ENDOFINPUT,   // End of input
    TOKENIZER_NUMBER,       // number
    TOKENIZER_PLUS,         // +
    TOKENIZER_MINUS,        // -
    TOKENIZER_ASTR,         // *
    TOKENIZER_SLASH,        // /
    TOKENIZER_LPAREN,       // (
    TOKENIZER_RPAREN,       // )
} token_t;


/* Private define ------------------------------------------------------------*/
#define EXPR_LEN        512 // 允许表达式长度
#define MAX_NUM_LEN     5   // 运算数长度

// 按键
#define KEY_BACKSPACE   8
#define KEY_DEL              127
#define KEY_ENTER           13
#define KEY_ESC                27
#define KEY_EQUAL           61

// 错误
#define NO_ERROR        0
#define SYTAX_ERROR     1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static char *curr_char = NULL;     // 表达式中当前分析到的字符
static char *next_char = NULL;     // 表达式中下一个字符
static token_t current_token = TOKENIZER_ERROR;
static int error_code = NO_ERROR;

/* Private function prototypes -----------------------------------------------*/
int factor(void);
int term(void);
int expr(void);

/* Private functions ---------------------------------------------------------*/

/**
 * \brief  获取表达式输入, 只接收数字与加减乘除运算符与括号
 *
 * \param  buf 用于存储表达式的缓存
 *
 * \return 接收的字符数
 */
int get_input(char *buf)
{
    int i = 0;
    char ch;

    while (i < EXPR_LEN)
    {
        ch = getch();
        // 只接收数字与运算符, 括号
        if (ch >= '(' && ch <= '9' && ch != ',' && ch != '.')
        {
            *buf = ch;
            // putchar(*buf);
            printf("%c", *buf);
            buf++;
            i++;
        }
        // 处理退格
        else if (ch == KEY_BACKSPACE || ch == KEY_DEL)
        {
            if (i != 0)
            {
                printf("\b \b");
                i--;
                buf--;
            }
        }
        else if (ch == KEY_ESC)
            exit(0);
        // 回车, 输入完毕
        else if (ch == KEY_ENTER || ch == KEY_EQUAL)
        {
        	printf("=");
            break;
        }
    }
    *buf = '\0';

    return i;
}




/**
 * \brief  获取单字符token类型
 *
 * \return token类型
 */
token_t siglechar(void)
{
    switch (*curr_char)
    {
    case '+':
        return TOKENIZER_PLUS;
    case '-':
        return TOKENIZER_MINUS;
    case '*':
        return TOKENIZER_ASTR;
    case '/':
        return TOKENIZER_SLASH;
    case '(':
        return TOKENIZER_LPAREN;
    case ')':
        return TOKENIZER_RPAREN;
    default:
        break;
    }

    return TOKENIZER_ERROR;
}




/**
 * \brief  获取一个token
 *
 * \return token类型
 */
token_t get_next_token(void)
{
    int i;

    // 表达式结束
    if (curr_char == '\0')
        return TOKENIZER_ENDOFINPUT;

    if (isdigit(*curr_char))
    {
        // 不可超过允许数字长度
        for (i = 0; i <= MAX_NUM_LEN; i++)
        {
            // 数字结束
            if (!isdigit(*(curr_char + i)))
            {
                next_char = curr_char + i;
                return TOKENIZER_NUMBER;
            }
        }
    }
    // 分界符
    else if (siglechar())
    {
        next_char++;
        return siglechar();
    }

    return TOKENIZER_ERROR;
}


bool tokenizer_finished(void)
{
    return *curr_char == '\0' || current_token == TOKENIZER_ENDOFINPUT;
}


/**
 * \brief  错误处理
 *
 * \param  err 错误类型
 */
void error(int err)
{
    error_code = err;

    return;
}


/**
 * \brief  解析下一个token
 */
void tokenizer_next(void)
{
    if (tokenizer_finished())
        return;

    curr_char = next_char;
    current_token = get_next_token();

    return;
}



/**
 * \brief  得到当前token
 */
token_t tokenizer_token(void)
{
    return current_token;
}



/**
 * \brief  将ASCII形式的数字转为数值
 */
int tokenizer_num(void)
{
    return atoi(curr_char);
}



/**
 * \brief  匹配token
 */
void accept(token_t token)
{
    if (token != tokenizer_token())
        error(1);

    tokenizer_next();
}



/**
 * \brief  取得当前因子的值, 若当前因子(类似上式中的M)是一个表达式, 进行递归求值
 */
int factor(void)
{
    int r;

    // 当前token的类型
    switch (tokenizer_token())
    {
    // 数字(终结符)
    case TOKENIZER_NUMBER:
        // 将其由ASCII转为数字值
        r = tokenizer_num();
        // 根据语法规则匹配当前token
        accept(TOKENIZER_NUMBER);
        break;
    // 左括号
    case TOKENIZER_LPAREN:
        accept(TOKENIZER_LPAREN);
        // 将括号里的值当作一个新的表达式, 递归计算(递归是从函数expr()开始的)
        r = expr();
        // 当括号里的表达式处理完毕后下一个token一定是右括号
        accept(TOKENIZER_RPAREN);
        break;
        // 除左括号和数字之外的其它token已经被上一级处理掉了
        // 若有其token, 一定是表达式语法错误
    default:
        error(SYTAX_ERROR);
    }

    // 返回因子的值
    return r;
}


/**
 * \brief  求第二级优先级(乘除)表达式的值
 */
int term(void)
{
    int f1, f2;
    token_t op;

    // 获取左操作数(因子)
    f1 = factor();
    // 获取操作符
    op = tokenizer_token();

    // 操作符只能是乘或者除(同一优先级)
    while (op == TOKENIZER_ASTR || op == TOKENIZER_SLASH)
    {
        // 下一个token
        tokenizer_next();
        // 获取右操作数(因子)
        f2 = factor();
        switch ((int)op)
        {
        case TOKENIZER_ASTR:
            f1 = f1 * f2;
            break;
        case TOKENIZER_SLASH:
            f1 = f1 / f2;
            break;
        }
        // 上面计算完毕的这个值将做为左操作数
        op = tokenizer_token();
    }

    return f1;
}

/**
 * \brief  求第一级优先级(加减)表达式的值
 */
int expr(void)
{
    int t1, t2 = 0;
    token_t op;

    // 第一个操作数
    t1 = term();
    // 获取运算符
    op = tokenizer_token();

    // 操作符只能是加或者减(同一优先级)
    while (op == TOKENIZER_PLUS || op == TOKENIZER_MINUS)
    {
        // 下一个token
        tokenizer_next();
        // 第二个操作数
        t2 = term();
        switch ((int)op)
        {
        case TOKENIZER_PLUS:
            t1 = t1 + t2;
            break;
        case TOKENIZER_MINUS:
            t1 = t1 - t2;
            break;
        }
        op = tokenizer_token();
    }

    return t1;
}



/**
 * \brief  词法分析器初始化
 *
 * \param  expr 输入的表达式字符串
 */
void tokenizer_init(char *expr)
{
    curr_char = next_char = expr;
    current_token = get_next_token();

    return;
}




int main(int argc, char *argv[])
{
    int i;
    char e[EXPR_LEN];
    int r;

    printf("DevLabs Calculator [Version 0.2]\n");
    printf("(C) Copyright 2013 - 2014 DevLabs\n");
    printf("Press ENTER or EQUAL  to calculate and press ESC to exit\n");

    while (1)
    {
        printf("\n>>> ");

        i = get_input(e);

        tokenizer_init(e);

        r = expr();

        if (i == 0)
        {
            error(2);
        }

        if (error_code)
        {
            if (error_code == SYTAX_ERROR)
                printf("\nSyntax Error!\n");

            error_code = NO_ERROR;

            continue;
        }

        printf("%d", r);
    }

    return 0;
}


/************* Copyright(C) 2013 - 2014 DevLabs **********END OF FILE**********/

