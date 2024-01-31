#include "run.h"
#include <iostream>

int main(int argc, char * argv[])
{
    const auto res = run(argc, argv);

    if (res.is_error())
    {
        const auto err = res.get_error();
        std::cout << "Error: " << err.msg << "\n";
        std::cout << "Function \"" << err.func << "\" at line " << err.line << "\n";
    }

    return res.code();
}
