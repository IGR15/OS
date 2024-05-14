#include <iostream>
#include <ctime>
using namespace std;
int cal (int x ,int y ){
    int g= x*y;
    return g;
}

int main()
{
 int x;
 int y;
 cout<< "inter x\n";
 cin>>x;
  cout<< "inter y\n";
 cin>>y;
 cal (x,y);
 cout<<cal;

return 0;
}