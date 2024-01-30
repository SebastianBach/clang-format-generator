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
    constexpr explicit setting(const char * cmd, VERSION v = VERSION::v3_5) : command(cmd), version(v) {}

    void set(ARG arg) { data = arg; }

    template <typename IN> void cast(const IN & input) { data = static_cast<ARG>(input); }

    constexpr auto is_set() const noexcept { return data.has_value(); }

    constexpr const ARG & get_value() const { return data.value(); }

    const char * const command;
    std::optional<ARG> data;
    const VERSION version;
};

enum class ALIGNMENT
{
    LEFT,
    MIDDLE,
    RIGHT
};

using setting_text = setting<const char*>;
using setting_number = setting<unsigned int>;
using setting_switch = setting<bool>;
using setting_alignment = setting<ALIGNMENT>;

struct clang_format_settings
{
    clang_format_settings()
    {
        Language.set("Cpp");
        UseTab.set("Never");
    }

    setting_text Language{"Language", VERSION::v3_5};
    setting_text UseTab{"UseTab", VERSION::v3_7};

    setting_number ColumnLimit{"ColumnLimit", VERSION::v3_7};
    setting_number IndentWidth{"IndentWidth", VERSION::v3_7};

    setting_number MaxEmptyLinesToKeep{"MaxEmptyLinesToKeep", VERSION::v3_7};

    struct
    {
        setting_alignment PointerAlignment{"PointerAlignment", VERSION::v3_7};
        setting_alignment ReferenceAlignment{"ReferenceAlignment", VERSION::v13_0};

    } Alignment;

    setting_switch FixNamespaceComments{"FixNamespaceComments", VERSION::v5_0};

    struct
    {
        setting_switch AfterClass{"AfterClass", VERSION::v3_8};
        setting_switch AfterNamespace{"AfterNamespace", VERSION::v3_8};
        setting_switch AfterStruct{"AfterStruct", VERSION::v3_8};
        setting_switch AfterFunction{"AfterFunction", VERSION::v3_8};
        setting_switch AfterControlStatement{"AfterControlStatement", VERSION::v3_8};
        setting_switch AfterEnum{"AfterEnum", VERSION::v3_8};
        setting_switch BeforeElse{"BeforeElse", VERSION::v3_8};

    } BreakBeforeBraces;

    setting_switch SpacesInSquareBrackets{"SpacesInSquareBrackets", VERSION::v3_7};

    struct
    {
        setting_switch SpaceBeforeAssignmentOperators{"SpaceBeforeAssignmentOperators", VERSION::v3_7};
        setting_switch SpaceBeforeSquareBrackets{"SpaceBeforeSquareBrackets", VERSION::v10_0};
    } SpaceBefore;

    struct
    {
        setting_switch InConditionalStatements{"InConditionalStatements", VERSION::v17_0};
        setting_switch Other{"Other", VERSION::v17_0};

        setting_switch SpacesInConditionalStatement{"SpacesInConditionalStatement", VERSION::v10_0};
        setting_switch SpacesInParentheses{"SpacesInParentheses", VERSION::v3_7};

    } SpacesInParens;

    struct
    {
        setting_text SpaceBeforeParens{"SpaceBeforeParens", VERSION::v3_5};
        setting_switch AfterControlStatements{"AfterControlStatements", VERSION::v14_0};
        setting_switch AfterFunctionDefinitionName{"AfterFunctionDefinitionName", VERSION::v14_0};

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
