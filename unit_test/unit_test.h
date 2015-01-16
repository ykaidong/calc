/**
  ******************************************************************************
  * @file    unit_test.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _UNIT_TEST_H_
#define _UNIT_TEST_H_

#ifdef UNIT_TESTING
/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
// 为了能够引用模块内的静态变量与函数, 需要取消掉其静态属性
#define     static

// 执行单元测试时main()被定义于测试模块中
#define     main        test_main

// 替换用户输入函数, 定义于测试模块中
#define     getch       test_getch
extern int test_getch(void);

// 替换printf()函数
#define     printf      test_printf
extern int test_printf(const char *format, ...);

/* Exported functions ------------------------------------------------------- */
#endif  // end of UNIT_TESTING

#endif
/************* Copyright(C) 2013 - 2015 DevLabs **********END OF FILE**********/


