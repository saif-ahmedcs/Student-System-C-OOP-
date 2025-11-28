class Student {
  private :
      string name;
      int age;
      string id;
      string phone_number;
      string school_year;
      double gpa;


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

    void setSchoolYear(string school_year){
     this->school_year = school_year;
    }

    void setGpa(double gpa){
     this->gpa = gpa;
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

    string GetSchoolYear(){
     return school_year;
    }

    double GetGpa(){
     return gpa;
    }




};
