export module Domain;
import std;

export class Course {
public:
    std::string id;
    std::string name;
    int credit;
    int maxCapacity;
    int currentEnrollment;

    // 构造函数声明
    Course(std::string _id, std::string _name, int _credit, int _max);


    std::string toString() const;
};

export class Student {
public:
    std::string id;
    std::string name;
    std::vector<std::string> enrolledCourseIds;

    // 构造函数声明
    Student(std::string _id, std::string _name);
};
