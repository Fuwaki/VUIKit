#include <iostream>
#include "vui.creater.hpp"
using namespace std;
using namespace VUI::Creater;
int main()
{
    Manager test("VUI");
    test.addElement("fuck", "a", VUITypes::Str);
    test.addElement("Endermo", "114514", VUITypes::Number);
    Object* temp;
    temp = test.addObject("Null");
    test.addElement("fly", "awa", VUITypes::Str, temp);
    temp=test.addObject("fuck",temp);
    test.addElement("fly","awa", VUITypes::Str,temp);
    

    cout<<test.outString()<<endl;
}
