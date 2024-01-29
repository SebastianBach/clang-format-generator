#include "clang_format_lib.h"
#include <cstring>
#include <fstream>
#include <iomanip>
#include <map>

namespace clang_format_lib
{

inline unsigned int version_to_uint(VERSION v)
{
    static std::map<VERSION, unsigned int> map_v_uint{
        {VERSION::v3_5, 35u},   {VERSION::v3_7, 37u},   {VERSION::v3_8, 38u},   {VERSION::v5_0, 50u},
        {VERSION::v10_0, 100u}, {VERSION::v13_0, 130u}, {VERSION::v14_0, 140u}, {VERSION::v16_0, 160u},
        {VERSION::v17_0, 170u}, {VERSION::v18_0, 180u}};

    auto it = map_v_uint.find(v);
    if (it != map_v_uint.end())
        return it->second;

    return 999u;
}

inline bool in_version(unsigned int version, VERSION introduced)
{
    return version >= version_to_uint(introduced);
}

inline bool in_version(unsigned int version, VERSION introduced, VERSION outdated)
{
    return version >= version_to_uint(introduced) && version < version_to_uint(outdated);
}

struct writer
{
    writer(const std::filesystem::path & file, unsigned int v) : stream(file), version(v) {}

    ~writer() { stream.close(); }

    bool open()
    {
        if (!stream.is_open())
            return false;

        stream << std::boolalpha;
        return true;
    }

    void write(const char * text) { stream << text << "\n"; }

    void new_line() { stream << "\n"; }

    template <typename VALUE> void write(const setting<VALUE> & s)
    {
        if (!clang_format_lib::in_version(version, s.version))
            return;

        if (s.is_set())
            stream << s.command << ": " << s.get_value() << "\n";
        else
            stream << "# " << s.command << ": ?\n";
    }

    template <typename VALUE> bool in_version(const setting<VALUE> & s) const noexcept
    {
        return clang_format_lib::in_version(version, s.version);
    }

    std::ofstream stream;
    const unsigned int version;
};

bool write_clang_format_file(const clang_format_settings & settings, const std::filesystem::path & file,
                             unsigned int version)
{
    writer writer(file, version);

    if (!writer.open())
        return false;

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

        writer.write(settings.BreakBeforeBraces.AfterClass);
        writer.write(settings.BreakBeforeBraces.AfterFunction);
        writer.write(settings.BreakBeforeBraces.AfterNamespace);
        writer.write(settings.BreakBeforeBraces.AfterStruct);
        writer.write(settings.BreakBeforeBraces.AfterControlStatement);
        writer.write(settings.BreakBeforeBraces.AfterEnum);
        writer.write(settings.BreakBeforeBraces.BeforeElse);
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

        writer.write(settings.SpacesInParens.InConditionalStatements);
        writer.write(settings.SpacesInParens.Other);
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
            if (std::strcmp(settings.Alignment.PointerAlignment.get_value(),
                            settings.Alignment.ReferenceAlignment.get_value()) == 0)
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

        writer.write(settings.SpaceBeforeParens.AfterControlStatements);
        writer.write(settings.SpaceBeforeParens.AfterFunctionDefinitionName);
    }

    return true;
}

} // namespace clang_format_lib
