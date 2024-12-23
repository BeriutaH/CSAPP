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

// 位移
int fun1(unsigned word) {
    // 先位移，再转成int
    return (int)((word << 24) >> 24);
}
int fun2(unsigned word) {
    // 先转int，再位移
    return ((int) word << 24) >> 24;
}

float sum_elements(const float a[], unsigned length) {
    float result = 0;
    for (int i = 0; i < length - 1; i++) {
        result += a[i];
    }
    return result;
}

int str_longer(char *s, char *t) {
    return strlen(s) > strlen(t);
}

// 两个无符号数相加
int u_add_ok(unsigned x, unsigned y) {
    /* 可能会出现相加的结果溢出的, 如果产生溢出就返回0 */
    unsigned result = x + y;
    printf("result = %u\n", result);
//    return result > x || result > y;
    return result >= x;
}

// 两个有符号数相加，判断是否会溢出
int tadd_ok(int x, int y) {
    /* 有符号的溢出分为:
     * 正溢出: 当两个操作数都是正数时，如果它们的和变成了负数，就发生了正溢出
     * 负溢出: 当两个操作数都是负数时，如果它们的和变成了正数，就发生了负溢出
     * */
    int flag = 0;
    int result = x + y;
    if ((x > 0 && y > 0 && result < 0) || (x < 0 && y < 0 && result > 0)) {
        flag = 1;  // 溢出
    }
    printf("result = %d\n", result);
    return flag;
}

// 为什么要笑
int two_add_ok(int x, int y) {
    int sum = x + y;
    printf("sum - x = %d\n", sum - x);
    printf("sum - y = %d\n", sum - y);
    printf("sum  %d\n", sum);
    return (sum - x == y) && (sum - y == x);
}

