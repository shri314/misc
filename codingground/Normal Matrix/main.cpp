#include <iostream>
#include "matrix.cpp"

#include <string>
#include <iostream>
using namespace std;

int main()
{
    Matrix<int, 3, 2> s1 =
    { 
        { 2, 3 },
        { 4, 5 },
        { 6, 7 }
    };
    
    Matrix<int, 2, 3> s2 =
    { 
        { 1, 2, 3 },
        { 4, 5, 6 },
    };
    
    auto r = 2 * (s1 * s2) * (s1 * s2) * 2;
    cout << s1 << "\n";
    cout << s2 << "\n";
    cout << r << "\n";
    cout << "R:" << r(0,1) << ":R\n";

    for(unsigned long long i = 0 ; i < (unsigned long long)(-1); ++i)
    {
       auto r1 = 2 * (s1 * s2) * (s1 * s2) * 2;

       r1(0,1);
    }
}
