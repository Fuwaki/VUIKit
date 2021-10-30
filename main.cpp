#include <iostream>
#include <sstream>

using namespace std;

int main(int, char**) {
    auto c = stringstream();
    c << "cnmd";
    string a;
    c >> a;
    cout << a << endl;
    c >> a;
    c << "n";
    a.clear();
    cout << a << endl;
    c >> a;
    cout << a << endl;
    c >> a;
    cout << a << endl;
    c >> a;
    cout << a << endl;
    c >> a;
    cout << a << endl;
    c >> a;
    cout << a << endl;
}
