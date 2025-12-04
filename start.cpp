#include <iostream>
using namespace std;

int main()
{

cout <<"\t********************* Welcome To Student System *********************"<<endl<<endl;



int process;
bool flag = true;

 while (flag){
   cout << endl;
   cout <<"Enter Process : "<<endl<<endl;
   cout <<"1- Add Student"<<"\t\t"<<"2- Remove Student"<<endl<<endl;
   cout <<"3- Edit Student"<<"\t\t"<<"4- Show Student"<<endl<<endl;
   cout <<"5- Exit"<<endl;
      cin >> process;

    switch (process){
        case 1: cout <<"Add Student"<<endl;
            break;
        case 2: cout <<"Remove Student"<<endl;
            break;
        case 3: cout <<"Edit Student"<<endl;
            break;
        case 4: cout <<"Show Student"<<endl;
            break;
        case 5: cout <<"Exit"<<endl;
          flag = false;
            break;
        default: cout << "Invalid"<<endl;

    }


 }




    return 0;
}
