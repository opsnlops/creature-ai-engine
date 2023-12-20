#include <iostream>
#include <string>
#include <curl/curl.h>
#include "nlohmann/json.hpp"
#include <cstdlib>

#include "AssistantConfig.h"

using json = nlohmann::json;

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string createAssistant() {
    CURL *curl = curl_easy_init();
    std::string readBuffer;

    const char* apiKey = std::getenv("OPENAI_API_KEY"); // Retrieve API key from environment variable
    if (!apiKey) {
        std::cerr << "API key not found in environment variables" << std::endl;
        return "";
    }

    // Enable verbose output
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    creatures::AssistantConfig config;
    config.instructions = "You are a talking trash can! You like it when people give you trash.";
    config.name = "Trashy";

    // Set up tools using C++ data structures
    creatures::Tool codeInterpreter;
    codeInterpreter.type = "function";
    config.tools.push_back(codeInterpreter);

    config.model = "gpt-4-1106-preview";

    json data = config.to_json();

    std::cout << "Request Body" << std::endl <<data.dump(4) << std::endl;

    std::cout << data.dump().c_str() << std::endl;

    std::string authHeader = "Authorization: Bearer " + std::string(apiKey);

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, authHeader.c_str());
    headers = curl_slist_append(headers, "OpenAI-Beta: assistants=v1");

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/assistants");
    curl_easy_setopt(curl, CURLOPT_POST, 1L); // Explicitly set the request to POST
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.dump().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    return readBuffer;
}

int main() {
    std::string response = createAssistant();
    std::cout << "Response from OpenAI: " << response << std::endl;
    return 0;
}
