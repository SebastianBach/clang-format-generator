#include "clang_format_lib.h"
#include <iomanip>
#include <map>

namespace clang_format_lib
{

inline auto version_to_uint(VERSION v)
{
    static const std::map<VERSION, unsigned int> map_v_uint{
        {VERSION::v3_5, 35u},   {VERSION::v3_7, 37u},   {VERSION::v3_8, 38u},   {VERSION::v5_0, 50u},
        {VERSION::v10_0, 100u}, {VERSION::v13_0, 130u}, {VERSION::v14_0, 140u}, {VERSION::v16_0, 160u},
        {VERSION::v17_0, 170u}, {VERSION::v18_0, 180u}};

    if (auto it = map_v_uint.find(v); it != map_v_uint.end())
        return it->second;

    return 999u;
}

inline auto in_version(unsigned int version, VERSION introduced)
{
    return version >= version_to_uint(introduced);
}

inline auto in_version(unsigned int version, VERSION introduced, VERSION outdated)
{
    return version >= version_to_uint(introduced) && version < version_to_uint(outdated);
}

inline auto format_version(unsigned int version)
{
    const auto major = version / 10;
    const auto minor = version - (major * 10);
    return std::to_string(major) + "." + std::to_string(minor);
}

template <typename ARG> void value_to_string(std::ostringstream & oss, const ARG & arg) { oss << arg; }

template <> void value_to_string(std::ostringstream & oss, const ALIGNMENT & arg)
{
    switch (arg)
    {
    case (ALIGNMENT::LEFT):
        oss << "Left";
        return;
    case (ALIGNMENT::MIDDLE):
        oss << "Middle";
        return;
    case (ALIGNMENT::RIGHT):
        oss << "Right";
        return;
    }
}

struct writer
{
    writer(std::vector<std::string> & l, unsigned int v) : lines(l), version(v) {}

    ~writer() {}

    void head()
    {
        lines.reserve(24);

        lines.push_back("# created with https://github.com/SebastianBach/clang-format-generator");

        const auto line = "# created for clang-format version " + format_version(version);
        lines.push_back(line);

        new_line();
    }

    void write(const char * text) { lines.push_back({text}); }

    void new_line() { lines.push_back({}); }

    template <typename VALUE> void write(const setting<VALUE> & s, bool indentation = false)
    {
        if (!clang_format_lib::in_version(version, s.version))
            return;

        std::ostringstream oss;

        if (s.is_set())
        {
            oss << std::boolalpha;

            if (indentation)
                oss << "  ";

            oss << s.command << ": ";
            value_to_string(oss, s.get_value());
        }
        else
        {
            oss << "# " << s.command << ": ?";
        }

        lines.push_back(oss.str());
    }

    template <typename VALUE> auto in_version(const setting<VALUE> & s) const noexcept
    {
        return clang_format_lib::in_version(version, s.version);
    }

    std::vector<std::string> & lines;
    const unsigned int version;
};

void write_clang_format_file(const clang_format_settings & settings, unsigned int version,
                             std::vector<std::string> & lines)
{
    writer writer(lines, version);

    writer.head();

    writer.write(settings.Language);

    writer.new_line();

    writer.write(settings.UseTab);
    writer.write(settings.IndentWidth);
    writer.write(settings.ColumnLimit);
    writer.write(settings.MaxEmptyLinesToKeep);
    writer.write(settings.FixNamespaceComments);

    writer.new_line();

    if (in_version(version, VERSION::v3_8))
    {
        writer.write("BreakBeforeBraces: Custom");
        writer.write("BraceWrapping:");

        writer.write(settings.BreakBeforeBraces.AfterClass, true);
        writer.write(settings.BreakBeforeBraces.AfterFunction, true);
        writer.write(settings.BreakBeforeBraces.AfterNamespace, true);
        writer.write(settings.BreakBeforeBraces.AfterStruct, true);
        writer.write(settings.BreakBeforeBraces.AfterControlStatement, true);
        writer.write(settings.BreakBeforeBraces.AfterEnum, true);
        writer.write(settings.BreakBeforeBraces.BeforeElse, true);
    }

    writer.new_line();

    if (in_version(version, VERSION::v3_7, VERSION::v10_0))
    {
        writer.write(settings.SpacesInParens.SpacesInParentheses);
    }
    else if (in_version(version, VERSION::v10_0, VERSION::v17_0))
    {
        writer.write(settings.SpacesInParens.SpacesInConditionalStatement);
    }
    else if (in_version(version, VERSION::v17_0))
    {
        writer.write("SpacesInParens: Custom");
        writer.write("SpacesInParensOptions:");

        writer.write(settings.SpacesInParens.InConditionalStatements, true);
        writer.write(settings.SpacesInParens.Other, true);
    }

    writer.new_line();

    writer.write(settings.SpaceBefore.SpaceBeforeAssignmentOperators);
    writer.write(settings.SpacesInSquareBrackets);
    writer.write(settings.SpaceBefore.SpaceBeforeSquareBrackets);

    // Alignment

    writer.write(settings.Alignment.PointerAlignment);

    if (writer.in_version(settings.Alignment.ReferenceAlignment))
    {
        if (settings.Alignment.PointerAlignment.is_set() && settings.Alignment.ReferenceAlignment.is_set())
        {
            if (settings.Alignment.PointerAlignment.get_value() ==
                settings.Alignment.ReferenceAlignment.get_value())
            {
                writer.write("ReferenceAlignment: Pointer");
            }
            else
            {
                writer.write(settings.Alignment.ReferenceAlignment);
            }
        }
    }

    writer.new_line();

    // SpaceBeforeParens

    if (in_version(version, VERSION::v3_5, VERSION::v14_0))
    {
        writer.write(settings.SpaceBeforeParens.SpaceBeforeParens);
    }
    else if (in_version(version, VERSION::v14_0))
    {
        writer.write("SpaceBeforeParens: Custom");
        writer.write("SpaceBeforeParensOptions:");

        writer.write(settings.SpaceBeforeParens.AfterControlStatements, true);
        writer.write(settings.SpaceBeforeParens.AfterFunctionDefinitionName, true);
    }
}

} // namespace clang_format_lib
