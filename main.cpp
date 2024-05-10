#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>
#include <filesystem>

struct Entity {
    int id;
    int level;
    std::string title;
    std::string question;
    std::vector<std::unique_ptr<Entity>> children;

    Entity(int id, int level, std::string title, std::string question)
        : id(id), level(level), title(std::move(title)), question(std::move(question)) {}

    [[nodiscard]] std::string toString(int indent = 0) const {
        std::ostringstream oss;
        std::string indentStr(indent * 4, ' ');

        oss << indentStr << "Entity ID: " << id << "\n";
        oss << indentStr << "Level: " << level << "\n";
        oss << indentStr << "Title: " << title << "\n";
        oss << indentStr << "Question: " << question << "\n";

        for (const auto& child : children) {
            oss << indentStr << "Child:\n";
            oss << child->toString(indent + 1);
        }

        return oss.str();
    }

    static std::unique_ptr<Entity> fromRow(const std::vector<std::string>& row) {
        if (row.size() < 4) {
            throw std::invalid_argument("Row does not have enough elements.");
        }

        try {
            int id = std::stoi(row[0]);
            int level = std::stoi(row[1]);
            const std::string& title = row[2];
            const std::string& question = row[3];

            return std::make_unique<Entity>(id, level, title, question);
        } catch (const std::exception& e) {
            throw std::runtime_error("Failed to convert row data to Entity: " + std::string(e.what()));
        }
    }
};

std::vector<std::unique_ptr<Entity>> nams_result(std::vector<std::unique_ptr<Entity>>& entities) {
    std::vector<std::unique_ptr<Entity>> result;

    result.reserve(entities.size());


    for (auto& e : entities) {
        // todo mapping



        result.push_back(std::move(e));

        //
    }

    return result;
}

std::vector<std::string> split(const std::string& line, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(line);

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    std::filesystem::path filePath = std::filesystem::current_path() / "../data.csv";
    std::ifstream file(filePath);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return 1;
    }

    if (!std::getline(file, line)) {
        std::cerr << "Error: File is empty or could not read the first line." << std::endl;
        return 1;
    }

    std::vector<std::unique_ptr<Entity>> entities;

    while (std::getline(file, line)) {
        std::vector<std::string> row = split(line, ',');
        entities.push_back(Entity::fromRow(row));
    }

    const auto result = nams_result(entities);

    for (const auto& entity : result) {
        std::cout << entity->toString() << std::endl;
    }

    return 0;
}