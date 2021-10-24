#include <iostream>
#include "vui.creater.hpp"
using namespace std;
using namespace vui;
int main(int, char**) {
    auto c=VUICreater();
    struct VUIElement e;
    c.AddObject<string>(e,string("cnmd"),"nm");
    cout<<e.outSelf()<<endl;


    

    

    
    

}