// 减法溢出检测
int t_sub_ok(int x, int y) {
    if (x == INT_MIN && y == 1) {
        return 1;  // 溢出，因为 INT_MIN - 1 会溢出
    }
    return tadd_ok(x, -y);  // 其他情况正常调用加法溢出检测
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
    int num1 = 0x0;
    // 二进制: 0000 || B2U4(无符号): 0*2^3 + 0*2^2 + 0*2^1 + 0*2^0 = 0  || B2T4(补码): 0*2^3 + 0*2^2 + 0*2^1 + 0*2^0 = 0
    int num2 = 0x5;
    // 二进制: 0101 || B2U4(无符号): 0*2^3 + 1*2^2 + 0*2^1 + 1*2^0 = 5  || B2T4(补码): 0*2^3 + 1*2^2 + 0*2^1 + 1*2^0 = -3 （做错，最高位是0不是1，所以不是负数，答案应该是5）
    int num3 = 0x8;
    // 二进制: 1000 || B2U4(无符号): 1*2^3 + 0*2^2 + 0*2^1 + 0*2^0 = 8  || B2T4(补码): -1*2^3 + 0*2^2 + 0*2^1 + 0*2^0 = -8
    int num4 = 0xD;
    // 二进制: 1101 || B2U4(无符号): 1*2^3 + 1*2^2 + 0*2^1 + 1*2^0 = 13  || B2T4(补码): -1*2^3 + 1*2^2 + 0*2^1 + 1*2^0 = -3
    int num5 = 0xF;
    // 二进制: 1111 || B2U4(无符号): 1*2^3 + 1*2^2 + 1*2^1 + 1*2^0 = 15  || B2T4(补码): -1*2^3 + 1*2^2 + 1*2^1 + 1*2^0 = -1

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
     *------------------------------------------------------------------------------------------------------------------
     *    -2147483647-1 == 2147483648U                无符号                               1
     *    关键点:
     *          1. 有符号整数和无符号整数比较时，有符号整数会被提升为无符号整数
     *          2. -2147483648 转换为无符号整数时，会变为其补码的无符号解释，即 2147483648U
     *          3. 最终结果是 1，表示表达式为 true
     *------------------------------------------------------------------------------------------------------------------
     *    -2147483647-1 < 2147483647                  有符号                               1
     *    关键点:
     *          1. 2147483647 是一个有符号整数常量，类型是 int,取值范围[-2^31, 2^31 - 1] 即 [-2147483648, 2147483647]
     *          2. -2147483648 是一个有符号整数常量，类型是 int, 32 位有符号整数类型 int 的最小值。
     *------------------------------------------------------------------------------------------------------------------
     *    -2147483647-1U < 2147483647                 无符号                               0
     *    关键点:
     *          1. -2147483647 是有符号值，当被提升为 unsigned int 时，它被解释为无符号整数 2^32 - 2147483647 = 2147483649
     *              1U是无符号值， 值为1，-2147483647-1U是一个运算表达式，所以需要先将有符号提升至无符号
     *              表达式就变为: 2147483649U - 1U :  2147483649 − 1 = 2147483648
     *          2. 2147483648 < 2147483647做对比， 结果为false
     *------------------------------------------------------------------------------------------------------------------
     *    -2147483647-1 < -2147483647                 有符号                               1
     *    关键点:
     *          1.-2147483648 <  -2147483647, 两个都是有符号的，结果为true
     *------------------------------------------------------------------------------------------------------------------
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
     *------------------------------------------------------------------------------------------------------------------
     */
//    short sx = -12345;
//    unsigned short usx = sx;
//    int x = sx;
//    unsigned ux = usx;
//    printf("sx = %d:\t", sx);
//    show_bytes((byte_pointer)&sx, sizeof(short));
//    printf("usx = %u:\t", usx);
//    show_bytes((byte_pointer)&usx, sizeof(unsigned short));
//    printf("x = %d:\t", x);
//    show_bytes((byte_pointer)&x, sizeof(int));
//    printf("ux = %u\t", ux);
//    show_bytes((byte_pointer)&ux, sizeof(unsigned));

// 2.22
    /* 通过应用等式(2. 3)，表明下面每个位向量都是一5 的补码表示
         * A. [1011] = -2^3 + 2^1+ 2^0 = -8+2+1 = -5
         * B. [11011] = -2^4 + 2^3 + 2^1+ 2^0 = -16+8+2+1 = -5
         * C. [111011] = -2^5 + 2^4 + 2^3 + 2^1+ 2^0 = -32+16+8+2+1 = -5
    */

//    short sx = -12345;
//    unsigned uy = sx;  // (unsigned)sx 等价于 (unsigned)(int)sx 而不是 (unsigned)(unsigned short)sx
//    printf("uy = %u:\t", uy);  // uy = 4294954951:	c7cfffff
//    show_bytes((byte_pointer)&uy, sizeof(unsigned));

// 2.23
    /* 假设在一个采用补码运算的机器上以32 位程序来执行这些函数。(补码:最高位权重为1是负数)
     * 还假设有符号数值 的右移是算术右移，（最高位（符号位）保持不变，其他位向右移动，空出的位置用符号位的值填充（如果是正数用 0，负数用 1））
     * 而无符号数值的右移是逻辑右移，（所有位（包括符号位）都向右移动，空出的位置统一用 0 填充）
         * A. 填写下表，说明这些函数对几个示例参数的结果。
             * 你会发现用十六进制表示来做会更方便，
             * 只要记住十六进制数字8 到F 的最高有效位等于1
             *     w                 fun1(w)保留后两位最高位加0                   fun2(w)保留最后两位，最高位加1
             * 0x00000076              0x00000076                                       0x00000076
             * 0x87654321              0x00000021                                       0xFFFFFF21
             * 0x000000c9              0x000000c9                                       0x000000c9
             * 0xEDCBA987              0x00000087                                       0xFFFFFF87
             * 这题做成换算十进制了，题目都没看懂……呜呜呜呜呜，下面找的一个十六进制换算十进制的方法
             * 0xEDCBA987        14*16^7 + 13*16^6 + 12*16^5 + 11*16^4 + 10*16^3 + 9*16^2 + 8*16^1 + 7*16^0 = 3989547399
             *
         * B. 函数funl从参数的低8位中提取一个值,得到范围0-255的一个整数。
         * 函数fun2也从这个参数的低8位中提取一个值,但是它还要执行符号扩展。结果将是介于一128-127的一个数
    */
    // 截断数字
    //    int x = 53191;
    //    short sx = (short)x;  // 32位的int截断为了16位的short int -12345
    //    int y = sx;  // -12345
    //    printf("%d", y);
    /*
     * 变量 x = 53191 是一个 32 位整数。当将 x 强制转换为 16 位的 short 类型时，发生了截断。
     * 截断只保留低 16 位，高 16 位被丢弃
         * 32 位整数 x = 53191 在二进制中的表示为： 0000 0000 0000 0000 1100 1111 1001 0111
         * 截断后只保留低 16 位：1100 1111 1001 0111
     * 此时，最高位是 1，表示这是一个负数。
     * 解释截断后的值：
        由于最高位是符号位，剩下的 16 位表示一个 16 位的补码数。
        计算其对应的十进制数：
            取反加一：1100 1111 1001 0111 → 0011 0000 0110 1001（取反） → 0011 0000 0110 1010（加一）。
        结果是 12345。
        所以，补码表示为负数 -12345
     * 因此，(short)x 的值是 -12345
     *
     * 当将 sx 赋值给 y 时，计算机会将 sx 扩展回 32 位。这时，会进行符号扩展：
     *     因为 sx 是一个负数（补码表示为 -12345），所以符号扩展会用符号位（1）填充高位
     *          1111 1111 1111 1111 1100 1111 1001 0111  (经过取反加一之后)这个值仍然是 -12345
     *
     * 注意: 当一个 16 位数扩展为 32 位时，高 16 位只是符号位的填充，用于保持数值的符号一致性。
     *      高位的内容不属于实际的数值部分，因此在进行取反加一还原为原码时，只需操作原来的有效位（低 16 位）
     * */

// 2.24
    /*
     * 假 设 将 一 个 4 位 数 值 (用 十 六 进 制 数 字 0 ~ F 表 示 )截 断 到 一 个 3 位 数 值 (用十六进制数字0~ 1 表示)。
     * 填写下表，根据那些位模式的无符号和补码解释，说明这种截断对某些情况的结果
     *       无符号                                              补码
     *  原始值        截断值                                  原始值   截断值
     *    0            0                                      0       0
     *    2            2                                      2       2
     *    9            1                                     -7       1
     *   11            3                                     -5       3
     *   15            7                                     -1      -1
     *
     * 注意：补码范围[−2^n−1,2^n−1 −1], n=3 所以补码范围是[-2^3-1, 2^3-1 -1] = [-4, 3]
    */

// 2.25
    /* 考虑下列代码，这段代码试图计算数组a 中所有元素的和，其中元素的 数量由参数 length 给出
     * 因为length是无符号，进入循环，先取值a[0]的第一位，所以会造成在原本没有的数据上取数据，会造成内存错误
     * 修改：
            * 有两种方洼可以改正这段代码
            * 其一是将length声明为int类型
            * 其二是将for循环的测试条件改为i<length
        const float empty_list[] = {};
        float result = sum_elements(empty_list,0);
        printf("result = %.2f", result);
    */

// 2.26
/*这个例子说明了无符号运算的一个细微的特性,同时也是我们执行无符号运算时不会意识到的属性。
 * 这会导致一些非常棘手的错误
    * A.在什么情况下,这个函数会产生不正确的结果?
        * s 比 t的长度短的时候
    * B.解释为什么会出现这样不正确的结果。
        * strlen函数返回的size_t是无符号类型，无符号-无符号如果出现负数，则会出现错误
    * C.说明如何修改这段代码好让它能可靠地工作
        * 因为都是无符号，可以直接对比大小就好 改成 strlen(s) > strlen(t)
    char *s1 = "H";
    char *s2 = "Nice";
    int num = str_longer(s1, s2);
    printf("num = %d", num);
*/

// 2.27
    /*int uadd_ok(unsigned x, unsigned y);
     * 如果参数x 和Y相加不会产生溢出，这个函数就返回1
     * 无符号数相加，如果溢出，那么溢出值永远比比两个加数的任意一个数小
        unsigned a = 4294967294;  // 32位无符号值最大值4294967295
        unsigned b = 18;
        int result =  u_add_ok(b, a);
        printf("a + b 没有溢出: %d", result);
    */

// 2.28
    /* 我们能用一个十六进制数字来表示长度w= 4 的位模式。对于这些数字 的无符号解释，
     * 使用等式(2. 12)填写下表，给出所示数字的无符号加法逆元的位表示 (用十六进制形式)
         * u 是无符号数字的位宽，它没有符号位（即没有正负之分）
         * 无符号加法逆元：由x找到-u_4x这个数，使 x+(-u_4x) = 0 mod 2^w
         * 已知 w = 4, 所以是mod 16, -u_4x设为y
         *      等式就变为 x + y = 0 mod 16，表示 x 和 y 的和在除以 16 时，余数为 0
    ---------------------------------------------------------------
     *           x                              -u/4 x
    ---------------------------------------------------------------
     *   十六进制      十进制              十进制      十六进制
    ------------------------------|---------------------------------
     *   0              0                 0           0
     *   5              5                 11          B
     *   8              8                 8           8
     *   D              13                3           3
     *   F              15                1           1
    */

// 2.29
/* 按照图2-25 的形式填写下表。分别列出5 位参数的整数值、整数与补码和的数值、补码和的位级表示，
 * 以及属于等式(2. 13)推导中的哪种情况。
 * 补码特性: 最大正数的绝对值总是小于最小负数的绝对值
 *        x              y                   x+y               x+1/5y(截取5位)               情况
 *  [10100] -12     [10001] -15         [100101] -21             [00101] 5               1 负溢出
 *  [11000] -8      [11000] -8          [110000] -16             [10000] -16            （做错，应该是2正常）1 负溢出
 *  [10111] -9      [01000] 8           [111111] -1              [11111] -1              2 正常
 *  [00010] 2       [00101] 5           [000111] 7               [00111] 7               3 正常
 *  [01100] 12      [00100] 4          [010000] 16              [10000] -16             4 正溢出
 * */

// 2.30
/* 校验补码加法不会溢出，如果溢出就返回1，不会就返回0
    int n1 = 2147483647;
    int n2 = 1;
    printf("是否溢出：%d", tadd_ok(n1, n2));
 */

// 2.31
/*
    int n1 = 2147483647;
    int n2 = 1;
    printf("%d", two_add_ok(n1, n2));
    因为阿贝尔群有以下特征:
        1. 封闭性：模2^n运算保证结果始终在 n-位 范围内
        2. 单位元：0 是加法单位元
        3. 逆元：补码设计保证了每个数都有唯一的逆元, 取反 + 1
        4. 结合律：加法满足结合律，无论溢出与否
        5. 交换律：硬件加法器保证交换性
    所以，虽然当前函数符合阿贝尔群，但是并不能校验是否溢出， 这会导致结果永远都等于1
*/

// 2.32
/* x取最小负数，y取-1，两个数相加，就会产生错误结果
    int n1 = INT_MIN;
    int n2 = -1;
    int n1 = -100;
    int n2 = -2147483647;
    int n1 = INT_MIN;
    int n2 = 1;  // 发生溢出
    printf("%d", t_sub_ok(n1, n2));
*/
}




