#ifndef RESULT_H__
#define RESULT_H__

#include <optional>
#include <source_location>

struct error
{
    error() = delete;

    constexpr explicit error(const char * e,
                             const std::source_location & loc = std::source_location::current()) noexcept
        : msg(e), func(loc.function_name()), file(loc.file_name()), line(loc.line())
    {
    }
    const char * const msg;
    const char * const func;
    const char * const file;
    const unsigned int line;
};

class [[nodiscard]] result : private std::optional<error>
{
  public:
    constexpr result() noexcept : std::optional<error>(){};
    constexpr result(const error & err) noexcept : std::optional<error>(err){};
    constexpr auto is_error() const noexcept { return has_value(); }
    constexpr auto is_success() const noexcept { return !is_error(); }
    constexpr const auto get_error(error invalid = error{
                                                     "invalid error request."}) const noexcept
    {
        if (is_error())
            return value();
        return invalid;
    }
};
static constexpr result SUCCESS;

#endif