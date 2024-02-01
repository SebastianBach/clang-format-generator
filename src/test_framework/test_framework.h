#ifndef TEST_FRAMEWORK__
#define TEST_FRAMEWORK__

#include <cstdlib>
#include <iostream>
#include <source_location>

class test_result
{
  public:
    test_result() noexcept : m_failure(false) {}
    test_result(const char * test,
                const std::source_location & loc = std::source_location::current()) noexcept
        : m_failure(true), m_test(test), m_line(loc.line()), m_func(loc.function_name())
    {
    }

    bool failed() const noexcept { return m_failure; }
    void print() const
    {
        if (m_failure)
            std::cout << "Error: " << m_test << " at line " << m_line << " in function \"" << m_func
                      << "\"\n";
    }

  private:
    const bool m_failure;
    const char * const m_test = nullptr;
    const unsigned int m_line = 0;
    const char * const m_func = nullptr;
};

static const test_result TEST_OK{};

#define TO_STRING(s) #s

#define CHECK_TRUE(TEST)                                                                                  \
    if ((TEST) == false)                                                                                  \
        return test_result(TO_STRING(TEST));

#define CHECK_FALSE(TEST)                                                                                 \
    if ((TEST) == true)                                                                                   \
        return test_result(TO_STRING(TEST));

#define RUN(TEST)                                                                                         \
    if (const auto res = TEST; res.failed())                                                              \
    {                                                                                                     \
        res.print();                                                                                      \
        return EXIT_FAILURE;                                                                              \
    }

#endif
