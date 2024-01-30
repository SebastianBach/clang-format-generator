#include "clang_format_lib.h"

#include <functional>
#include <sstream>
#include <vector>

namespace clang_format_lib
{

static const std::string ref_content = R"(
///////////////////////////////////// MAX WIDTH ///////////////////////////////////////////////////////////

namespace lib
{

enum TYPES
{
    TYPE_A,
    TYPE_B,
};

struct test_data 
{
    float data;
};

class ReferenceClass 
{
public:
    ReferenceClass(int* v, float& ref)
    {
        if (v) 
        {
            int a = 5;
        }
        else
        {
            int a = 6;
        }
    }
private:
    int values[5];
};

} // namespace lib
)";

void generate_reference_file(std::vector<std::string> & lines)
{
    std::istringstream iss(ref_content);

    std::string line;
    while (std::getline(iss, line))
    {
        lines.push_back(line);
    }
}
} // namespace clang_format_lib
