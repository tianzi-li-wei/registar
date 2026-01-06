export module LogicLayer;
import Domain;
import DataLayer;
import std;

export class CourseController {
private:
    std::shared_ptr<IDataAccess> db;

public:
    CourseController(std::shared_ptr<IDataAccess> dataAccess);


    std::vector<Course> getAvailableCourses();
    std::vector<Teacher> getAllTeachers();

    std::string assignTeachingTask(std::string courseId, std::string teacherId);


    std::optional<Student> getStudentInfo(const std::string& studentId);
    bool enrollCourse(const std::string& studentId, const std::string& courseId);
    bool dropCourse(const std::string& studentId, const std::string& courseId);


    std::optional<Teacher> getTeacherInfo(const std::string& teacherId);
    std::vector<Course> getTeachingCourses(const std::string& teacherId);
    std::vector<Student> getCourseStudents(const std::string& courseId);
    bool updateStudentGrade(const std::string& studentId, const std::string& courseId, double grade);
};

// 实现
CourseController::CourseController(std::shared_ptr<IDataAccess> dataAccess) : db(dataAccess) {}

std::vector<Course> CourseController::getAvailableCourses() {
    return db->getAllCourses();
}

std::vector<Teacher> CourseController::getAllTeachers() {
    return db->getAllTeachers();
}

std::string CourseController::assignTeachingTask(std::string courseId, std::string teacherId) {
    if (db->assignTeacher(courseId, teacherId)) {
        return "排课成功！";
    }
    return "排课失败：课程ID或教师ID不存在。";
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
