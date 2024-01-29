#include "clang_format_lib.h"
#include <functional>
#include <vector>

namespace clang_format_lib
{
inline auto find(const std::string & line, const char * search)
{
    return line.find(search) != std::string::npos;
}

void parse_line(const std::string & line, clang_format_settings & settings) noexcept
{
    if (const auto pos = line.find("TYPE_A"); pos != std::string::npos)
    {
        settings.IndentWidth.cast(pos);
    }
    if (find(line, "namespace") && !find(line, "// namespace"))
    {
        settings.BreakBeforeBraces.AfterNamespace.set(!find(line, "{"));
    }
    if (find(line, "class"))
    {
        settings.BreakBeforeBraces.AfterClass.set(!find(line, "{"));
    }
    if (find(line, "struct"))
    {
        settings.BreakBeforeBraces.AfterStruct.set(!find(line, "{"));
    }
    if (find(line, "enum"))
    {
        settings.BreakBeforeBraces.AfterEnum.set(!find(line, "{"));
    }
    if (find(line, "ReferenceClass") && !find(line, "class"))
    {
        settings.BreakBeforeBraces.AfterFunction.set(!find(line, "{"));
    }

    if (find(line, "MAX WIDTH"))
    {
        settings.ColumnLimit.cast(line.length());
    }

    if (find(line, "["))
    {
        settings.SpacesInSquareBrackets.set(find(line, "[ 5 ]"));
        settings.SpaceBefore.SpaceBeforeSquareBrackets.set(find(line, " ["));
    }

    if (find(line, "="))
    {
        settings.SpaceBefore.SpaceBeforeAssignmentOperators.set(find(line, " ="));
    }

    if (find(line, "// namespace lib"))
    {
        settings.FixNamespaceComments.set(true);
    }

    if (find(line, "int* "))
    {
        settings.Alignment.PointerAlignment.set("Left");
    }
    else if (find(line, "int * "))
    {
        settings.Alignment.PointerAlignment.set("Middle");
    }
    else if (find(line, "int *"))
    {
        settings.Alignment.PointerAlignment.set("Right");
    }

    if (find(line, "float& "))
    {
        settings.Alignment.ReferenceAlignment.set("Left");
    }
    else if (find(line, "float & "))
    {
        settings.Alignment.ReferenceAlignment.set("Middle");
    }
    else if (find(line, "float &"))
    {
        settings.Alignment.ReferenceAlignment.set("Right");
    }

    if (find(line, "( v )"))
    {
        settings.SpacesInParens.InConditionalStatements.set(true);
        settings.SpacesInParens.SpacesInConditionalStatement.set(true);
    }
    else if (find(line, "(v)"))
    {
        settings.SpacesInParens.InConditionalStatements.set(false);
        settings.SpacesInParens.SpacesInConditionalStatement.set(false);
    }

    if (find(line, "(int"))
    {
        settings.SpacesInParens.SpacesInParentheses.set(false);
        settings.SpacesInParens.Other.set(false);
    }
    else if (find(line, "( int"))
    {
        settings.SpacesInParens.SpacesInParentheses.set(true);
        settings.SpacesInParens.Other.set(true);
    }

    if (find(line, "if"))
    {
        settings.BreakBeforeBraces.AfterControlStatement.set(!find(line, "{"));
    }

    if (find(line, "else"))
    {
        settings.BreakBeforeBraces.BeforeElse.set(!find(line, "}"));
    }
}

void parse_lines(const std::vector<std::string> & lines, clang_format_settings & settings) noexcept
{
    auto max_consecutive_empty_lines = 0u;
    auto consecutive_empty_lines = 0u;

    auto space_after_if = false;
    auto space_after_function = false;

    for (const auto & line : lines)
    {
        if (line.empty())
        {
            consecutive_empty_lines++;
            if (consecutive_empty_lines > max_consecutive_empty_lines)
                max_consecutive_empty_lines = consecutive_empty_lines;
        }
        else
        {
            consecutive_empty_lines = 0u;
        }

        if (find(line, "if ("))
            space_after_if = true;

        if (find(line, "ReferenceClass ("))
            space_after_function = true;
    }

    if (space_after_function && space_after_if)
        settings.SpaceBeforeParens.SpaceBeforeParens.set("Always");
    else if (!space_after_function && !space_after_if)
        settings.SpaceBeforeParens.SpaceBeforeParens.set("Never");
    else if (!space_after_function && space_after_if)
        settings.SpaceBeforeParens.SpaceBeforeParens.set("ControlStatements");

    settings.SpaceBeforeParens.AfterControlStatements.set(space_after_if);
    settings.SpaceBeforeParens.AfterFunctionDefinitionName.set(space_after_function);

    settings.MaxEmptyLinesToKeep.set(max_consecutive_empty_lines);
}
} // namespace clang_format_lib
