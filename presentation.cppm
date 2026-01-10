export module Presentation;
import Domain;
import LogicLayer;
import std;

export class ConsoleView {
private:
    std::unique_ptr<CourseController> controller;

    std::string currentStudentId;
    std::string currentTeacherId;

    void secretaryLoop() {
        while(true) {
            std::println("\n--- 教学秘书控制台 (ADMIN) ---\n");
            std::println("1. 查看所有课程");
            std::println("2. 查看所有教师");
            std::println("3. 安排教学任务 (排课)");
            std::println("4. 返回上级菜单");
            std::print("请输入选项: ");

            int op;
            if(!(std::cin >> op)) { std::cin.clear(); std::cin.ignore(1000, '\n'); continue; }

            switch (op) {
                case 1: {
                    auto courses = controller->listCourses();
                    for(auto c : courses) std::println("{}", c->toString());
                    break;
                }
                case 2: {
                    auto teachers = controller->listTeachers();
                    for(auto t : teachers) std::println("工号: {} | 姓名: {}", t->id, t->name);
                    break;
                }
                case 3: {
                    std::string cId, tId;
                    std::print("请输入 课程ID 教师ID (空格分隔): ");
                    std::cin >> cId >> tId;
                    std::println(">>> {}", controller->assignTeachingTask(cId, tId));
                    break;
                }
                case 4: return;
                default: std::println("无效选项");
            }
        }
    }

    void teacherLoop() {

        auto t = controller->findTeacher(currentTeacherId);
        if (!t) return;

        std::println("\n欢迎您，{} 老师！", t->name);

        while(true) {
            std::println("\n--- 教师系统 ---");
            std::println("1. 我的授课列表");
            std::println("2. 录入/修改成绩");
            std::println("3. 返回上级菜单");
            std::print("请输入选项: ");

            int op;
            if(!(std::cin >> op)) { std::cin.clear(); std::cin.ignore(1000, '\n'); continue; }

            switch (op) {
                case 1: {
                    auto courses = controller->fetchTeacherCourses(currentTeacherId);
                    if(courses.empty()) std::println("您暂时没有授课任务。");
                    for(auto c : courses) std::println("{}", c->toString());
                    break;
                }
                case 2: {
                    std::string cId, sId; double score;
                    std::print("请输入 [课程ID] [学生ID] [分数]: ");
                    std::cin >> cId >> sId >> score;
                    std::println(">>> {}", controller->updateGrade(currentTeacherId, cId, sId, score));
                    break;
                }
                case 3: return;
                default: std::println("无效选项");
            }
        }
    }

    void studentLoop() {
        auto s = controller->findStudent(currentStudentId);
        if (!s) return;

        std::println("\n欢迎您，{} 同学！", s->name);

        while(true) {
            std::println("\n--- 学生选课系统 ---");
            std::println("1. 查看所有课程");
            std::println("2. 选课");
            std::println("3. 退课");
            std::println("4. 我的课表与成绩");
            std::println("5. 返回上级菜单");
            std::print("请输入选项: ");

            int op;
            if(!(std::cin >> op)) { std::cin.clear(); std::cin.ignore(1000, '\n'); continue; }

            switch (op) {
                case 1: {
                    auto courses = controller->listCourses();
                    for(auto c : courses) std::println("{}", c->toString());
                    break;
                }
                case 2: {
                    std::string cId; std::print("请输入要选的课程ID: "); std::cin >> cId;
                    std::println(">>> {}", controller->enrollCourse(currentStudentId, cId));
                    break;
                }
                case 3: {
                    std::string cId; std::print("请输入要退的课程ID: "); std::cin >> cId;
                    std::println(">>> {}", controller->dropCourse(currentStudentId, cId));
                    break;
                }
                case 4: {
                    std::println("已选课程:");
                    for(const auto& cId : s->enrolledCourseIds) {
                        double g = s->courseGrades.contains(cId) ? s->courseGrades.at(cId) : -1.0;
                        std::string gradeStr = (g < 0) ? "暂未出分" : std::format("{:.1f}", g);
                        std::println("- 课程ID: {} | 成绩: {}", cId, gradeStr);
                    }
                    break;
                }
                case 5: return;
                default: std::println("无效选项");
            }
        }
    }

    void showMainMenu() {
        std::println("\n==============");
        std::println("    选课系统   ");
        std::println("==============");
        std::println("请选择您的身份登录：");
        std::println("1. 学生");
        std::println("2. 教师");
        std::println("3. 教学秘书");
        std::println("0. 退出系统");
        std::print("请输入选项 (0-3): ");
    }

public:
    ConsoleView(std::unique_ptr<CourseController> ctrl) : controller(std::move(ctrl)) {}

    void run() {
        int choice;
        while(true) {
            showMainMenu();
            if (!(std::cin >> choice)) {
                std::cin.clear(); std::cin.ignore(1000, '\n');
                continue;
            }

            if (choice == 0) break;

            switch (choice) {
                case 1: { // 学生登录
                    std::print("请输入您的学号 (例如 S001): ");
                    std::cin >> currentStudentId;
                    // 校验ID是否存在
                    if (controller->findStudent(currentStudentId)) {
                        studentLoop();
                    } else {
                        std::println("登录失败：学号不存在！");
                    }
                    break;
                }
                case 2: {
                    std::print("请输入您的工号 (例如 T001): ");
                    std::cin >> currentTeacherId;
                    // 校验ID是否存在
                    if (controller->findTeacher(currentTeacherId)) {
                        teacherLoop();
                    } else {
                        std::println("登录失败：工号不存在！");
                    }
                    break;
                }
                case 3: {
                secretaryLoop();
                break;
                }
                default:
                    std::println("无效选项，请重新输入！");
            }
        }
        std::println("感谢使用，再见！");
    }
};
