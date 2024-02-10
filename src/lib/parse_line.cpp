#include "clang_format_lib.h"
#include <any>
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

struct topic_info_data
{
    std::function<bool(line_info & info, std::any & data)> func;
    bool done = false;
    std::any func_data;
};

template <typename FUNC> void add_topic(std::vector<topic_info> & topics, FUNC && f)
{
    topics.emplace_back(topic_info{f, false});
}

template <typename DATA, typename FUNC>
void add_topic_data(std::vector<topic_info_data> & topics, FUNC && f)
{
    topics.emplace_back(topic_info_data{f, false, DATA()});
}

void set_topics(std::vector<topic_info> & topics, std::vector<topic_info_data> & topics_data)
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
        else if (info.find("(value"))
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

    struct max_consecutive_empty_lines
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
    };

    add_topic_data<max_consecutive_empty_lines>(topics_data, [](line_info & info, std::any & data) {
        auto & d = std::any_cast<max_consecutive_empty_lines &>(data);

        if (info.finish)
        {
            info.s.MaxEmptyLinesToKeep.set(d.max_consecutive_empty_lines);
            return true;
        }

        if (info.line.empty())
            d.increment();
        else
            d.reset();

        return false;
    });

    struct space_after_function
    {
        bool space_after_if = false;
        bool space_after_function = false;
    };

    add_topic_data<space_after_function>(topics_data, [](line_info & info, std::any & data) {
        auto & d = std::any_cast<space_after_function &>(data);

        if (info.finish || (d.space_after_if && d.space_after_function))
        {
            if (d.space_after_function && d.space_after_if)
                info.s.SpaceBeforeParens.SpaceBeforeParens.set("Always");
            else if (!d.space_after_function && !d.space_after_if)
                info.s.SpaceBeforeParens.SpaceBeforeParens.set("Never");
            else if (!d.space_after_function && d.space_after_if)
                info.s.SpaceBeforeParens.SpaceBeforeParens.set("ControlStatements");

            info.s.SpaceBeforeParens.AfterControlStatements.set(d.space_after_if);
            info.s.SpaceBeforeParens.AfterFunctionDefinitionName.set(d.space_after_function);

            return true;
        }

        if (info.find("if ("))
            d.space_after_if = true;
        else if (info.find("ReferenceClass ("))
            d.space_after_function = true;

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
        set_topics(topics, topics_data);
    }

    void parse_line(const std::string & line)
    {
        line_info info{line, settings, false};

        for (auto & t : topics)
            if (!t.done)
                t.done = t.func(info);

        for (auto & t : topics_data)
            if (!t.done)
                t.done = t.func(info, t.func_data);
    }

    void finish()
    {
        line_info info{"", settings, true};

        for (auto & t : topics)
            if (!t.done)
                t.done = t.func(info);

        for (auto & t : topics_data)
            if (!t.done)
                t.done = t.func(info, t.func_data);
    }

    ~impl() = default;

    clang_format_settings & settings;
    std::vector<topic_info> topics;
    std::vector<topic_info_data> topics_data;
};

// parser

parser::parser(clang_format_settings & s) : m_impl(std::make_unique<parser::impl>(s)) {}

void parser::parse_line(const std::string & line) { m_impl->parse_line(line); }

void parser::finish() { m_impl->finish(); }

parser::~parser() {}

} // namespace clang_format_lib
