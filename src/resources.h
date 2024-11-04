#ifndef LAMINAR_RESOURCES_H_
#define LAMINAR_RESOURCES_H_

#include <unordered_map>
#include <utility>
#include <string>

// Simple class to abstract away the mapping of HTTP requests for
// resources to their data.
class Resources {
public:
    Resources();

    // If a resource is known for the given path, set start and end to the
    // binary data to send to the client, and content_type to its MIME
    // type. Function returns false if no resource for the given path exists
    bool handleRequest(std::string path, const char** start, const char** end, const char** content_type);

    // Allows providing a custom HTML template. Pass an empty string to use the default.
    void setHtmlTemplate(std::string templ = std::string());

private:
    struct Resource {
        const char* start;
        const char* end;
        const char* content_type;
    };
    std::unordered_map<std::string, Resource> resources;
    std::string index_html;
};

#endif // LAMINAR_RESOURCES_H_
