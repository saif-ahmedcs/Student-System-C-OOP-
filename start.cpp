#include <iostream>
#include "controller/Controller.h"
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

StudentRepositoryImpl studentrepo;
StudentServiceImpl studentservice(studentrepo);
StudentController studentController(studentservice);
displaySystem();

int process;
bool flag = true;

 while (flag){
      cin >> process;

    switch (process){
        case 1: showProcesses("Student");

           int studentProcess;
           cin >> studentProcess;

          if (studentProcess == 1){


            cout << "Please Enter Student Data :"<<endl;
            Student student;

            cout <<"Enter Student Name:"<<endl;
            string name;
            cin>>name;
            student.setName(name);

            cout <<"Enter Student id:"<<endl;
            string id;
            cin>>id;
            student.setId(id);

            cout <<"Enter Student gpa:"<<endl;
            double gpa;
            cin>>gpa;
            student.setGpa(gpa);

            cout <<"Enter Student school grade:"<<endl;
            int grade;
            cin>>grade;

            cout <<"Enter Student phone number:"<<endl;
            string phoneNumber;
            cin>>phoneNumber;

            student.setPhoneNumber(phoneNumber);

            cout <<studentController.addStudent(grade,student);

           } break;

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
