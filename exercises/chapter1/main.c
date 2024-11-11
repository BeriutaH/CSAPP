#include <stdio.h>
#include <math.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len) {
    size_t i;
    for (i = 0; i < len; i++)
        printf("%.2x", start[i]);
    printf("\n");
}

void show_int(int x) {
    show_bytes((byte_pointer) &x, sizeof(int));
}

void show_float(float x) {
    show_bytes((byte_pointer) &x, sizeof(float));
}

void show_pointer(void *x) {
    show_bytes((byte_pointer) &x, sizeof(void *));
}

void t_show_bytes(int val) {
    int ival = val;
    float fval = (float) ival;
    int *pval = &ival;
    show_int(ival);
    show_float(fval);
    show_pointer(pval);
}

int main() {
//    printf("Hello, World!\n");
//    printf("%d\n", 33);
//    int a = 23;
//    int b = 2312;
//    printf("a: %d, b: %d\n", a, b);
//    printf("小数: %f\n", 12.35);
    double a1 = 34.21;
    double a2 = 23.34;
    printf("a1= %f, a2= %f\n", a1, a2);
//    // 显示小数点后两位
//    double a3 = 3.141592653;
//    printf("显示小数点后两位: %.2f", a3);
//    int a = 200 * 300 * 400 * 500;
//    printf("%d\n", a);
//    float f = (3.14 + 1e20) - 1e20;
//    printf("%f\n", f);
//    float f1 = 3.14 + (1e20 - 1e20);
//    printf("%f", f1);
//    t_show_bytes(12345);
    // 有符号和无符号之间的转化，要从位级角度来看，而不是数的角度,区别在于最高位
//        short int v = -12345;
//        unsigned short uv = (unsigned short) v;
//        printf("v = %d, uv = %u\n", v, uv);  // v = -12345, uv = 53191
//        unsigned u = 4294967295u;
//        int tu = (int) u;
//        printf("u = %u, tu = %d\n", u, tu);  // u = 4294967295, tu = -1
    // 默认是有符号的值，如果想要创建无符号常量，必须加上后缀字符"U" 或者 'u'，例如12345U 或者 0x1A2Bu
    return 0;
}
