// calc24v2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "Calc24Server.h"

int main()
{
    Calc24Server calc24Server;
    if (!calc24Server.init(5, "0.0.0.0", 8888)) {
        return 0;
    }

    return 1;
}

