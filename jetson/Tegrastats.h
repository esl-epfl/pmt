#include <string>
#include <vector>

namespace tegrastats {
class Tegrastats {
public:
    Tegrastats();
    ~Tegrastats();
    std::vector<std::pair<std::string, int>> measure();

private:
    std::string logfile = "";
    bool started_tegrastats = false;
    const int interval = 100; // milliseconds
}; // class Tegrastats

} // end namespace tegrastats