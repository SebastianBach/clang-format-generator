#include "clang_format_lib.h"
#include "result.h"
#include <cstdlib>
#include <fstream>
#include <iostream>

result parse_clang_format_settings(const std::string & src, const std::string & dst, unsigned int version)
{
    if (src.empty() || dst.empty())
        return error("Undefined source or target file.");

    std::ifstream file{src};

    if (!file.is_open())
        return error("Could not open source file.");

    clang_format_lib::clang_format_settings settings;

    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line))
    {
        clang_format_lib::parse_line(line, settings);
        lines.push_back(line);
    }

    clang_format_lib::parse_lines(lines, settings);

    file.close();

    return clang_format_lib::write_clang_format_file(settings, dst, version)
               ? SUCCESS
               : error("Error writing the clang format file.");
}

result make_reference_file(const std::filesystem::path & path)
{
    return clang_format_lib::generate_reference_file(path) ? SUCCESS
                                                           : error("Could not create reference file.");
}

result app(int argc, char * argv[])
{
    if (argc == 2)
        return make_reference_file(argv[1]);

    if (argc != 4)
        return error("Invalid command line arguments.");

    auto version = 0u;

    try
    {
        version = static_cast<unsigned int>(std::stoul(argv[3]));
    } catch (std::invalid_argument const & ex)
    {
        return error(ex.what());
    }

    return parse_clang_format_settings(argv[1], argv[2], version);
}

int main(int argc, char * argv[])
{
    const auto res = app(argc, argv);

    if (res.is_error())
    {
        const auto err = res.get_error();
        std::cout << "Error: " << err.msg << "\n";
        std::cout << "in function \"" << err.func << "\" at line " << err.line;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
