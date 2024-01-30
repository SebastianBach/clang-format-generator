#ifndef LIB_H
#define LIB_H

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace clang_format_lib
{
enum class VERSION
{
    v3_5,
    v3_7,
    v3_8,
    v5_0,
    v10_0,
    v13_0,
    v14_0,
    v16_0,
    v17_0,
    v18_0
};

template <typename ARG> struct setting
{
    constexpr setting(const char * cmd, VERSION v = VERSION::v3_5) : command(cmd), version(v) {}

    void set(ARG arg) { data = arg; }

    template <typename IN> void cast(const IN & input) { data = static_cast<ARG>(input); }

    constexpr auto is_set() const noexcept { return data.has_value(); }

    constexpr const ARG & get_value() const { return data.value(); }

    const char * const command;
    std::optional<ARG> data;
    const VERSION version;
};

struct clang_format_settings
{
    clang_format_settings()
    {
        Language.set("Cpp");
        UseTab.set("Never");
    }

    setting<const char *> Language{"Language", VERSION::v3_5};
    setting<const char *> UseTab{"UseTab", VERSION::v3_7};

    setting<unsigned int> ColumnLimit{"ColumnLimit", VERSION::v3_7};
    setting<unsigned int> IndentWidth{"IndentWidth", VERSION::v3_7};

    setting<unsigned int> MaxEmptyLinesToKeep{"MaxEmptyLinesToKeep", VERSION::v3_7};

    struct
    {
        setting<const char *> PointerAlignment{"PointerAlignment", VERSION::v3_7};
        setting<const char *> ReferenceAlignment{"ReferenceAlignment", VERSION::v13_0};

    } Alignment;

    setting<bool> FixNamespaceComments{"FixNamespaceComments", VERSION::v5_0};

    struct
    {
        setting<bool> AfterClass{"AfterClass", VERSION::v3_8};
        setting<bool> AfterNamespace{"AfterNamespace", VERSION::v3_8};
        setting<bool> AfterStruct{"AfterStruct", VERSION::v3_8};
        setting<bool> AfterFunction{"AfterFunction", VERSION::v3_8};
        setting<bool> AfterControlStatement{"AfterControlStatement", VERSION::v3_8};
        setting<bool> AfterEnum{"AfterEnum", VERSION::v3_8};
        setting<bool> BeforeElse{"BeforeElse", VERSION::v3_8};

    } BreakBeforeBraces;

    setting<bool> SpacesInSquareBrackets{"SpacesInSquareBrackets", VERSION::v3_7};

    struct
    {
        setting<bool> SpaceBeforeAssignmentOperators{"SpaceBeforeAssignmentOperators", VERSION::v3_7};
        setting<bool> SpaceBeforeSquareBrackets{"SpaceBeforeSquareBrackets", VERSION::v10_0};
    } SpaceBefore;

    struct
    {
        setting<bool> InConditionalStatements{"InConditionalStatements", VERSION::v17_0};
        setting<bool> Other{"Other", VERSION::v17_0};

        setting<bool> SpacesInConditionalStatement{"SpacesInConditionalStatement", VERSION::v10_0};
        setting<bool> SpacesInParentheses{"SpacesInParentheses", VERSION::v3_7};

    } SpacesInParens;

    struct
    {
        setting<const char *> SpaceBeforeParens{"SpaceBeforeParens", VERSION::v3_5};
        setting<bool> AfterControlStatements{"AfterControlStatements", VERSION::v14_0};
        setting<bool> AfterFunctionDefinitionName{"AfterFunctionDefinitionName", VERSION::v14_0};

    } SpaceBeforeParens;
};

const char * version();

void parse_line(const std::string & line, clang_format_settings & settings) noexcept;

void parse_lines(const std::vector<std::string> & lines, clang_format_settings & settings) noexcept;

void generate_reference_file(std::vector<std::string> & lines);

void write_clang_format_file(const clang_format_settings & settings, unsigned int version,
                             std::vector<std::string> & lines);

} // namespace clang_format_lib

#endif
