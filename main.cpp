#include "autholas.h"

int main() {
    std::string username, password, hwid;

    std::cout << "===================================" << std::endl;
    std::cout << "      Autholas Login System        " << std::endl;
    std::cout << "         C++ Example Code          " << std::endl;
    std::cout << "===================================" << std::endl;

    std::cout << "Username: ";
    std::getline(std::cin, username);
    std::cout << "Password: ";
    std::getline(std::cin, password);

    // Get Hardware ID
    hwid = getHardwareID();
    std::cout << "Device ID: " << hwid.substr(0, 8) << "..." << std::endl;

    if (authenticateUser(username, password, hwid)) {
        // User authenticated - start your application
        std::cout << "\nAuthentication successful!" << std::endl;
        std::cout << "Starting application..." << std::endl;
        // Your app logic here
    }
    else {
        std::cout << "\nAuthentication failed." << std::endl;
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return 1;
    }

    return 0;
}