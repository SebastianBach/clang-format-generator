
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
    ReferenceClass(int* value, float& ref)
    {
        if (value) 
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
