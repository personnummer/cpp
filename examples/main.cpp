#include "personnummer.hpp"
#include <iostream>
#include <string>

int main()
{
    std::string ssn = "19900101-0017";
    Personnummer pnr = Personnummer{ssn};

    if (!pnr.valid())
    {
        std::cout << "invalid personal identity number\n";
        return 1;
    }

    std::string gender = pnr.is_female() ? "female" : "male";
    std::cout << "The person with personal identity number " << pnr.format(true)
              << " is a " << gender
              << " of age " << pnr.get_age()
              << std::endl;

    return 0;
}
