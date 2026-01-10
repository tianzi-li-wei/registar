module;

#include <pqxx/pqxx>
export module DataLayer;
import Domain;
import std;

export class IDataAccess {
public:
    virtual ~IDataAccess() = default;


    virtual std::vector<Course*> retrieveAllCourses() = 0;
    virtual std::vector<Teacher*> retrieveAllTeachers() = 0;

    virtual Student* findStudent(const std::string& id) = 0;
    virtual Teacher* findTeacher(const std::string& id) = 0;
    virtual Course* findCourse(const std::string& id) = 0;

    virtual std::vector<Course*> fetchTeachingCourses(const std::string& teacherId) = 0;
    virtual std::vector<Student*> fetchCourseStudents(const std::string& courseId) = 0;


    virtual bool assignTeacher(const std::string& cId, const std::string& tId) = 0;
    virtual bool enrollStudent(const std::string& sId, const std::string& cId) = 0;
    virtual bool dropCourse(const std::string& sId, const std::string& cId) = 0;
    virtual bool updateStudentGrade(const std::string& sId, const std::string& cId, double grade) = 0;
};

export class PostgresDB : public IDataAccess {
private:
    std::string connStr;
    std::vector<Course> courseCache;
    std::vector<Teacher> teacherCache;
    std::vector<Student> studentCache;

public:
    PostgresDB(std::string connectionString) : connStr(connectionString) {}


    std::vector<Course*> retrieveAllCourses() override {
        courseCache.clear();
        try {
            pqxx::connection c(connStr);
            pqxx::work w(c);
            pqxx::result r = w.exec("SELECT id, name, credit, max_capacity, teacher_id FROM courses ORDER BY id");

            for (auto row : r) {
                std::string tId = row["teacher_id"].is_null() ? "" : row["teacher_id"].c_str();
                courseCache.emplace_back(
                    row["id"].c_str(), row["name"].c_str(),
                    row["credit"].as<int>(), row["max_capacity"].as<int>(), tId
                );
            }
            for(auto& course : courseCache) {
                pqxx::result countRes = w.exec("SELECT COUNT(*) FROM enrollments WHERE course_id = " + w.quote(course.id));
                course.currentEnrollment = countRes[0][0].as<int>();
            }
        } catch (std::exception const &e) {
            std::cerr << "DB Error: " << e.what() << std::endl;
        }

        std::vector<Course*> ptrs;
        for(auto& item : courseCache) ptrs.push_back(&item);
        return ptrs;
    }

    std::vector<Teacher*> retrieveAllTeachers() override {
        teacherCache.clear();
        try {
            pqxx::connection c(connStr);
            pqxx::work w(c);
            pqxx::result r = w.exec("SELECT id, name FROM teachers ORDER BY id");
            for (auto row : r) {
                teacherCache.emplace_back(row["id"].c_str(), row["name"].c_str());
            }
        } catch (...) {}
        std::vector<Teacher*> ptrs;
        for(auto& item : teacherCache) ptrs.push_back(&item);
        return ptrs;
    }


    Student* findStudent(const std::string& id) override {
        std::erase_if(studentCache, [&](const Student& s){ return s.id == id; });
        try {
            pqxx::connection c(connStr);
            pqxx::work w(c);
            pqxx::result r = w.exec("SELECT name FROM students WHERE id = " + w.quote(id));
            if (r.empty()) return nullptr;

            studentCache.emplace_back(id, r[0]["name"].c_str());
            Student* s = &studentCache.back();

            pqxx::result enrollRes = w.exec("SELECT course_id, grade FROM enrollments WHERE student_id = " + w.quote(id));
            for (auto row : enrollRes) {
                std::string cId = row["course_id"].c_str();
                double grade = row["grade"].as<double>();
                s->enrolledCourseIds.push_back(cId);
                s->courseGrades[cId] = grade;
            }
            return s;
        } catch (...) { return nullptr; }
    }

