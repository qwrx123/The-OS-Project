/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <gtest/gtest.h>
#include <iostream>
extern "C"
{
	//STD library wants _STDLIB_H
	extern char *itoa(int value, char *str, int base);
}

TEST(itoa_test, zero_base10)
{
	char buf[32] = { 0 };
	itoa(0, buf, 10);
	EXPECT_STREQ("0", buf);
}

TEST(itoa_test, positive_base10)
{
	char buf[32] = { 0 };
	itoa(12345, buf, 10);
	EXPECT_STREQ("12345", buf);
}

TEST(itoa_test, single_digit)
{
	char buf[32] = { 0 };
	itoa(7, buf, 10);
	EXPECT_STREQ("7", buf);
}