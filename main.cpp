#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Endpoint {
    std::string Type;
    std::string Url;
    std::string Model;
};

// nlohmann::json → Endpoint
inline Endpoint toEndpoint(const nlohmann::json& json) {
    return Endpoint{
        .Type  = json.at("type").get<std::string>(),
        .Url   = json.at("url").get<std::string>(),
        .Model = json.at("model").get<std::string>()
    };
}

int main() {
    namespace fs = std::filesystem;
    fs::path p = fs::current_path();
    fs::path filePath = p.parent_path() / "config.json";
    if (fs::exists(filePath))
        std::cout << "File exists: " << filePath << std::endl;
    else
        std::cout << "File does NOT exist: " << filePath << std::endl;

    fs::path jsonWithPath = "config.json";
    std::ifstream file(jsonWithPath);
    if (!file) {
        std::cerr << "Failed to open config.json\n";
        return 1;
    }

    json config;
    file >> config;

    std::string endpointType = config["endpoint"]["type"];
    std::string url          = config["endpoint"]["url"];
    std::string model        = config["endpoint"]["model"];
    std::string logLevel     = config["log_level"];

    std::cout << "Endpoint type: " << endpointType << "\n";
    std::cout << "URL: " << url << "\n";
    std::cout << "Model: " << model << "\n";
    std::cout << "Log level: " << logLevel << "\n";

    // 
    Endpoint endpoint = toEndpoint(config.at("endpoint"));
    std::cout << "Type: "  << endpoint.Type  << '\n'
              << "Url: "   << endpoint.Url   << '\n'
              << "Model: " << endpoint.Model << '\n';

    return 0;
}
