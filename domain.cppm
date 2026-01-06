export module Domain;
import std;

export class Course {
public:

    std::string id;
    std::string name;
    int credit;
    int maxCapacity;
    int currentEnrollment;
    std::string teacherId; // 授课教师ID

    Course(std::string _id, std::string _name, int _credit, int _max, std::string _teacherId = "")
        : id(_id), name(_name), credit(_credit), maxCapacity(_max), currentEnrollment(0), teacherId(_teacherId) {}

    std::string toString() const {
        std::string tInfo = teacherId.empty() ? "待定" : teacherId;
        return std::format("课程ID: {} | 名称: {} | 学分: {} | 容量: {}/{} | 教师: {}",
            id, name, credit, currentEnrollment, maxCapacity, tInfo);
    }


    bool isFull() const { return currentEnrollment >= maxCapacity; }


    bool enroll() {
        if (isFull()) return false;
        currentEnrollment++;
        return true;
    }


    void drop() {
        if (currentEnrollment > 0) currentEnrollment--;
    }
};

export class Student {
public:
    std::string id;
    std::string name;
    std::vector<std::string> enrolledCourseIds;
    std::unordered_map<std::string, double> courseGrades;

    Student(std::string _id, std::string _name) : id(_id), name(_name) {}

    bool isEnrolled(const std::string& courseId) const {
        return std::ranges::find(enrolledCourseIds, courseId) != enrolledCourseIds.end();
    }
};

export class Teacher {
public:
    std::string id;
    std::string name;
    std::vector<std::string> teachingCourseIds;

    Teacher(std::string _id, std::string _name) : id(_id), name(_name) {}

    bool isTeaching(const std::string& courseId) const {
        return std::ranges::find(teachingCourseIds, courseId) != teachingCourseIds.end();
    }
};
