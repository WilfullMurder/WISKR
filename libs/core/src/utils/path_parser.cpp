#include <wiskr/utils/path_parser.h>

namespace whisker {
    namespace utils {

        std::vector<std::string> ParsePathFrom(const std::string &path) {
            std::vector<std::string> components;
            std::string currentComponent;
            bool escapeNext = false;

            for (size_t i = 0; i < path.length(); ++i) {
                char c = path[i];

                if (escapeNext) {
                    currentComponent += c;
                    escapeNext = false;
                } else if (c == '\\') {
                    escapeNext = true;
                } else if (c == '/') {
                    if (!currentComponent.empty()) {
                        components.push_back(std::move(currentComponent));
                        currentComponent.clear();
                    }
                } else {
                    currentComponent += c;
                }
            }

            if (!currentComponent.empty()) {
                components.push_back(std::move(currentComponent));
            }
            return components;
        }


            std::string FormatPath(const std::vector<std::string>& components) {
                std::string path;
                for (const auto& component : components) {
                    if (!path.empty()) {
                        path += '/';
                    }

                    std::string escapedValue = component;

                    size_t pos = 0;
                    while ((pos = escapedValue.find('\\', pos)) != std::string::npos) {
                        escapedValue.insert(pos, "\\");
                        pos += 2; // Move past the escaped backslash
                    }

                    pos = 0;
                    while ((pos = escapedValue.find('/', pos)) != std::string::npos) {
                        escapedValue.insert(pos, "\\");
                        pos += 2; // Move past the escaped slash
                    }
                    path += escapedValue;
                }
                return path;
            }
    } // namespace utils
} // namespace whisker