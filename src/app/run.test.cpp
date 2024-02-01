#include "run.h"
#include "test_framework.h"
#include <string>
#include <vector>

struct argv
{
    void add(const char * arg) { data.push_back({arg}); }

    char ** get_list()
    {
        list.clear();
        list.reserve(data.size());

        for (auto & d : data)
        {
            list.push_back(d.data());
        }

        return list.data();
    }

    int count() const { return static_cast<int>(data.size()); }

    std::vector<std::string> data;
    std::vector<char *> list;
};

test_result test_run()
{
    {
        // missing arguments

        const auto res = run(0, nullptr);
        CHECK_TRUE(res.is_error())
    }

    {
        // missing arguments

        argv argv;
        argv.add("programName");

        const auto res = run(argv.count(), argv.get_list());
        CHECK_TRUE(res.is_error())
    }

    {
        argv argv;
        argv.add("programName");
        argv.add("new_source.cpp");

        const auto res = run(argv.count(), argv.get_list());
        CHECK_TRUE(res.is_success())
    }

    {
        // missing version argument

        argv argv;
        argv.add("programName");
        argv.add("new_source.cpp");
        argv.add("output");

        const auto res = run(argv.count(), argv.get_list());
        CHECK_TRUE(res.is_error())
    }

    {
        // invalid version argument

        argv argv;
        argv.add("programName");
        argv.add("new_source.cpp");
        argv.add("output");
        argv.add("abc");

        const auto res = run(argv.count(), argv.get_list());
        CHECK_TRUE(res.is_error())
    }

    {
        // invalid source argument

        argv argv;
        argv.add("programName");
        argv.add("no_source.cpp");
        argv.add("output");
        argv.add("140");

        const auto res = run(argv.count(), argv.get_list());
        CHECK_TRUE(res.is_error())
    }

    {
        // invalid source argument

        argv argv;
        argv.add("programName");
        argv.add("new_source.cpp");
        argv.add("output");
        argv.add("140");

        const auto res = run(argv.count(), argv.get_list());
        CHECK_TRUE(res.is_success())
    }

    return TEST_OK;
}

int main()
{

    RUN(test_run())

    return EXIT_SUCCESS;
}
