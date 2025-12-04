#include <iostream>
using namespace std;


void displaySystem(){

cout <<"\t********************* Welcome To Student System *********************"<<endl<<endl;
   cout << endl;
   cout <<"Please enter the process you need ! "<<endl;
   cout <<"1- About Student" <<"\t" <<"2- About Course"<<endl;
   cout <<"3- About Teacher" <<"\t" <<"4- Exit" << endl;
}

void showProcesses(string s){

  cout <<"1- Add "<< s <<"\t\t" <<"2- Remove "<< s <<endl;
  cout <<  "3- Edit "<< s << "\t\t" << "4- Show " << s <<" Info" <<endl;
}


int main()
{


displaySystem();

int process;
bool flag = true;

 while (flag){
      cin >> process;

    switch (process){
        case 1: showProcesses("Student");
            break;
        case 2: showProcesses("Course");
            break;
        case 3: showProcesses("Teacher");
            break;
        case 4: cout <<"Exit"<<endl;
          flag = false;
            break;
        default: cout << "Invalid"<<endl;

    }


 }


    return 0;
}