    Teacher* findTeacher(const std::string& id) override {
        std::erase_if(teacherCache, [&](const Teacher& t){ return t.id == id; });
        try {
            pqxx::connection c(connStr);
            pqxx::work w(c);
            pqxx::result r = w.exec("SELECT name FROM teachers WHERE id = " + w.quote(id));
            if (r.empty()) return nullptr;
            teacherCache.emplace_back(id, r[0]["name"].c_str());
            return &teacherCache.back();
        } catch (...) { return nullptr; }
    }


    Course* findCourse(const std::string& id) override {
        std::erase_if(courseCache, [&](const Course& c){ return c.id == id; });
        try {
            pqxx::connection c(connStr);
            pqxx::work w(c);
            pqxx::result r = w.exec("SELECT name, credit, max_capacity, teacher_id FROM courses WHERE id = " + w.quote(id));
            if (r.empty()) return nullptr;

            std::string tId = r[0]["teacher_id"].is_null() ? "" : r[0]["teacher_id"].c_str();
            courseCache.emplace_back(id, r[0]["name"].c_str(), r[0]["credit"].as<int>(), r[0]["max_capacity"].as<int>(), tId);
            pqxx::result count = w.exec("SELECT COUNT(*) FROM enrollments WHERE course_id = " + w.quote(id));
            courseCache.back().currentEnrollment = count[0][0].as<int>();
            return &courseCache.back();
        } catch (...) { return nullptr; }
    }


    std::vector<Course*> fetchTeachingCourses(const std::string& teacherId) override {
        auto all = retrieveAllCourses();
        std::vector<Course*> result;
        for(auto* c : all) {
            if(c->teacherId == teacherId) result.push_back(c);
        }
        return result;
    }


    std::vector<Student*> fetchCourseStudents(const std::string& courseId) override {
        std::vector<Student*> result;
        try {
            pqxx::connection c(connStr);
            pqxx::work w(c);
            pqxx::result r = w.exec(
                "SELECT s.id, s.name, e.grade FROM students s "
                "JOIN enrollments e ON s.id = e.student_id "
                "WHERE e.course_id = " + w.quote(courseId));

            for(auto row : r) {
                std::string sId = row["id"].c_str();
                auto it = std::find_if(studentCache.begin(), studentCache.end(), [&](const Student& s){ return s.id == sId; });
                if (it == studentCache.end()) {
                    studentCache.emplace_back(sId, row["name"].c_str());
                    it = std::prev(studentCache.end());
                }
                it->courseGrades[courseId] = row["grade"].as<double>();
                result.push_back(&(*it));
            }
        } catch (...) {}
        return result;
    }


    bool assignTeacher(const std::string& cId, const std::string& tId) override {
        try {
            pqxx::connection c(connStr);
            pqxx::work w(c);
            pqxx::result tCheck = w.exec("SELECT 1 FROM teachers WHERE id = " + w.quote(tId));
            if (tCheck.empty()) return false;
            w.exec("UPDATE courses SET teacher_id = " + w.quote(tId) + " WHERE id = " + w.quote(cId));
            w.commit();
            return true;
        } catch (...) { return false; }
    }

    bool enrollStudent(const std::string& sId, const std::string& cId) override {
        try {
            pqxx::connection c(connStr);
            pqxx::work w(c);
            w.exec("INSERT INTO enrollments (student_id, course_id, grade) VALUES ("
                   + w.quote(sId) + ", " + w.quote(cId) + ", -1)");
            w.commit();
            return true;
        } catch (...) { return false; }
    }

    bool dropCourse(const std::string& sId, const std::string& cId) override {
        try {
            pqxx::connection c(connStr);
            pqxx::work w(c);
            w.exec("DELETE FROM enrollments WHERE student_id = " + w.quote(sId) + " AND course_id = " + w.quote(cId));
            w.commit();
            return true;
        } catch (...) { return false; }
    }

    bool updateStudentGrade(const std::string& sId, const std::string& cId, double grade) override {
        try {
            pqxx::connection c(connStr);
            pqxx::work w(c);
            w.exec("UPDATE enrollments SET grade = " + std::to_string(grade) +
                   " WHERE student_id = " + w.quote(sId) + " AND course_id = " + w.quote(cId));
            w.commit();
            return true;
        } catch (...) { return false; }
    }
};
