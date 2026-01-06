export module Presentation;
import Domain;
import LogicLayer;
import std;
using std::size_t;
using std::print;

export class ConsoleView {
private:
    std::unique_ptr<CourseController> controller;
    std::string currentStudentId;
    std::string currentTeacherId;

    void showCourseList();
    void handleEnrollment();
    void handleDropCourse();

    // 角色循环
    void studentLoop();
    void teacherLoopImpl();
    void secretaryLoop(); // [新增]

    void showCourseStudents();
    void handleGradeInput();

public:
    using ControllerPtr = std::unique_ptr<CourseController>;
    ConsoleView(ControllerPtr ctrl);
    void showMainMenu();
    auto run()->int;
};

// 实现
ConsoleView::ConsoleView(ControllerPtr ctrl) : controller(std::move(ctrl)) {}

void ConsoleView::showMainMenu() {
    print("\n=== 欢迎使用选课系统 ===\n");
    print("请选择您的身份登录：\n");
    print("1. 学生\n");
    print("2. 教师\n");
    print("3. 教学秘书\n");
    print("0. 退出系统\n");
    print("请输入选项（0-3）：");
}


void ConsoleView::secretaryLoop() {
    int choice;
    while(true) {
        print("\n--- 教学秘书控制台 ---\n");
        print("1. 查看所有课程\n");
        print("2. 查看所有教师\n");
        print("3. 安排教学任务 (排课)\n");
        print("4. 返回上级菜单\n");
        print("请输入选项：");

        if (!(std::cin >> choice)) {
            std::cin.clear(); std::cin.ignore(1000, '\n'); continue;
        }

        switch(choice) {
            case 1:
                showCourseList();
                break;
            case 2: {
                auto teachers = controller->getAllTeachers();
                print("\n教师列表：\n");
                for(const auto& t : teachers) {
                    print("工号: {} | 姓名: {}\n", t.id, t.name);
                }
                break;
            }
            case 3: {
                std::string cId, tId;
                print("请输入 课程ID 教师ID (空格分隔): ");
                std::cin >> cId >> tId;
                std::string result = controller->assignTeachingTask(cId, tId);
                print(">>> {}\n", result);
                break;
            }
            case 4: return;
            default: print("无效选项\n");
        }
    }
}

void ConsoleView::studentLoop() {
    int choice;
    while (true) {
        auto studentOpt = controller->getStudentInfo(currentStudentId);
        if (!studentOpt) {
            print("学生信息不存在！\n");
            return;
        }

        Student& student = studentOpt.value();
        print("\n[学生] {}（学号：{}）您好！\n", student.name, student.id);
        print("请选择操作：\n");
        print("1. 查看可选课程列表\n");
        print("2. 选课\n");
        print("3. 退课\n");
        print("4. 查看已选课程及成绩\n");
        print("5. 返回上级菜单\n");
        print("请输入选项（1-5）：");

        std::cin >> choice;

        switch (choice) {
            case 1: showCourseList(); break;
            case 2: handleEnrollment(); break;
            case 3: handleDropCourse(); break;
            case 4:
                print("\n已选课程及成绩：\n");
                for (const auto& cId : student.enrolledCourseIds) {
                    // 直接访问 map，不用 getGrade
                    double g = 0.0;
                    bool hasG = false;
                    if(student.courseGrades.contains(cId)) {
                         g = student.courseGrades.at(cId);
                         hasG = true;
                    }

                    print("课程ID: {} | 成绩: ", cId);
                    if (!hasG || g == 0.0) print("(未录入)\n"); // 假设0.0为初始
                    else print("{}\n", g);
                }
                break;
            case 5: return;
            default: print("无效选项，请重新输入！\n");
        }
    }
}

void ConsoleView::teacherLoopImpl() {
    int choice;
    while (true) {
        auto teacherOpt = controller->getTeacherInfo(currentTeacherId);
        if (!teacherOpt) {
            print("教师信息不存在！\n");
            return;
        }

        Teacher& teacher = teacherOpt.value();
        print("\n[教师] {}（工号：{}）您好！\n", teacher.name, teacher.id);
        print("请选择操作：\n");
        print("1. 查看我的授课列表\n");
        print("2. 查看课程选课学生\n");
        print("3. 录入/修改课程成绩\n");
        print("4. 返回上级菜单\n");
        print("请输入选项（1-4）：");

        std::cin >> choice;
        switch (choice) {
            case 1: {
                auto courses = controller->getTeachingCourses(currentTeacherId);
                print("\n授课列表：\n");
                for (const auto& course : courses) {
                    print("{}\n", course.toString());
                }
                break;
            }
            case 2: showCourseStudents(); break;
            case 3: handleGradeInput(); break;
            case 4: return;
            default: print("无效选项，请重新输入！\n");
        }
    }
}

