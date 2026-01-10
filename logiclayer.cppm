export module LogicLayer;
import Domain;
import DataLayer;
import std;

export class CourseController {
private:
    std::shared_ptr<IDataAccess> db;

public:
    CourseController(std::shared_ptr<IDataAccess> dataAccess) : db(dataAccess) {}

    // 通用查询
    std::vector<Course*> listCourses() { return db->retrieveAllCourses(); }
    std::vector<Teacher*> listTeachers() { return db->retrieveAllTeachers(); }

    // 秘书功能
    std::string assignTeachingTask(std::string courseId, std::string teacherId) {
        if (db->assignTeacher(courseId, teacherId)) {
            return "排课成功！";
        }
        return "排课失败：请检查ID是否正确。";
    }

    //  学生功能
    Student* findStudent(std::string id) { return db->findStudent(id); }

    std::string enrollCourse(std::string sId, std::string cId) {
        Student* s = db->findStudent(sId);
        Course* c = db->findCourse(cId);

        if (!s) return "学生ID错误";
        if (!c) return "课程ID错误";

        if (c->isFull()) return "选课失败：课程人数已满";
        if (s->isEnrolled(cId)) return "选课失败：您已选修该课程";

        if (db->enrollStudent(sId, cId)) {
            c->currentEnrollment++;
            s->enrolledCourseIds.push_back(cId);
            s->courseGrades[cId] = -1.0;
            return "选课成功！";
        }
        return "选课失败：数据库写入错误。";
    }

    std::string dropCourse(std::string sId, std::string cId) {
        Student* s = db->findStudent(sId);
        if (!s || !s->isEnrolled(cId)) return "退课失败：未找到选课记录";

        if (db->dropCourse(sId, cId)) {
            std::erase(s->enrolledCourseIds, cId);
            s->courseGrades.erase(cId);
            return "退课成功！";
        }
        return "退课失败。";
    }

    // 教师功能
    Teacher* findTeacher(std::string id) { return db->findTeacher(id); }

    std::vector<Course*> fetchTeacherCourses(std::string tId) {
        return db->fetchTeachingCourses(tId);
    }

    std::vector<Student*> fetchCourseStudents(std::string cId) {
        return db->fetchCourseStudents(cId);
    }

    std::string updateGrade(std::string tId, std::string cId, std::string sId, double score) {
        Course* c = db->findCourse(cId);
        if (!c) return "课程不存在";
        if (c->teacherId != tId) return "权限拒绝：您不是该课程的讲师";
        if (score < 0 || score > 100) return "分数无效 (0-100)";

        if (db->updateStudentGrade(sId, cId, score)) {
            return "成绩录入成功";
        }
        return "录入失败：学生可能未选修该课";
    }
};
