
#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace creatures {

    using json = nlohmann::json;

    struct Tool {
        std::string type;

        // Function to convert Tool to JSON
        json to_json() const {
            return json{{"type", type}};
        }
    };

    class AssistantConfig {
    public:
        std::string instructions;
        std::string name;
        std::vector<Tool> tools;  // Use a vector of Tool
        std::string model;

        // Function to convert AssistantConfig to JSON
        json to_json() const {
            std::vector<json> tools_json;
            for (const auto &tool: tools) {
                tools_json.push_back(tool.to_json());
            }

            return json{
                    {"instructions", instructions},
                    {"name",         name},
                    {"tools",        tools_json},
                    {"model",        model}
            };
        }
    };

}