void ConsoleView::showCourseStudents() {
    auto courses = controller->getTeachingCourses(currentTeacherId);
    if (courses.empty()) {
        print("您当前没有授课课程！\n");
        return;
    }

    print("\n请选择要查看的课程：\n");
    for (size_t i = 0; i < courses.size(); ++i) {
        print("{}. {}\n", i + 1, courses[i].toString());
    }
    print("请输入课程编号：");

    int courseChoice;
    std::cin >> courseChoice;

    if (courseChoice < 1 || courseChoice > static_cast<int>(courses.size())) {
        print("无效的课程编号！\n");
        return;
    }

    const std::string& courseId = courses[courseChoice - 1].id;
    auto students = controller->getCourseStudents(courseId);

    print("\n课程 {} 的选课学生列表：\n", courseId);
    if (students.empty()) {
        print("该课程暂无学生选课。\n");
    } else {
        for (const auto& s : students) {
            double grade = 0.0;
            if(s.courseGrades.contains(courseId)) grade = s.courseGrades.at(courseId);

            print("学号: {} | 姓名: {} | 成绩: ", s.id, s.name);
            if (grade == 0.0) print("(未录入)\n");
            else print("{}\n", grade);
        }
    }
}

void ConsoleView::handleGradeInput() {
    auto courses = controller->getTeachingCourses(currentTeacherId);
    if (courses.empty()) {
        print("您当前没有授课课程！\n");
        return;
    }

    print("\n请选择要录入成绩的课程：\n");
    for (size_t i = 0; i < courses.size(); ++i) {
        print("{}. {}\n", i + 1, courses[i].toString());
    }
    print("请输入课程编号：");

    int courseChoice;
    std::cin >> courseChoice;

    if (courseChoice < 1 || courseChoice > static_cast<int>(courses.size())) {
        print("无效的课程编号！\n");
        return;
    }

    const std::string& courseId = courses[courseChoice - 1].id;
    auto students = controller->getCourseStudents(courseId);

    if (students.empty()) {
        print("该课程暂无学生选课，无法录入成绩。\n");
        return;
    }

    print("\n为课程 {} 录入成绩：\n", courseId);
    for (const auto& s : students) {
        double currentGrade = 0.0;
        if(s.courseGrades.contains(courseId)) currentGrade = s.courseGrades.at(courseId);

        print("学生: {} (学号: {}) [当前成绩: {}] 请输入新成绩: ", s.name, s.id, currentGrade);

        double newGrade;
        std::cin >> newGrade;

        if (controller->updateStudentGrade(s.id, courseId, newGrade)) {
            print("成绩录入成功！\n");
        } else {
            print("成绩录入失败！\n");
        }
    }
}

void ConsoleView::showCourseList() {
    const auto& courses = controller->getAvailableCourses();
    print("\n可选课程列表：\n");
    for (const auto& course : courses) {
        print("{}\n", course.toString());
    }
}

void ConsoleView::handleEnrollment() {
    std::string courseId;
    print("请输入要选择的课程ID：");
    std::cin >> courseId;

    if (controller->enrollCourse(currentStudentId, courseId)) {
        print("选课成功！\n");
    } else {
        print("选课失败，请检查课程ID或是否已选该课程！\n");
    }
}

void ConsoleView::handleDropCourse() {
    std::string courseId;
    print("请输入要退选的课程ID：");
    std::cin >> courseId;

    if (controller->dropCourse(currentStudentId, courseId)) {
        print("退课成功！\n");
    } else {
        print("退课失败，请检查课程ID或是否已选该课程！\n");
    }
}

auto ConsoleView::run() -> int {
    int choice;
    while (true) {
        showMainMenu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                currentStudentId = "S001";
                studentLoop();
                break;
            case 2:
                currentTeacherId = "T001";
                teacherLoopImpl();
                break;
            case 3:
                secretaryLoop(); // 进入秘书循环
                break;
            case 0:
                print("感谢使用选课系统，再见！\n");
                return 0;
            default:
                print("无效选项，请重新输入！\n");
        }
    }
    return 0;
}
