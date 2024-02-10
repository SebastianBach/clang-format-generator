#ifndef RESULT_H__
#define RESULT_H__

#include <cstdlib>
#include <optional>
#include <source_location>

struct [[nodiscard]] error
{
    error() = delete;

    const char * const msg;
    const char * const func;
    const char * const file;
    const unsigned int line;

  protected:
    constexpr explicit error(const char * e,
                             const std::source_location & loc = std::source_location::current()) noexcept
        : msg(e), func(loc.function_name()), file(loc.file_name()), line(loc.line())
    {
    }

    friend class result;
};

class [[nodiscard]] result : private std::optional<::error>
{
  private:
    constexpr result() noexcept : std::optional<::error>(){};
    constexpr result(const error & err) noexcept : std::optional<::error>(err){};
    constexpr result(const char * e, const std::source_location & loc) noexcept
        : std::optional<::error>(::error{e, loc}){};

  public:
    constexpr auto is_error() const noexcept { return has_value(); }
    constexpr auto is_success() const noexcept { return !is_error(); }
    constexpr auto code() const noexcept
    {
        if (is_error())
            return EXIT_FAILURE;
        return EXIT_SUCCESS;
    }
    constexpr const auto get_error(error invalid = ::error{"invalid error request."}) const noexcept
    {
        return value_or(invalid);
    }

    static constexpr result ok() noexcept { return {}; }

    static constexpr result
    error(const char * e, const std::source_location & loc = std::source_location::current()) noexcept
    {
        return result(e, loc);
    }
};

#endif
