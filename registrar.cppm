export module registrar;
export import Domain;
export import DataLayer;
export import LogicLayer;
export import Presentation;
import std;

export class Registrar {
public:
    int exec() {

        std::string connStr = "dbname=registrar user=postgres password=password host=127.0.0.1 port=5432";


        auto db = std::make_shared<PostgresDB>(connStr);

        auto ctrl = std::make_unique<CourseController>(db);
        ConsoleView view(std::move(ctrl));
        view.run();
        return 0;
    }
};
