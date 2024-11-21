#ifndef CHAPTER2_H
#define CHAPTER2_H
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

typedef unsigned char *byte_pointer;
typedef unsigned int uint32;
void function_from_chat2();
void show_bytes(byte_pointer start, size_t len);
void show_int (int x);
void show_float (float x);
void show_pointer(void *x);
void test_show_bytes(int val);
void move_bits();
void hex_to_binary(uint32 n);
void inplace_swap(int *x, int *y);
void reverse_array(int a[], int cnt);
void or_operation(uint32 x);
int bis(int x, int m);
int bic(int x, int m);
void contrast();
int is_equal(int x, int y);
int fun1(unsigned word);
int fun2(unsigned word);
float sum_elements(const float a[], unsigned length);
int str_longer(char *s, char *t);
int u_add_ok(unsigned x, unsigned y);
#endif