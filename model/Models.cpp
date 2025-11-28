#include <iostream>
using namespace std;

class BaseEntity {

 protected :
    string name;
    int age;
    string id;
    string phone_number;

 public :
    // Setters
    void setName(string name){
     this->name = name;
    }

    void setAge(int age){
     this->age = age;
    }

    void setId(string id){
     this->id = id;
    }

    void setPhoneNumber(string phone_number){
     this->phone_number = phone_number;
    }

    // Getters
    string GetName(){
     return name;
    }

    int GetAge(){
     return age;
    }

    string GetId(){
     return id;
    }

    string GetPhoneNumber(){
     return phone_number;
    }


};

class Teacher : public BaseEntity { // Teacher inherits from BaseEntity to reuse common attributes
  private :
      double Monthly_salary;
      string Teacher_subject;

  public :
    // Setters
    void setMonthlySalary(double Monthly_salary){
     this->Monthly_salary = Monthly_salary;
    }

    void setTeacherSubject(string Teacher_subject){
     this->Teacher_subject = Teacher_subject;
    }

    // Getters
    double GetMonthlySalary(){
     return Monthly_salary;
    }

    string GetTeacherSubject(){
     return Teacher_subject;
    }



};

class Student : public BaseEntity { // Student inherits from BaseEntity to reuse common attributes
  private :
      string school_year;
      double gpa;
      Teacher Teachers[6];


  public :
     // Setters
    void setSchoolYear(string school_year){
     this->school_year = school_year;
    }

    void setGpa(double gpa){
     this->gpa = gpa;
    }

    void SetTeachers(Teacher Teachers[6]){
       for (int i=0; i<6; i++){
         this->Teachers[i] = Teachers[i];
       }
    }

    // Getters
    string GetSchoolYear(){
     return school_year;
    }

    double GetGpa(){
     return gpa;
    }

    Teacher *GetTeachers(){
        return this->Teachers;
    }

};
