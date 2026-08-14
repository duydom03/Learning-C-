#include <iostream>
#include <memory>
#include <vector>

class Resource {
    public:
        virtual ~Resource() = default;
        virtual void load() = 0;
};

class Texture : public Resource {
    public:
        void load() override {
            std::cout << "Loading texture" << std::endl;
        }
};

class Sound : public Resource {
    public:
        void load() override {
            std::cout << "Loading sound" << std::endl;
        }
};

class ResourceManager {
    private:
        std::vector<std::shared_ptr<Resource>> resources;
    public:
        void addResource(std::shared_ptr<Resource> resource) {
            resources.push_back(resource);
        }

        void loadAll() {
            for (const auto& resource : resources) {
                resource->load();
            }
        }
};

int main() {
    ResourceManager manager;

    auto texture = std::make_shared<Texture>();
    auto sound = std::make_shared<Sound>();

    manager.addResource(texture);
    manager.addResource(sound);

    manager.loadAll();

    return 0;
}