#include <iostream>
#include "psvm.hpp"

// TODO: replace fork with thread, run the wrapper and simulator
// on separate threads

int main()
{
    ShowdownRuntime sr;

    for (auto i : SIM_TEST_LINES)
    {
        sr.insert(i);
    }

    bool loop = true;
    while (loop)
    {
        auto val = sr.readResult();
        if (val.has_value())
        {
            std::string line = val.value();

            std::cout << line << "\n";

            if (line == "|tie" || line.substr(0, 4) == "|win")
            {
                loop = false;
            }
        }
    }

    return 0;
}