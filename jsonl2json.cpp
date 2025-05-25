#include <cstdlib>
#include <fstream>
#include <optional>
#include <simdjson.h>
#include <string_view>

namespace
{

using namespace std::string_view_literals;

constexpr std::string_view new_line()
{
#ifdef _WIN32
    return "\r\n";
#else
    return "\n";
#endif
}

constexpr std::string_view NEW_LINE = new_line();

void print_help()
{
    std::cerr << "Usage: jsonl2json <input-file> <output-file>" << std::endl;
    std::cerr << "  input-file: json lines file to read, if not set use stdin" << std::endl;
    std::cerr << "  output-file: json output file, if not set use stdout" << std::endl;
}

void convert_to_json(std::istream &input, std::ostream &output)
{
    std::string line;
    simdjson::ondemand::parser parser;

    output << "[" << NEW_LINE;
    size_t lines = 0;
    while (std::getline(input, line))
    {
        if (lines != 0)
        {
            output << "," << NEW_LINE;
        }
        output << simdjson::to_json_string(parser.iterate(line));
        lines++;
    }
    output << NEW_LINE << "]";
}

int run(std::span<const char *> arguments)
{
    if (!arguments.empty() && (arguments[0] == "-h"sv || arguments[0] == "--help"sv))
    {
        print_help();
        return EXIT_SUCCESS;
    }

    std::optional<std::ifstream> input_file;
    if (!arguments.empty())
    {
        input_file.emplace(arguments[0], std::ifstream::in);
        if (!input_file->is_open())
        {
            std::cerr << "Failed to open file: " << arguments[0] << std::endl;
            return EXIT_FAILURE;
        }
    }
    std::optional<std::ofstream> output_file;
    if (arguments.size() > 1)
    {
        output_file.emplace(arguments[1], std::ifstream::out);
        if (!output_file->is_open())
        {
            std::cerr << "Failed to open file: " << arguments[1] << std::endl;
            return EXIT_FAILURE;
        }
    }

    convert_to_json(input_file.has_value() ? input_file.value() : std::cin,
                    output_file.has_value() ? output_file.value() : std::cout);
    return EXIT_SUCCESS;
}

} // namespace

int main(int argc, const char *argv[])
{
    assert(argc > 0);
    std::span<const char *> arguments{argv + 1, static_cast<size_t>(argc - 1)};
    return run(arguments);
}