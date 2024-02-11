#include "run.h"
#include "test_framework.h"
#include <string>
#include <vector>

struct args
{
    void add(const char * arg) { data.push_back({arg}); }

    char ** get()
    {
        list.clear();
        list.reserve(data.size());

        for (auto & d : data)
            list.push_back(d.data());

        return list.data();
    }

    int count() const { return static_cast<int>(data.size()); }

    operator int() const noexcept { return count(); }

    operator char **() noexcept { return get(); }

    std::vector<std::string> data;
    std::vector<char *> list;
};

auto run_args(args & a) { return run(a.count(), a.get()); }

auto test_run()
{
    {
        TEST_CASE("no arguments")

        const auto res = run(0, nullptr);
        CHECK_TRUE(res.is_error())
    }

    {
        TEST_CASE("missing arguments")

        args args;
        args.add("programName");

        const auto res = run_args(args);
        CHECK_TRUE(res.is_error())
    }

    {
        TEST_CASE("reference file creation")

        args args;
        args.add("programName");
        args.add("new_source.cpp");

        const auto res = run_args(args);
        CHECK_TRUE(res.is_success())
    }

    {
        TEST_CASE("missing version argument")

        args args;
        args.add("programName");
        args.add("new_source.cpp");
        args.add("output");

        const auto res = run_args(args);
        CHECK_TRUE(res.is_error())
    }

    {
        TEST_CASE("invalid version argument")

        args args;
        args.add("programName");
        args.add("new_source.cpp");
        args.add("output");
        args.add("abc");

        const auto res = run_args(args);
        CHECK_TRUE(res.is_error())
    }

    {
        TEST_CASE("invalid source argument")

        args args;
        args.add("programName");
        args.add("no_source.cpp");
        args.add("output");
        args.add("140");

        const auto res = run_args(args);
        CHECK_TRUE(res.is_error())
    }

    {
        TEST_CASE("empty arguments")

        args args;
        args.add("programName");
        args.add("");
        args.add("");
        args.add("");

        const auto res = run_args(args);
        CHECK_TRUE(res.is_error())
    }

    {
        TEST_CASE("clang format file creation")

        args args;
        args.add("programName");
        args.add("new_source.cpp");
        args.add("output");
        args.add("140");

        const auto res = run_args(args);
        CHECK_TRUE(res.is_success())
    }

    return test_result::ok();
}

int main()
{
    RUN(test_run())

    return EXIT_SUCCESS;
}
