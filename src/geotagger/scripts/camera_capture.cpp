#include <cstdlib>
#include <iostream>

int main() {
    // Assuming l_path, no_images, and capture_gap are predefined variables
    const char* capture_command = "cd /home/edhitha/images_test && nvgstcapture-1.0 --automate --count 10 --capture-gap 1000 --capture-auto 10 --image-res 12";

    int return_code = system(capture_command);

    if (return_code == 0) {
        // The command executed successfully
        std::cout << "Camera capture command executed successfully!" << std::endl;
    } else {
        // There was an error executing the command
        std::cerr << "Error executing camera capture command" << std::endl;
        // Handle the error, print a message, etc.
    }

    return 0;
}

