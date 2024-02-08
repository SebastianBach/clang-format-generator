#ifndef TEST_FRAMEWORK__
#define TEST_FRAMEWORK__

#include <cstdlib>
#include <iostream>
#include <memory>
#include <source_location>

class [[nodiscard]] test_result
{
  public:
    test_result() noexcept {}
    test_result(const char * test_case, const char * test,
                const std::source_location & loc = std::source_location::current()) noexcept
        : m_error(std::make_unique<error_info>(test, test_case, loc.line(), loc.function_name()))
    {
    }

    auto failed() const noexcept { return m_error.get() != nullptr; }
    void print() const noexcept
    {
        if (failed())
        {
            auto * error = m_error.get();
            std::cout << "Error in test case \"" << error->test_case << "\", check \"" << error->test
                      << "\"  at line " << error->line << " in function \"" << error->func << "\"\n";
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

inline auto TEST_OK() { return test_result(); }

#define TO_STRING(s) #s

#define TEST_CASE(TEST) const char * __testcase = TEST;

#define CHECK_TRUE(TEST)                                                                                  \
    if ((TEST) == false)                                                                                  \
        return test_result(__testcase, TO_STRING(TEST));

#define CHECK_FALSE(TEST)                                                                                 \
    if ((TEST) == true)                                                                                   \
        return test_result(__testcase, TO_STRING(TEST));

#define RUN(TEST)                                                                                         \
    if (const auto testresult = TEST; testresult.failed())                                                \
    {                                                                                                     \
        testresult.print();                                                                               \
        return EXIT_FAILURE;                                                                              \
    }

#endif
