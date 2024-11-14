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



















}




