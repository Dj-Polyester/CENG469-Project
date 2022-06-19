#include "app.h"

int main(int argc, char const *argv[])
{
    try
    {
        App app{};
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
