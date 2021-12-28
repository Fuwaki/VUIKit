#include <iostream>
#include "vui.creater.hpp"
using namespace std;
using namespace VUI::Creater;
int main()
{
    Manager test(L"VUI");
    test.addElement(L"fuck",L"a", VUITypes::Str);
    test.addElement(L"Endermo", L"114514", VUITypes::Number);
    Object* temp;
    temp = test.addObject(L"Null");
    test.addElement(L"fly", L"awa", VUITypes::Str, temp);
    temp=test.addObject(L"fuck",temp);
    test.addElement(L"fly",L"awa", VUITypes::Str,temp);
    wcout<<test.outString()<<endl;
}
