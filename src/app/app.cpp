#include "clang_format_lib.h"
#include "result.h"
#include <fstream>
#include <iostream>

using text_file_content = std::vector<std::string>;

result write_to_file(const std::filesystem::path & dst, const text_file_content & lines)
{
    std::ofstream file_stream(dst);

    if (!file_stream.is_open())
        return error("Could not open clang-format file to write.");

    for (const auto & line : lines)
        file_stream << line << "\n";

    file_stream.close();

    return SUCCESS;
}

result create_clang_format_file(const clang_format_lib::clang_format_settings & settings,
                                const std::string & dst, unsigned int version)
{
    text_file_content lines;

    clang_format_lib::write_clang_format_file(settings, version, lines);

    if (lines.empty())
        return error("could not create clang-format styles.");

    return write_to_file(dst, lines);
}

result parse_clang_format_settings(const std::string & src, const std::string & dst, unsigned int version)
{
    if (src.empty() || dst.empty())
        return error("Undefined source or target file.");

    std::ifstream file{src};

    if (!file.is_open())
        return error("Could not open source file.");

    clang_format_lib::clang_format_settings settings;

    std::string line;
    text_file_content lines;
    while (std::getline(file, line))
    {
        clang_format_lib::parse_line(line, settings);
        lines.push_back(line);
    }

    clang_format_lib::parse_lines(lines, settings);

    file.close();

    return create_clang_format_file(settings, dst, version);
}

result make_reference_file(const std::filesystem::path & path)
{
    text_file_content lines;

    clang_format_lib::generate_reference_file(lines);

    if (lines.empty())
        return error("Could not obtain reference file data.");

    return write_to_file(path, lines);
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
    } catch (std::invalid_argument const &)
    {
        return error("Invalid argument for clang-format version.");
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
        std::cout << "Function \"" << err.func << "\" at line " << err.line << "\n";
    }

    return res.code();
}
