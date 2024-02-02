#include "clang_format_lib.h"
#include "test_framework.h"
#include <cstring>

test_result test_settings()
{
    {
        TEST_CASE("base settings test")

        clang_format_lib::clang_format_settings settings;

        CHECK_TRUE(settings.Language.is_set())

        auto & exampleSetting = settings.BreakBeforeBraces.AfterClass;

        CHECK_FALSE(exampleSetting.is_set())

        exampleSetting.set(true);

        CHECK_TRUE(exampleSetting.is_set())

        CHECK_TRUE(exampleSetting.get_value())

        exampleSetting.set(false);

        CHECK_TRUE(exampleSetting.is_set())

        CHECK_FALSE(exampleSetting.get_value())
    }

    return TEST_OK;
}

test_result test_parse_line()
{
    {
        TEST_CASE("base parsing test")

        clang_format_lib::clang_format_settings settings;

        auto & exampleSetting = settings.BreakBeforeBraces.AfterClass;

        CHECK_FALSE(exampleSetting.is_set())

        clang_format_lib::parse_line("class SomeClass {", settings);

        CHECK_TRUE(exampleSetting.is_set())

        CHECK_FALSE(exampleSetting.get_value())

        clang_format_lib::parse_line("class SomeClass", settings);

        CHECK_TRUE(exampleSetting.is_set())

        CHECK_TRUE(exampleSetting.get_value())
    }

    {
        TEST_CASE("multi-line parsing test")

        clang_format_lib::clang_format_settings settings;

        auto & SpaceBeforeParens = settings.SpaceBeforeParens.SpaceBeforeParens;
        auto & MaxEmptyLinesToKeep = settings.MaxEmptyLinesToKeep;

        CHECK_FALSE(SpaceBeforeParens.is_set())
        CHECK_FALSE(MaxEmptyLinesToKeep.is_set())

        std::vector<std::string> lines;
        lines.push_back("  if ( ");
        lines.push_back("");
        lines.push_back("");
        lines.push_back("  ReferenceClass ( ");

        clang_format_lib::parse_lines(lines, settings);

        CHECK_TRUE(SpaceBeforeParens.is_set())
        CHECK_TRUE(MaxEmptyLinesToKeep.is_set())

        CHECK_TRUE(MaxEmptyLinesToKeep.get_value() == 2)

        const char * spaceBefore = SpaceBeforeParens.get_value();

        CHECK_TRUE(std::strcmp(spaceBefore, "Always") == 0)
    }

    return TEST_OK;
}

test_result test_write_clang_format_file() { return TEST_OK; }

int main()
{
    RUN(test_settings())
    RUN(test_parse_line())
    RUN(test_write_clang_format_file())

    return EXIT_SUCCESS;
}
