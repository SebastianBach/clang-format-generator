#include "clang_format_lib.h"
#include "test_framework.h"

auto test_failure()
{
    {
        TEST_CASE("This should fail")

        CHECK_TRUE(false);
    }

    return test_result::ok();
}

auto test_test_framework()
{
    {
        TEST_CASE("Expected failed test")

        const auto res = test_failure();

        CHECK_TRUE(res.failed());

        res.print();
    }

    return test_result::ok();
}

auto test_settings()
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

    return test_result::ok();
}

auto test_write_clang_format_file()
{
    {
        TEST_CASE("basic clang format file test")

        clang_format_lib::clang_format_settings settings;
        std::vector<std::string> result;

        clang_format_lib::write_clang_format_file(settings, 170, result);

        CHECK_FALSE(result.empty())
    }

    return test_result::ok();
}

auto test_parser()
{
    {
        TEST_CASE("basic parser test")

        clang_format_lib::clang_format_settings settings;
        clang_format_lib::parser parser(settings);

        parser.parse_line("");
        parser.parse_line("namespace test");
        parser.parse_line("{");
        parser.parse_line(" if (");
        parser.parse_line("");
        parser.parse_line("");
        parser.parse_line("}");
        parser.finish();

        const auto & settingBreak = settings.BreakBeforeBraces.AfterNamespace;

        CHECK_TRUE(settingBreak.is_set());
        CHECK_TRUE(settingBreak.get_value());

        const auto & settingEmpyLines = settings.MaxEmptyLinesToKeep;

        CHECK_TRUE(settingEmpyLines.is_set());
        CHECK_TRUE(settingEmpyLines.get_value() == 2);
    }

    return test_result::ok();
}

auto test_version()
{
    {
        TEST_CASE("test version")

        const char * version = clang_format_lib::version();
        CHECK_TRUE(version != nullptr);
        CHECK_TRUE(version[0] != '\0');
    }

    return test_result::ok();
}

int main()
{
    RUN(test_test_framework())
    RUN(test_settings())
    RUN(test_write_clang_format_file())
    RUN(test_parser())
    RUN(test_version())

    return EXIT_SUCCESS;
}
