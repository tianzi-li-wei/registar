export module registrar;
export import Domain;
export import DataLayer;
export import LogicLayer;
export import Presentation;
import std;

export class Registrar {
public:
    int exec() {
        auto db = std::make_shared<MockDB>();
        auto ctrl = std::make_unique<CourseController>(db);
        ConsoleView view(std::move(ctrl));
        view.run();
        return 0;
    }
};

