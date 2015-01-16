/**
  ******************************************************************************
  * @file    calc_test.c
  * @author  ykaidong (http://www.DevLabs.cn)
  * @version V0.1
  * @date    2015-01-14
  * @brief
  ******************************************************************************
  * Change Logs:
  * Date           Author       Notes
  * 2015-01-14     ykaidong     the first version
  *
  ******************************************************************************
  * @attention
  *
  * Copyright(C) 2013-2015 by ykaidong<ykaidong@126.com>
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

// 单元测试所需要的头文件
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include ".\cmockery\cmockery.h"

// 其它
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Private typedef -----------------------------------------------------------*/

// 位于目标文件中的一些typedef和宏, 原样搬过来
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
#define KEY_ENTER       13
#define KEY_ESC         27

// 错误
#define NO_ERROR        0
#define SYTAX_ERROR     1

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

// 引用目标模块中的函数
extern int get_input(char *buf);
extern token_t siglechar(void);
extern token_t get_next_token(void);
extern bool tokenizer_finished(void);
extern void error(int error);
extern void tokenizer_next(void);
extern token_t tokenizer_token(void);
extern int tokenizer_num(void);
extern void accept(token_t token);
extern int factor(void);
extern int term(void);
extern int expr(void);
extern void tokenizer_init(char *expr);
extern int test_main(int argc, char *argv[]);

// 引用目标模块中的全局变量
extern char *curr_char;
extern char *next_char;
extern token_t current_token;
extern int error_code;


// 替换getch(), 模拟用户输入
int test_getch(void)
{
    // 这个返回值是使用will_return()函数指定的
    return (int)mock();
}

// 替换printf()
int test_printf(const char *format, ...)
{
    int return_value;
    va_list args;
    char temporary_buffer[32];
    va_start(args, format);
    return_value = vsnprintf(temporary_buffer, sizeof(temporary_buffer),
        format, args);
    va_end(args);

    return return_value;
}



// 测试函数
void test_get_input(void **state)
{
    // 模拟用户输入的字符串
    char *user_input = "10   +9 - 8(\b  * (4 - 3)2\b/2   \r";
    // 用户输入被处理之后得到的字符串
    char *input = "10+9-8*(4-3)/2";
    // 存储输入的缓存区
    char input_buf[EXPR_LEN];

    size_t i;
    for (i = 0; i < strlen(user_input); i++)
    {
        // 当调用test_getch()时将依次返回此处写入的字符
        will_return(test_getch, *(user_input + i));
    }

    // 测试, get_input()返回的是输入的有效字符数
    assert_int_equal(get_input(input_buf), strlen(input));

    // 测试, 得到的输入值是否符合期望
    assert_string_equal(input_buf, input);

    return;
}



void test_singlechar(void **state)
{
    // 待测试的符号
    char *tok_str = "+-*/()= ";
    // 正确的返回值
    token_t retval[] =
    {
        TOKENIZER_PLUS,     // +
        TOKENIZER_MINUS,    // -
        TOKENIZER_ASTR,     // *
        TOKENIZER_SLASH,    // /
        TOKENIZER_LPAREN,   // (
        TOKENIZER_RPAREN,   // )
        TOKENIZER_ERROR,    // =
        TOKENIZER_ERROR     //
    };

    // curr_char 是目标模块中的静态全局变量
    char *backup = curr_char;
    curr_char = tok_str;

    // int i;
    // for (i = 0; i < (sizeof(retval) / sizeof(retval[0])); i++)
    // {
    //     assert_int_equal((int)siglechar(), (int)retval[i]);
    //     curr_char++;
    // }

    assert_int_equal((int)siglechar(), (int)retval[curr_char - tok_str]);
    curr_char++;
    assert_int_equal((int)siglechar(), (int)retval[curr_char - tok_str]);
    curr_char++;
    assert_int_equal((int)siglechar(), (int)retval[curr_char - tok_str]);
    curr_char++;
    assert_int_equal((int)siglechar(), (int)retval[curr_char - tok_str]);
    curr_char++;
    assert_int_equal((int)siglechar(), (int)retval[curr_char - tok_str]);
    curr_char++;
    assert_int_equal((int)siglechar(), (int)retval[curr_char - tok_str]);
    curr_char++;
    assert_int_equal((int)siglechar(), (int)retval[curr_char - tok_str]);
    curr_char++;
    assert_int_equal((int)siglechar(), (int)retval[curr_char - tok_str]);
    curr_char++;

    // 恢复
    curr_char = backup;

    return;
}


// 对expr()进行测试之前进行必要的初始化(构造函数)
void test_expr_setup(void **state)
{
    // 初始化一个表达式, 以提供给expr()计算
    char *expr = "1+2*3-(4/2)";    // = 5

    tokenizer_init(expr);
}


// 对expr()进行测试
void test_expr(void **state)
{
    assert_int_equal(expr(), 5);
}

// 对expr()进行测试后清场的函数(析构函数)
void test_expr_teardown(void **state)
{
    return;
}


int main(int argc, char *argv[])
{
    UnitTest tests[] =
    {
        unit_test(test_get_input),
        unit_test(test_singlechar),
        unit_test_setup_teardown(test_expr, test_expr_setup, test_expr_teardown),
        // unit_test_setup_teardown(test_expr, test_expr_setup, NULL);  // 这样也可以
    };

    return run_tests(tests);
}


/************* Copyright(C) 2013 - 2015 DevLabs **********END OF FILE**********/


