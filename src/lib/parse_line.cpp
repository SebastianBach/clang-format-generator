#include "clang_format_lib.h"
#include <functional>
#include <vector>

namespace clang_format_lib
{
struct line_info
{
    const std::string & line;
    clang_format_settings & s;
    const bool finish;

    bool find(const char * search) { return line.find(search) != std::string::npos; }
    bool without(const char * search) { return !find(search); }
};

struct topic_info
{
    std::function<bool(line_info & info)> func;
    bool done = false;
};

template <typename FUNC> void add_topic(std::vector<topic_info> & topics, FUNC && f)
{
    topics.emplace_back(topic_info{f, false});
}

void set_topics(std::vector<topic_info> & topics)
{
    add_topic(topics, [](line_info & info) {
        const auto result = info.find("namespace") && info.without("// namespace");

        if (result)
            info.s.BreakBeforeBraces.AfterNamespace.set(info.without("{"));

        return result;
    });
    add_topic(topics, [](line_info & info) {
        const auto result = info.find("class");

        if (result)
            info.s.BreakBeforeBraces.AfterClass.set(info.without("{"));

        return result;
    });
    add_topic(topics, [](line_info & info) {
        const auto result = info.find("struct");

        if (result)
            info.s.BreakBeforeBraces.AfterStruct.set(info.without("{"));

        return result;
    });
    add_topic(topics, [](line_info & info) {
        const auto result = info.find("enum");

        if (result)
            info.s.BreakBeforeBraces.AfterEnum.set(info.without("{"));

        return result;
    });
    add_topic(topics, [](line_info & info) {
        const auto result = info.find("ReferenceClass") && info.without("class");

        if (result)
            info.s.BreakBeforeBraces.AfterFunction.set(info.without("{"));

        return result;
    });
    add_topic(topics, [](line_info & info) {
        const auto pos = info.line.find("TYPE_A");
        const auto result = pos != std::string::npos;
        if (result)
            info.s.IndentWidth.cast(pos);

        return result;
    });
    add_topic(topics, [](line_info & info) {
        const auto result = info.find("MAX WIDTH");
        if (result)
            info.s.ColumnLimit.cast(info.line.length());

        return result;
    });
    add_topic(topics, [](line_info & info) {
        const auto result = info.find("[");
        if (result)
        {
            info.s.SpacesInSquareBrackets.set(info.find("[ 5 ]"));
            info.s.SpaceBefore.SpaceBeforeSquareBrackets.set(info.find(" ["));
        }

        return result;
    });
    add_topic(topics, [](line_info & info) {
        const auto result = info.find("=");
        if (result)
            info.s.SpaceBefore.SpaceBeforeAssignmentOperators.set(info.find(" ="));

        return result;
    });
    add_topic(topics, [](line_info & info) {
        if (info.finish)
        {
            if (!info.s.FixNamespaceComments.is_set())
                info.s.FixNamespaceComments.set(false);
            return true;
        }

        const auto result = info.find("// namespace");
        if (result)
            info.s.FixNamespaceComments.set(true);

        return result;
    });

    add_topic(topics, [](line_info & info) {
        if (info.find("int* "))
        {
            info.s.Alignment.PointerAlignment.set(ALIGNMENT::LEFT);
        }
        else if (info.find("int * "))
        {
            info.s.Alignment.PointerAlignment.set(ALIGNMENT::MIDDLE);
        }
        else if (info.find("int *"))
        {
            info.s.Alignment.PointerAlignment.set(ALIGNMENT::RIGHT);
        }

        return info.s.Alignment.PointerAlignment.is_set();
    });
    add_topic(topics, [](line_info & info) {
        if (info.find("float& "))
        {
            info.s.Alignment.ReferenceAlignment.set(ALIGNMENT::LEFT);
        }
        else if (info.find("float & "))
        {
            info.s.Alignment.ReferenceAlignment.set(ALIGNMENT::MIDDLE);
        }
        else if (info.find("float &"))
        {
            info.s.Alignment.ReferenceAlignment.set(ALIGNMENT::RIGHT);
        }

        return info.s.Alignment.ReferenceAlignment.is_set();
    });
    add_topic(topics, [](line_info & info) {
        if (info.find("( value )"))
        {
            info.s.SpacesInParens.InConditionalStatements.set(true);
            info.s.SpacesInParens.SpacesInConditionalStatement.set(true);
        }
        else if (info.find("(value)"))
        {
            info.s.SpacesInParens.InConditionalStatements.set(false);
            info.s.SpacesInParens.SpacesInConditionalStatement.set(false);
        }

        return info.s.SpacesInParens.InConditionalStatements.is_set();
    });
    add_topic(topics, [](line_info & info) {
        if (info.find("(int"))
        {
            info.s.SpacesInParens.SpacesInParentheses.set(false);
            info.s.SpacesInParens.Other.set(false);
        }
        else if (info.find("( int"))
        {
            info.s.SpacesInParens.SpacesInParentheses.set(true);
            info.s.SpacesInParens.Other.set(true);
        }

        return info.s.SpacesInParens.SpacesInParentheses.is_set();
    });
    add_topic(topics, [](line_info & info) {
        const auto result = info.find("if");
        if (result)
            info.s.BreakBeforeBraces.AfterControlStatement.set(info.without("{"));

        return result;
    });
    add_topic(topics, [](line_info & info) {
        const auto result = info.find("else");
        if (result)
            info.s.BreakBeforeBraces.BeforeElse.set(info.without("}"));

        return result;
    });

    struct
    {
        unsigned int max_consecutive_empty_lines = 0u;
        unsigned int consecutive_empty_lines = 0u;

        void increment()
        {
            consecutive_empty_lines++;
            if (consecutive_empty_lines > max_consecutive_empty_lines)
                max_consecutive_empty_lines = consecutive_empty_lines;
        }

        void reset() { consecutive_empty_lines = 0u; }
    } empty_lines;

    add_topic(topics, [empty_lines](line_info & info) mutable {
        if (info.finish)
        {
            info.s.MaxEmptyLinesToKeep.set(empty_lines.max_consecutive_empty_lines);
            return true;
        }

        if (info.line.empty())
            empty_lines.increment();
        else
            empty_lines.reset();

        return false;
    });

    struct
    {
        bool space_after_if = false;
        bool space_after_function = false;
    } space_after;

    add_topic(topics, [space_after](line_info & info) mutable {
        if (info.finish || (space_after.space_after_if && space_after.space_after_function))
        {
            if (space_after.space_after_function && space_after.space_after_if)
                info.s.SpaceBeforeParens.SpaceBeforeParens.set("Always");
            else if (!space_after.space_after_function && !space_after.space_after_if)
                info.s.SpaceBeforeParens.SpaceBeforeParens.set("Never");
            else if (!space_after.space_after_function && space_after.space_after_if)
                info.s.SpaceBeforeParens.SpaceBeforeParens.set("ControlStatements");

            info.s.SpaceBeforeParens.AfterControlStatements.set(space_after.space_after_if);
            info.s.SpaceBeforeParens.AfterFunctionDefinitionName.set(space_after.space_after_function);

            return true;
        }

        if (info.find("if ("))
            space_after.space_after_if = true;
        else if (info.find("ReferenceClass ("))
            space_after.space_after_function = true;

        return false;
    });
}

// parser::impl

class parser::impl
{
  public:
    impl(clang_format_settings & s) : settings(s)
    {
        topics.reserve(15);
        set_topics(topics);
    }

    void parse_line(const std::string & line) { run_checks(line, false); }

    void finish() { run_checks("", true); }

    void run_checks(const std::string & line, bool finish)
    {
        line_info info{line, settings, finish};

        for (auto & t : topics)
            if (!t.done)
                t.done = t.func(info);
    }

    ~impl() = default;

    clang_format_settings & settings;
    std::vector<topic_info> topics;
};

// parser

parser::parser(clang_format_settings & s) : m_impl(std::make_unique<parser::impl>(s)) {}

void parser::parse_line(const std::string & line) { m_impl->parse_line(line); }

void parser::finish() { m_impl->finish(); }

parser::~parser() {}

} // namespace clang_format_lib
