export module LogicLayer;

import Domain;
import DataLayer;
import std;

export class CourseController {
private:
    std::shared_ptr<IDataAccess> db;

public:
    CourseController(std::shared_ptr<IDataAccess> dataAccess);

    // 添加缺失的成员函数
    std::vector<Course> getAvailableCourses();
    std::optional<Student> getStudentInfo(const std::string& studentId);  // 修正函数名
    bool enrollCourse(const std::string& studentId, const std::string& courseId);
    bool dropCourse(const std::string& studentId, const std::string& courseId);
};

// 实现部分
CourseController::CourseController(std::shared_ptr<IDataAccess> dataAccess)
    : db(dataAccess) {}

std::vector<Course> CourseController::getAvailableCourses() {
    return db->getAllCourses();
}

std::optional<Student> CourseController::getStudentInfo(const std::string& studentId) {
    return db->getStudentById(studentId);
}

bool CourseController::enrollCourse(const std::string& studentId, const std::string& courseId) {
    return db->enrollStudent(studentId, courseId);
}

bool CourseController::dropCourse(const std::string& studentId, const std::string& courseId) {
    return db->dropCourse(studentId, courseId);
}
