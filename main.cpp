#include <iostream>
#include "vui.creater.hpp"

using namespace std;
using namespace vui;
int main(int, char **)
{
    VUIObjectElement root;
    root.AddObject(123, "TESTNUM");
    root.AddObject("123", "TESTSTR");
    cout << root.OutSelf() << endl;
    return EXIT_SUCCESS;
}
