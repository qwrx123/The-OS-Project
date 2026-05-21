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

TEST(itoa_test, return_pointer_is_buffer)
{
	char buf[32] = { 0 };
	char *ret = itoa(123, buf, 10);
	EXPECT_EQ(ret, buf);
	EXPECT_STREQ("123", buf);
}

TEST(itoa_test, base16_lowercase)
{
	char buf[32] = { 0 };
	itoa(255, buf, 16);
	EXPECT_STREQ("ff", buf);
}

TEST(itoa_test, base2_binary)
{
	char buf[32] = { 0 };
	itoa(5, buf, 2);
	EXPECT_STREQ("101", buf);
}

TEST(itoa_test, base8_octal)
{
	char buf[32] = { 0 };
	itoa(64, buf, 8);
	EXPECT_STREQ("100", buf);
}

TEST(itoa_test, large_number_base10)
{
	char buf[32] = { 0 };
	itoa(2147483647, buf, 10);
	EXPECT_STREQ("2147483647", buf);
}