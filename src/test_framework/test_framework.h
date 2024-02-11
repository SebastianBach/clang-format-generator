#ifndef TEST_FRAMEWORK__
#define TEST_FRAMEWORK__

#include <cstdlib>
#include <iostream>
#include <memory>
#include <source_location>

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

    static test_result failure(const char * test_case, const char * test,
                               const std::source_location & loc = std::source_location::current()) noexcept
    {
        return test_result{test_case, test, loc};
    }

    auto failed() const noexcept { return m_error.get() != nullptr; }

    void print() const noexcept
    {
        if (failed())
        {
            auto * error = m_error.get();
            std::cout << "Error in test case \"" << error->test_case << "\", check \"" << error->test
                      << "\" at line " << error->line << " in function \"" << error->func << "\"\n";
        }
    }

    static void print_success(const char * test_case, const char * test)
    {
        std::cout << "Success: " << test_case << ", check \"" << test << "\" \n";
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

#define TO_STRING(s) #s

#define TEST_CASE(TEST) test_case __testcase{TEST};

#define CHECK_TRUE(TEST)                                                                                  \
    if ((TEST) == false)                                                                                  \
        return test_result::failure(__testcase.err(), TO_STRING(TEST));

#define CHECK_FALSE(TEST)                                                                                 \
    if ((TEST) == true)                                                                                   \
        return test_result::failure(__testcase.err(), TO_STRING(TEST));

#define RUN(TEST)                                                                                         \
    if (const auto testresult = TEST; testresult.failed())                                                \
    {                                                                                                     \
        testresult.print();                                                                               \
        return EXIT_FAILURE;                                                                              \
    }

#endif
