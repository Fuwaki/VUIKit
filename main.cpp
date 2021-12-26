#include <iostream>
#include "vui.creater.hpp"
using namespace std;
using namespace VUI::Creater;
int main()
{
    Manager test;
    for (int i = 0; i < 1000000; i++) {
        test.addNum("test", i);
    }
    test.outString();
}
