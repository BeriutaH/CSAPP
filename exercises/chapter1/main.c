#include <stdio.h>
#include <math.h>


void speedup_ratio() {
    /*
     假设你是个卡车司机，要将土豆从爱达荷州的Boise 运送到明尼苏达州 的Minneapolis，全程2500公里。
     在限速范围肉，你估计平均速度为100公里/ 小时， 整个行程需要2 5 个小时。
        A. 你听到新闻说蒙大拿州刚刚取消了限速，这使得行程中有1500公里卡车的速度可 以为150公里/ 小时。
        那么这对整个行程的加速比是多少?
        B. 你可以在www.fasttrucks.com网站上为自己的卡车买个新的涡轮增压器。网站现 货供应各种型号，
        不过速度越快，价格越高。如果想要让整个行程的加速比为 1.67X，那么你必须以多快的速度通过蒙大拿州
    */
    // A. 计算加速比
    double total_distance = 2500.0;
    double old_speed = 100.0;
//    double new_speed = 150.0;
    double montana_distance = 1500.0;  // 蒙大拿州的行程
    double elsewhere = total_distance - montana_distance;
    // 原本的总时间
    double T1 = total_distance / old_speed;
//    // 新时间
//    double T2 = (montana_distance / new_speed) + (elsewhere / old_speed);
//    // 计算加速比
//    double speedup = T1 / T2;
//    printf("T1=%.2f, T2=%.2f, 加速比=%.2f\n", T1, T2, speedup);
//
    // B. 计算速度
    double speedup = 1.67; // 目标加速比
    double T2 = T1 / speedup;  // 目标时间
    double new_speed = montana_distance / (T2 - elsewhere / old_speed);
    printf("T2=%.2f, new_speed=%.2f\n", T2, new_speed);
}


int main() {
    speedup_ratio();
    return 0;
}
