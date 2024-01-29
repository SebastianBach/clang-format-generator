#include "clang_format_lib.h"
#include <fstream>
#include <functional>
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

bool generate_reference_file(const std::filesystem::path & file)
{
    std::ofstream reference_file(file);

    if (!reference_file.is_open())
        return false;

    reference_file << ref_content;

    reference_file.close();

    return true;
}
} // namespace clang_format_lib
