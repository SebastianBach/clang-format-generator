#ifndef TEST_FRAMEWORK__
#define TEST_FRAMEWORK__

#include <cstdlib>
#include <iostream>
#include <memory>
#include <source_location>

class test_case
{
  public:
    test_case(const char * t) : _test_case(t) {}
    ~test_case()
    {
        if (_success)
            std::cout << "Success: " << _test_case << "\n";
        else
            std::cout << "Failure: " << _test_case << "\n";
    }

    const char * err() noexcept
    {
        _success = false;
        return _test_case;
    }

  private:
    const char * const _test_case;
    bool _success = true;
};

class [[nodiscard]] test_result
{
  private:
    test_result() noexcept {}

    test_result(const char * test_case, const char * test, const std::source_location & loc) noexcept
        : m_error(std::make_unique<error_info>(test, test_case, loc.line(), loc.function_name()))
    {
    }

  public:
    static test_result ok() noexcept { return test_result(); }

    static test_result failure(test_case & __test_case, const char * test,
                               const std::source_location & loc = std::source_location::current()) noexcept
    {
        return test_result{__test_case.err(), test, loc};
    }

    auto failed() const noexcept
    {
        const auto * error = m_error.get();
        const auto failure = error != nullptr;

        if (failure)
        {
            std::cout << "Error in test case \"" << error->test_case << "\", check \"" << error->test
                      << "\" at line " << error->line << " in function \"" << error->func << "\"\n";
        }

        return failure;
    }

  private:
    struct error_info
    {
        const char * const test = nullptr;
        const char * const test_case = nullptr;
        const unsigned int line = 0;
        const char * const func = nullptr;
    };

    const std::unique_ptr<error_info> m_error;
};

#define TO_STRING(s) #s

#define TEST_CASE(TEST) test_case __test_case{TEST};

#define CHECK_TRUE(TEST)                                                                                  \
    if ((TEST) == false)                                                                                  \
        return test_result::failure(__test_case, TO_STRING(TEST));

#define CHECK_FALSE(TEST)                                                                                 \
    if ((TEST) == true)                                                                                   \
        return test_result::failure(__test_case, TO_STRING(TEST));

#define RUN(TEST)                                                                                         \
    if (const auto testresult = TEST; testresult.failed())                                                \
        return EXIT_FAILURE;

#endif
