#include "chapter2.h"




void show_bytes(byte_pointer bp, size_t len) {
    /*
     * start: 是指向要显示的数据的指针, 强制类型转换为unsigned char *
     * len: 是要显示的字节数
     * return: 指针 bp 所指向内存区域的 len 个字节的十六进制表示。每个字节的十六进制值会紧密地排列在一起
     * */
    for (size_t i = 0; i < len; i++) {
        printf("%.2x", bp[i]);
    }
    printf("\n");
}

void show_int (int x) {
    // show_int 将 x 的地址传给 show_bytes，并指定字节数为 sizeof(int)
    // sizeof 计算数据类型或变量所占用的内存大小（以字节为单位）
    // 以显示该 int 类型变量的内存表示
    show_bytes((byte_pointer)&x, sizeof(int));
}

void show_float (float x) {
    show_bytes((byte_pointer)&x, sizeof(float));
}

void show_pointer(void *x) {
    show_bytes((byte_pointer)&x, sizeof(void *));
}

void test_show_bytes(int val) {
    int ival = val;
    float fval = (float)ival;
    int *pval = &ival;
    show_int(ival);
    show_float(fval);
    show_pointer(pval);
}


// 计算 a 和 b 二进制表示中相同的位数
int count_matching_bits(uint32 a, uint32 b) {
    printf("此时对比的a=");
    hex_to_binary(a);
    printf("此时对比的b=");
    hex_to_binary(b);
    int count = 0;
    for (int i = 0; i < 32; i++) {
        // & 按位与: a & 0x1 会把 a 的最低位提取出来
        if ((a & 0x1) == (b & 0x1)) {
            count++;
        }
        a >>= 1;  // a 右移一位
        b >>= 1;  // b 右移一位
    }
    return count;
}
// 输入十六进制，返回二进制
void hex_to_binary(uint32 n) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);  // 将n右移i位，并与1进行按位与操作，得到每一位的值
    }
    printf("\n");
}

//计算两个十六进制的最多匹配数
void move_bits() {
    // 这里使用无符号32位，是因为无符号不涉及符号位，直接使用二进制数表示，
    // 而有符号的最高位是符号位，用二进制的话，需要补码来表示负数
    uint32 num1 = 0x00359141;
    uint32 num2 = 0x4A564504;
    // 这里左右循环32次，也就是说是-32到+32
    int max_match = 0;

    // 让 num2 沿 num1 左右滑动 32 次，找到最大匹配位数
    for (int shift = -31; shift <= 31; ++shift) {
        uint32 shifted_b = (shift < 0) ? (num2 >> -shift) : (num2 << shift);
        int match = count_matching_bits(num1, shifted_b);
        if (match > max_match) {
            max_match = match;
        }
    }
    printf("最大匹配位数: %d\n", max_match);
}

// 亦或值交换两个值的内容
void inplace_swap(int *x, int *y) {
    *y = *x ^ *y;  // *y 存储了 *x 和 *y 的异或值
    *x = *x ^ *y;  // *x 被还原为原始的 *y 值
    *y = *x ^ *y;  // *y 被还原为原始的 *x 值
}

void reverse_array(int a[], int cnt) {
    int first, last;
    for (first = 0, last = cnt-1;first <= last; first++, last--) {
        printf("first=%d, last=%d\n", first, last);
        if (first != last) {
            inplace_swap(&a[first], &a[last]);
        }
    }
}

