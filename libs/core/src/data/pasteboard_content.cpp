#include <wiskr/data/pasteboard_content.h>

namespace whisker {

    void PasteboardContent::addUrl(const std::string& url) {
        urls_.emplace_back(url);
    }

    bool PasteboardContent::hasUrls() {
        return urls_.size() > 0;
    }

    std::vector<std::string> PasteboardContent::urls() const {
        return urls_;
    }

    PasteboardContent makePasteboardContentWithUrls(const std::vector<std::string>& urls) {
        PasteboardContent content;
        for (const auto& url : urls) {
            content.addUrl(url);
        }
        return content;
    }

} // namespace whisker