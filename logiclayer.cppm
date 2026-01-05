export module LogicLayer;
import Domain;
import DataLayer;
import std;

export class CourseController {
private:
    std::shared_ptr<IDataAccess> db;

public:
    CourseController(std::shared_ptr<IDataAccess> dataAccess);

    // 学生相关功能
    std::vector<Course> getAvailableCourses();
    std::optional<Student> getStudentInfo(const std::string& studentId);
    bool enrollCourse(const std::string& studentId, const std::string& courseId);
    bool dropCourse(const std::string& studentId, const std::string& courseId);

    // 教师相关功能
    std::optional<Teacher> getTeacherInfo(const std::string& teacherId);
    std::vector<Course> getTeachingCourses(const std::string& teacherId);
    std::vector<Student> getCourseStudents(const std::string& courseId);
    bool updateStudentGrade(const std::string& studentId, const std::string& courseId, double grade);
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

std::optional<Teacher> CourseController::getTeacherInfo(const std::string& teacherId) {
    return db->getTeacherById(teacherId);
}

std::vector<Course> CourseController::getTeachingCourses(const std::string& teacherId) {
    return db->getTeachingCourses(teacherId);
}

std::vector<Student> CourseController::getCourseStudents(const std::string& courseId) {
    return db->getCourseStudents(courseId);
}

bool CourseController::updateStudentGrade(const std::string& studentId, const std::string& courseId, double grade) {
    return db->updateStudentGrade(studentId, courseId, grade);
}
