#ifndef WISKR_PASTEBOARD_CONTENT_H
#define WISKR_PASTEBOARD_CONTENT_H

#include <wiskr/wiskr_core_export.h>

#include <string>
#include <vector>

namespace whisker {

    /**
     * The PasteboardContent class represents the content that can be placed on the system clipboard (pasteboard) for copy-paste operations.
     * It can contain multiple items of different types, such as text, images, or custom data formats, allowing for flexible data transfer between applications.
     */
    class WISKRCORE_EXPORT PasteboardContent {
    public:
        PasteboardContent() = default;

        /**
         * Adds a text item to the pasteboard content. This allows for copying plain text to the clipboard.
         * @param url The string of text to add to the pasteboard content. This can be any arbitrary text, such as a URL, a snippet of code, or a plain message.
         */
        void addUrl(const std::string& url);

        /**
         * Checks if the pasteboard content contains any URL items. This can be used to determine if there is URL data available in the pasteboard content.
         * @return true if there is at least one URL item in the pasteboard content, false otherwise.
         */
        bool hasUrls();

        /**
         * Retrieves all text items from the pasteboard content. This allows for accessing the text data that has been added to the pasteboard.
         * @return A vector of strings containing all text items in the pasteboard content. If no text items have been added, this will return an empty vector.
         */
        std::vector<std::string> urls() const;

    private:
        std::vector<std::string> urls_; ///< Internal storage for text items added to the pasteboard content. This can be extended in the future to support other types of content such as images or custom data formats.
    };

    PasteboardContent WISKRCORE_EXPORT makePasteboardContentWithUrls(const std::vector<std::string>& urls);

} // namespace whisker


#endif //WISKR_PASTEBOARD_CONTENT_H
