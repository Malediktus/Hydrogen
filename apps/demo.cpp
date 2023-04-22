#include <Hydrogen/Hydrogen.hpp>

class DemoApplication : public Hydrogen::Application {
public:
    void OnSetup() override {
        ApplicationInfo.Name = "Hydrogen Demo";
        ApplicationInfo.Version = {1, 0, 0};
    }

    void OnInit() override {
        Console->Debug("Started App");
    }

    void OnShutdown() override {
    }

    void OnUpdate() override {
    }
};

Hydrogen::Reference<Hydrogen::Application> Hydrogen::CreateApplication() {
    return Hydrogen::NewReference<DemoApplication>();
}