void or_operation(uint32 x) {
    printf("x原始值:");
    hex_to_binary(x);
    // 掩码，保留最低有效位，忽略高位的内容
    uint32 mask_low = 0xFF;
    printf("mask_low:");
    hex_to_binary(mask_low);
    // x先跟0xFF与运算,先保证x的最低有效字节为8位
    uint32 low_byte = x & mask_low;
    printf("保留最低有效位:");
    hex_to_binary(low_byte);
    // 对高 3 个字节进行取反，然后加 1（即取补）
    // 补码
    printf("取补开始\n");
    uint32 reverse_x = ~x;
    printf("1.先将x取反:");
    hex_to_binary(reverse_x);
    // 掩码，保留高位，将最低有效字节清零
    uint32 mask_height =0xFFFFFF00;
    printf("高位掩码mask_height:");
    hex_to_binary(mask_height);
    printf("2.再将reverse_x跟掩码按位与操作:");
    uint32 test_num = reverse_x & mask_height;
    hex_to_binary(test_num);
    printf("3.将按位与操作得到的结果+1:");
    uint32 reverse_height = test_num+1;
    hex_to_binary(reverse_height);
    uint32 result = reverse_height | low_byte;
    printf("4.最后合并高 3 个字节的取补结果和原始的最低有效字节\n最终的十六进制: %X\n",result);
    // 最低有效位全设置成1
    uint32 height_num = x | mask_low;
    hex_to_binary(height_num);
    printf("%X", height_num);

}

// m为1的每个位置上，将z对应的位设置为1
int bis(int x, int m) {
    /*
     * m: 掩码
     * x: 数据
    */
    return x | m;

}

// m为1的每个位置，将z对应的位设置为0, m为0的，保留原本
int bic(int x, int m) {
    /*
     * m: 掩码
     * x: 数据
    */
    // 位清除操作，& 操作就是都为1的时候，才为1
    return x & ~m;
}

// 位运算跟逻辑运算各自对比
void contrast() {
    // 位运算
    uint32 x = 0x66;  // 二进制 0110 0110
    uint32 y = 0x39;  // 二进制 0011 1001
    uint32 bit_and = x & y; // 0010 0000
    uint32 bit_or = x | y; // 0111 1111
    uint32 bit_reverse = ~x | ~y; // ~x:1001 1001 ~y: 1100 0110 |: 1101 1110
    uint32 bit_and_not = x & !y;  // !y: 0000 0000 x将全部变成0
    // 逻辑运算
    int num1 = x && y;  // true
    int num2 = x || y; // true
    int num3 = !x || !y; // false
    int num4 = x && ~y;  // true （做错）
    hex_to_binary(bit_and);
    hex_to_binary(bit_or);
    hex_to_binary(bit_reverse);
    hex_to_binary(bit_and_not);
    printf("-----------------------------------\n");
    printf("%d\n", num1);
    printf("%d\n", num2);
    printf("%d\n", num3);
    printf("%d\n", num4);
}

// 判断两个数值是不是相同
int is_equal(int x, int y) {
    // 用^表示，如果相等，得出的结果为0，再逻辑not，返回0跟1
    return !(x ^ y);
}

