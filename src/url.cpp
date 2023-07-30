#include <vector>
#include <string>
#include <string_view>

#include <ada.h>
#include <fast_io.h>
#include <fast_io_device.h>

std::vector<std::string> read_urls(const std::string_view filename) noexcept
{
    static constexpr std::string_view https = "https://";

    try
    {
        std::vector<std::string> urls;
        auto file = fast_io::ibuf_file{filename};
        for (auto&& buffer : line_scanner(file))
        {
            auto end = buffer.end();
            // fast_io only parse utf8 + LF
            if (*(end - 1) == '\r')
                end -= 1;

            const auto line = std::string_view{buffer.begin(), end};
            const auto url = fast_io::concat(https, line);
            if (ada::parse<ada::url_aggregator>(url).has_value())
            {
                urls.push_back(std::string{line});
            }
        }

        if (urls.empty())
        {
            perrln("clear_domains.txt is empty, not url must be anti-tracker");
        }
        return urls;
    }
    catch (fast_io::error& e)
    {
        perrln("clear_domains.txt not found\n", e);
        return {};
    }
}
