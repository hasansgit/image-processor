#include <iostream>
#include <string>

void Help() {
    std::string message;

    message += "This is simple Command Line Interface image-processor working with BMP images.\n";

    message += "I think you know all standard filters there.\n";

    message +=
        "The custom filter is 'Yamete'. You can use it by command -yamete (eps), where eps is some rational number.\n";

    message += "Recommended eps for 'Yamete' is 0.67.\n";

    message += "\n\n";

    message +=
        "Hasankhon Burhonov, 20.03.2026\n don't have Copyright (c), GNU Public Licence or something like that.\n";

    std::cout << message << std::endl;
}