void function_from_chat2() {
    // int溢出，改用long
    //    long long w = 200LL * 300 * 400 * 500;
    //    printf("%lld\n", w);
        /* A. 将Ox39A7F8转换为二进制。 --> 0011 1001 1010 0111 1111 1000
            B. 将二进制1100 1001 0111 1011转换为十六进制。  -->  C 9 7 B
            C. 将OxD5EAC转换为二进制。 --> 1101 0101 1110 1011 1100
            D. 将二进制 10 0110 1110 0111 1011 0101 转換为十六进制   -->  26E7B5

            答案要用 十六进制表示。提示:只要将执行十进制加法和減法所使用的方法改成以16 为基数。
            A. 0х503с + 0х8 =
            B. 0x503c - 0x40 =
            C. 0x503c + 64 =
            D. 0x50ea - 0×503c =
        */
    //    unsigned int a = 0x503C;
    //    unsigned int b = 0x08;
    //    unsigned int c = 0x40;
    //    unsigned int d = 0x50EA;
    //    unsigned int resultA = a + b;
    //    printf("A. 0x503C + 0x08 = 0x%X\n", resultA);
    //    unsigned int resultB = a - c;
    //    printf("B. 0x503c - 0x40 = 0x%X\n", resultB);
    //    unsigned int resultC = a + c;  // 64 的16进制为 0x40
    //    printf("C. 0x503c + 64 = 0x%X\n", resultC);
    //    unsigned int resultD = d - a;
    //    printf("D. 0x50EA - 0x503C = 0x%X\n", resultD);
        // 不同数据值的字节表示
    //    test_show_bytes(12345);

        // 指出在小端法机器和大端法机器上，每次调用的输出值
    //    int val = 0x87654321;
    //    byte_pointer valp = (byte_pointer) &val;
    //    show_bytes(valp,1); // 小端: 21  大端: 87
    //    show_bytes(valp,2); // 小端: 2143  大端: 8765
    //    show_bytes(valp,3); // 小端: 214365  大端: 876543

        /*
         * 使用show_int 和 show_float，我们确定整数3510593的十六进制表示 为Ox00359141，
         * 而浮点数3510593. 0 的十六进制表示为0x4A564504
            A. 写出这两个十六进制值的二进制表示。
                0x00359141 --> 0000 0000 0011 0101 1001 0001 0100 0001
                0x4A564504 --> 0100 1010 0101 0110 0100 0101 0000 0100
            B. 移动这两个二进制串的相对位置，使得它们相匹配的位数最多。有多少位相匹配呢?
                这里需要注意，要双向对比，不能只对比左边，或者只对比右边
                因为这两个十六进制是八位的十六进制，一个十六进制数字对应4位二进制数字，所以八个十六进制是32位二进制
                 move_bits();
            C. 串中的什么部分不相匹配?
             //    const char* s = "abcdef";
            //    show_bytes((byte_pointer)s, strlen(s));
        */


// 2.10
    /*
        // 亦或值，交换值的时候，不需要第三个变量，只需要取亦或值，三次，则会交换
        //    int x = 5;
        //    int y = 7;
        //    // 输出交换前的值
        //    printf("交换前的值: x = %d, y = %d\n", x, y);
        //    inplace_swap(&x, &y);
        //    // 输出交换后的值
        //    printf("交换后的值: x = %d, y = %d\n", x, y);
     */

// 2.11
    /*
     * A. 对于一个长度为奇数的数组，长度cnt= 2k+1，函效reverse_array最后一次 循环中，变量first 和last 的值分别是什么?
        * first=k, last=k
     * B. 为什么这时调用函数i npla ce_swap会将数组元素设置为0?
        * 因为第一次亦或取值，相同的值都是0,
        * 第二次任何数字与 0 进行异或运算都会得到其本身
        * 第三次又变成本身跟本身做对比，又变成了0
     * C. 对reverse_array的代码做哪些简单改动就能消除这个问题?
        * 判断如果是相等的，直接不做亦或取值
        int list[] = {1,3,4, 9,6,5,8};
        //    int cnt = sizeof(list) / sizeof(list[0]);
        //    // list直接传递是因为，传递一个数组作为参数时，数组会被隐式转换为一个指向数组第一个元素的指针
        //    reverse_array(list, cnt);
        //    for (int i = 0; i < cnt; i++) {
        //        printf("%d ", list[i]);
        //    }
    */

// 2.12
    /*
     * 掩码位运算: x & 0xFF 会选中 x 的 最低有效字节（即最低 8 位），其他字节（高 24 位）会被置为 0
     * 对 于 下 面 的 值 ， 写 出 变 量 x 的 C 语 言 表 达 式 。
     * 你 的 代 码 应 该 对 任 何 字 长w≥ 8 都能工作。我们给出了当x=0x87654321 以及w= 32 时表达式求值的结果， 仅供参考。
        * A. x 的最低有效字节，其他位均置为0。[Ox00000021]。
            * or_operation(0x87654321);
        * B. 除了x的最低有效字节外，其他的位都取补，最低有效字节保持不变。[Ox789ABC21]。
            * 取补是取反再加1
        * C. x的最低有效字节设置成全1，共他字节都保持不变。[Ox876543EF]
            * uint32 nn = 0x87654321;
            * or_operation(nn);
     */

// 2.13
    /*
     *从20 世纪70年代末到80 年代末，Digital Equipment 的VAX计算机 是一种非常流行的机型。它没有布尔运算AND和OR指令 ，
     * 只有bis(位设置)和 bi c(位清除)这两种指令。两种指令的输入都是一个数据字x 和一个掩码字m。
     * 它们 生成一个结果z，2是由根据掩码m的位来修改x的位得到的。使用bis 指令，这种 修改就是在m为1的每个位置上，将z 对应的位设置为1。
     * 使用bic 指令，这种修改 就是在m为1 的每个位置，将z 对应的位设置为0。
     * 为了看清楚这些运算与C语官位级运算的关系，假设我们有两个函数bis 和bi c来实 现位设置和位清除操作。
     * 只想用这两个函数，而不使用任何其他C 语言运算，来实现按 位 | 和^ 运 算 。
     * 填 写 下列 代 码 中 缺 失 的 代 码 。 提 示 : 写 出 b i s 和 b i c 运 算 的 C 语 言 表 达
        int x = 0b10101010;
        int m = 0b11110000;
        int s = bic(x,m);
        hex_to_binary((uint32)s);
    */

// 2.14
    /*
     * 假 设 x 和 y 的 字 节 值 分 别 为 O x 6 6 和 O x 3 9
        * contrast();
    */

// 2.15
    /*
     * 只使用位级和還辑运算，编写一个C表达式，它等价于x==y。
     * 换句话 说，当x 和Y相等时它将返回1，否则就返回0
        int r = is_equal(3, 3);
        printf("%d", r);
    */

// 2.16
    /*
     * 将下列十六进制的转化成二进制，然后进行位移运算，最后再转回十六进制
        // 二进制: 1100 0011  num1<<3: 0001 1000  16进制: 0x18  num1>>2(逻辑): 0011 0000 16进制: 0x30 num1>>2(算数): 1111 0000 16进制: 0xF0
            int num1 = 0xC3;
        // 二进制: 0111 0101  num2<<3: 1010 1000  16进制: 0xA8  num2>>2(逻辑): 0001 1101 16进制: 0x1D num2>>2(算数): 0001 1101 16进制: 0x1D
            int num2 = 0x75;
        // 二进制: 1000 0111  num3<<3: 0011 1000  16进制: 0x38  num3>>2(逻辑): 0010 0001 16进制: 0x21 num3>>2(算数): 1110 0001 16进制: 0xE1
            int num3 = 0x87;
        // 二进制: 0110 0110  num4<<3: 0011 0000  16进制: 0x30  num4>>2(逻辑): 0001 1001 16进制: 0x19 num4>>2(算数): 0001 1001 16进制: 0x19
            int num4 = 0x66;
    */
// 2.17
/*
 * 假设w= 4，我们能给每个可能的十六进制数字赋予一个数值，假设用 一个无符号或者补码表示。
 * 请根据这些表示，通过写出等式(2. 1)和等式(2. 3)所示的 求和公式中的2 的非零次幂，填写下表:
    // 二进制: 0000 || B2U4(无符号): 0*2^3 + 0*2^2 + 0*2^1 + 0*2^0 = 0  || B2T4(补码): -0*2^3 + -0*2^2 + -0*2^1 + -0*2^0 = 0
    int num1 = 0x0;
    // 二进制: 0101 || B2U4(无符号): 0*2^3 + 1*2^2 + 0*2^1 + 1*2^0 = 5  || B2T4(补码): -0*2^3 + -1*2^2 + -0*2^1 + -1*2^0 = -3 （做错，最高位是0不是1，所以不是负数，答案应该是5）
    int num2 = 0x5;
    // 二进制: 1000 || B2U4(无符号): 1*2^3 + 0*2^2 + 0*2^1 + 0*2^0 = 8  || B2T4(补码): -1*2^3 + -0*2^2 + -0*2^1 + -0*2^0 = -8
    int num3 = 0x8;
    // 二进制: 1101 || B2U4(无符号): 1*2^3 + 1*2^2 + 0*2^1 + 1*2^0 = 13  || B2T4(补码): -1*2^3 + -1*2^2 + -0*2^1 + -1*2^0 = -3
    int num4 = 0xD;
    // 二进制: 1111 || B2U4(无符号): 1*2^3 + 1*2^2 + 1*2^1 + 1*2^0 = 15  || B2T4(补码): -1*2^3 + -1*2^2 + -1*2^1 + -1*2^0 = -1
    int num5 = 0xF;

    int32_t x = -123;
    uint64_t y = 1234567890123456789ULL;
    // 下面两个打印是相同的，但是使用宏能保证:不论代码是如何被编译的，都能生成正确的格式字籽串
    printf("x = %" PRId32 ", y = %" PRIu64 "\n", x, y);
    printf("x = %d, y = %lu\n", x, y);
*/
//    short x = 12345;
//    short mx = -x;
//    show_bytes((byte_pointer)&x, sizeof(short));
//    show_bytes((byte_pointer)&mx, sizeof(short));
//    uint32 x = 0x3039;  // 0011000000111001
//    hex_to_binary(x);
//    uint32 mx = 0xcfc7; // 1100111111000111
//    hex_to_binary(mx);

// 2.18
/*
 * 在第3章中，我们将看到由反汇编器生成的列表，反汇编器是一种将可 执行程序文件转换回可读性更好的ASCII 码形式的程序。
 * 这些文件包含许多十六进制 数字，都是用典型的补码形式来表示这些值。
 * 能够认识这些数字并理解它们的意义 (例如它们是正数还是负数)，是一项重要的技巧。
 * 在 下 面 的 列 表 中 ， 对 于 标 号 为 A ~ I (标 记 在 右 边 )的 那 些 行 ，
 * 将 指 令 名 (s u b 、 m o v 和add)右边显示的(32 位补码形式表示的)十六进制值转换为等价的十进制值
     * 4004d0: 48 81 ес e0 02 00 00    sub     $0x2e0,%rsp               A. 0010 1110 0000 = 2^9 + 2^7 + 2^6 + 2^5 = 736
     * 4004d7: 48 8 44 24 a8           mov     -0x58 (%rsp) ,%rax        B. 0101 1000 = 2^6 + 2^4 + 2^3 = -88
     * 4004dc: 48 03 47 28             add     0x28(%rdi) ,%rax          C. 0010 1000 = 2^5 + 2^3 = 40
     * 4004e0: 48 89 44 24 do          mov     rax, -0x30 (%rsp)         D. 0011 0000 = 2^5 + 2^4 = -48
     * 4004e5: 48 8b 44 24 78          mov     0x78(%rsp) ,%rax          E. 0111 1000 = 2^6 + 2^5 + 2^4 + 2^3 = 120
     * 4004ea: 48 89 87 88 00 00 00    mov     %.rax,0x88(%rdi)          F. 1000 1000 = 2^7 + 2^3 = 136
     * 4004f1: 48 8b 84 24 f8 01 00    mov     0x1f8 (%rsp) ,%rax        G. 0001 1111 1000 = 2^8 + 2^7 + 2^6 + 2^5 + 2^4 + 2^3 = 504
     * 400418: 00
     * 4004£9: 48 03 44 24 08          add     0x8(%rsp) ,%rax
     * 4004fe: 48 89 84 24 c0 00 00    mov     %rax,0xc0 (%rsp)          H. 1100 0000 = 2^7 + 2^6 = 192
     * 400505: 00
     * 400506: 48 8b 44 d4 b8          mov     -0x48(%rsp,%rdx,8),%rax   I. 0100 1000 = 2^6 + 2^3 = -72
*/
//    short int v = -12345;
//    unsigned short uv = (unsigned short) v;
//    printf("v = %d, uv = %u\n", v, uv);
//    unsigned u = 4294967295u;
//    int tu = (int)u;
//    printf("u = %u, tu = %d\n", u, tu);

// 2.19
    /*
         * 补码    无符号数(T2U4 = 2^4=16): T2Uw这个w等于几，就是加 2^w次方 的值
         * -8       8
         * -3       13
         * -2       14
         * -1       15
         * 0        16 (做错 0)
         * 5        21 (做错 5)
    */
//// 显式的强制类型转换
//int tx, ty;
//unsigned ux, uy;
//tx = (int)ux;
//uy - (unsigned)ty;
//// 隐性转换
//int tx, ty;
//unsigned ux, uy;
//tx = ux;
//uy = ty;
// %d: 有符号十进制 %u: 无符号十进制 %x: 十六进制
//    int x = -1;
//    unsigned u = 2147483648;
//    printf("x 无符号十进制 = %u , 有符号十进制 = %d\n", x, x);  // x 无符号十进制 = 4294967295 , 有符号十进制 = -1
//    printf("u 无符号十进制 = %u , 有符号十进制 = %d\n", u, u);  // u 无符号十进制 = 2147483648 , 有符号十进制 = -2147483648

// 2.21
/* 假设在采用补码运算的32 位机器上对这些表达式求值，按照图2-19 的 格式填写下表 ，描述强制类型转换和关系运算的结果
 *           表达式                                 类型                              求值
 *    -2147483647-1 == 2147483648U                无符号                               1
 *    关键点:
 *          1. 有符号整数和无符号整数比较时，有符号整数会被提升为无符号整数
 *          2. -2147483648 转换为无符号整数时，会变为其补码的无符号解释，即 2147483648U
 *          3. 最终结果是 1，表示表达式为 true
 *
 *    -2147483647-1 < 2147483647                  有符号                               1
 *    关键点:
 *          1. 2147483647 是一个有符号整数常量，类型是 int,取值范围[-2^31, 2^31 - 1] 即 [-2147483648, 2147483647]
 *          2. -2147483648 是一个有符号整数常量，类型是 int, 32 位有符号整数类型 int 的最小值。
 *
 *    -2147483647-1U < 2147483647                 无符号                               0
 *    关键点:
 *          1. -2147483647 是有符号值，当被提升为 unsigned int 时，它被解释为无符号整数 2^32 - 2147483647 = 2147483649
 *              1U是无符号值， 值为1，-2147483647-1U是一个运算表达式，所以需要先将有符号提升至无符号
 *              表达式就变为: 2147483649U - 1U :  2147483649 − 1 = 2147483648
 *          2. 2147483648 < 2147483647做对比， 结果为false
 *
 *    -2147483647-1 < -2147483647                 有符号                               1
 *    关键点:
 *          1.-2147483648 <  -2147483647, 两个都是有符号的，结果为true
 *
 *    -2147483647-1U < - 2147483647               无符号                               1
 *    关键点:
 *          1. -2147483647-1U : -2147483647是有符号， 1U是无符号，这个表达式需要先将有符号转化为无符号
 *          2. -2147483647转化为无符号: 2^32 - 2147483647 = 2147483649U
 *          3. 2147483649U - 1U = 2147483648U
 *          4. 右侧-2147483647是int，也要提升为unsigned int无符号值，−2147483647 → 2147483649U
 *              4.1: 2147483647 的二进制01111111 11111111 11111111 11111111
 *              4.2: 取反后： 10000000 00000000 00000000 00000000
 *              4.3: 加 1 后： 10000000 00000000 00000000 00000001
 *              4.4: 最终得到-2147483647的二进制
 *              而如果从-2147483647反推，提升到无符号
 *                  4.4.1: -2147483647的二进制补码 10000000 00000000 00000000 00000001
 *                  4.4.2: 因为无符号数没有符号位，所以直接转化为正数 2147483649
 *          5. 2147483648U < 2147483649U 所以结果为true
 * */
}




