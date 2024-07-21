// #include "ros/ros.h"
// #include "geotagger/MavrosData.h"
// #include <vector>
// #include <regex>
// #include <thread>
// #include <fstream>
// #include <filesystem>
// #include <algorithm>

// std::vector<geotagger::MavrosData> mavrosDataVector;
// std::mutex mavrosDataMutex;  // Mutex to synchronize access to mavrosDataVector
// std::vector<std::string> imageFileNames;  // Vector to store image file names
// //int imageCaptureNumber = 1;  // Declare imageCaptureNumber as a global variable
// std::string csvFileName = "/home/edhitha/mavros_geotagger_ws/src/geotagger/logs/geotagged_images.csv";
// std::string imageDirectory = "/home/edhitha/Images_21_2_24";  // Specify the path to the directory containing images


// void gpsInfoThread(int argc, char *argv[]);

// void mavrosDataCallback(const geotagger::MavrosData::ConstPtr& mavros_data_msg) {
//     // Append data to the vector
//     std::lock_guard<std::mutex> lock(mavrosDataMutex);
//     mavrosDataVector.push_back(*mavros_data_msg);

//     ROS_INFO("------------------------------------------------------------------");
//     ROS_INFO("Received Data: "
//               "Latitude: %.6f, Longitude: %.6f, Altitude: %.6f, Compass Heading: %.2f, Yaw: %.6f, Timestamp: %.6f",
//               mavros_data_msg->latitude, mavros_data_msg->longitude, mavros_data_msg->altitude,
//               mavros_data_msg->compass_heading, mavros_data_msg->yaw, mavros_data_msg->timestamp.toSec());
// }

// void initializeCSVFile() {
//     std::ofstream csvFile(csvFileName, std::ios::trunc);
//     // Write CSV headers
//     csvFile << "file,lat,lon,alt,head,yaw,time\n";

//     csvFile.close();
// }

// void listAndSortImageFiles() {
//     try {
//         for (const auto& entry : std::filesystem::directory_iterator(imageDirectory)) {
//             if (entry.is_regular_file()) {
//                 std::string fileName = entry.path().filename();
//                 if (fileName.find("_00000") == std::string::npos) {
//                     imageFileNames.push_back(fileName);
//                 }
//             }
//         }

//         // Sort the file names based on the 5-digit number at the end
//         std::sort(imageFileNames.begin(), imageFileNames.end(), [](const std::string& a, const std::string& b) {
//             return std::stoi(a.substr(a.find_last_of('_') + 1)) < std::stoi(b.substr(b.find_last_of('_') + 1));
//         });

//     } catch (const std::filesystem::filesystem_error& e) {
//         ROS_ERROR("Error accessing image directory: %s", e.what());
//     }
// }


// void associateImageWithPreviousGPS(ros::Time imageCaptureTimestamp) {
//     // Find the index of the last GPS entry before the image capture
//     std::lock_guard<std::mutex> lock(mavrosDataMutex);

//     if (mavrosDataVector.empty()) {
//         ROS_WARN("No GPS entries available to associate with the image.");
//         return;
//     }

//     // Find the index of the last GPS entry before the image capture
//     int lastIndexBeforeImageCapture = -1;
//     for (size_t i = 0; i < mavrosDataVector.size(); ++i) {
//         if (mavrosDataVector[i].timestamp < imageCaptureTimestamp) {
//             lastIndexBeforeImageCapture = static_cast<int>(i);
//         } else {
//             break;  // Stop iterating when we reach an entry after the image capture
//         }
//     }

//     // Check if a suitable previous entry is found
//     if (lastIndexBeforeImageCapture != -1) {
//         ROS_INFO("Associating Image Capture with Previous GPS Entry:");
//         ROS_INFO("Previous GPS Data: "
//                   "Latitude: %.6f, Longitude: %.6f, Altitude: %.6f, Compass Heading: %.2f, Yaw: %.6f, Timestamp: %.6f",
//                   mavrosDataVector[lastIndexBeforeImageCapture].latitude, mavrosDataVector[lastIndexBeforeImageCapture].longitude,
//                   mavrosDataVector[lastIndexBeforeImageCapture].altitude, mavrosDataVector[lastIndexBeforeImageCapture].compass_heading,
//                   mavrosDataVector[lastIndexBeforeImageCapture].yaw, mavrosDataVector[lastIndexBeforeImageCapture].timestamp.toSec());

//         // Append the associated data to the CSV file
//         std::ofstream csvFile(csvFileName, std::ios::app);  // Open in append mode

//         if (csvFile.is_open()) {

//             // Get the most recent image file name
//             std::string recentImageFileName = imageFileNames.empty() ? "" : imageFileNames.back();
            
//             csvFile << recentImageFileName << ","
//                     << std::fixed << std::setprecision(9) 
//                      << mavrosDataVector[lastIndexBeforeImageCapture].latitude << ","
//                      << mavrosDataVector[lastIndexBeforeImageCapture].longitude << ","
//                      << mavrosDataVector[lastIndexBeforeImageCapture].altitude << ","
//                      << mavrosDataVector[lastIndexBeforeImageCapture].compass_heading << ","
//                      << mavrosDataVector[lastIndexBeforeImageCapture].yaw << ","
//                      << imageCaptureTimestamp.toSec() << "\n";

//             csvFile.close();
//         } else {
//             ROS_ERROR("Unable to open CSV file for appending.");
//         }
//     } else {
//         ROS_WARN("No suitable previous GPS entry found to associate with the image.");
//     }
// }

//     // Check if a suitable previous entry is found
// //     if (lastIndexBeforeImageCapture != -1) {
// //         ROS_INFO("Associating Image Capture with Previous GPS Entry:");
// //         ROS_INFO("Previous GPS Data: "
// //                   "Latitude: %.6f, Longitude: %.6f, Altitude: %.6f, Compass Heading: %.2f, Yaw: %.6f, Timestamp: %.6f",
// //                   mavrosDataVector[lastIndexBeforeImageCapture].latitude, mavrosDataVector[lastIndexBeforeImageCapture].longitude,
// //                   mavrosDataVector[lastIndexBeforeImageCapture].altitude, mavrosDataVector[lastIndexBeforeImageCapture].compass_heading,
// //                   mavrosDataVector[lastIndexBeforeImageCapture].yaw, mavrosDataVector[lastIndexBeforeImageCapture].timestamp.toSec());
// //     } else {
// //         ROS_WARN("No suitable previous GPS entry found to associate with the image.");
// //     }
// // }

// void gpsInfoThread(int argc, char *argv[]) {
//     //ros::init(argc, nullptr, "Mavros_Subscriber_Node_GPS_Thread");

//     // ROS initialization for the thread
//     ros::NodeHandle nh;

//     // Subscribe to the final GPS data topic
//     ros::Subscriber mavros_data_sub = nh.subscribe<geotagger::MavrosData>(
//         "final_gps_data_topic",
//         10,
//         mavrosDataCallback
//     );

//     ros::spin();
// }

// void imageCaptureThread(int argc, char *argv[]) {

//     initializeCSVFile();
//     listAndSortImageFiles();

//     FILE *pipe = popen("cd /home/edhitha/Images_21_2_24 && nvgstcapture-1.0  --automate --count 5 --capture-gap 1000 --capture-auto 5 --image-res 12", "r");
//     if (!pipe) {
//         ROS_ERROR("Error opening pipe.");
//         return;
//     }

//     // Define the regular expression pattern to match
//     std::regex imageCapturedRegex("Image Captured");

//     // Read the output line by line
//     char buffer[1024];
//     int imageCaptureNumber = 1;
//     while (fgets(buffer, sizeof(buffer), pipe) != NULL && ros::ok()) {
//         std::string line = buffer;

//         // Check if the line matches the pattern
//         if (std::regex_search(line, imageCapturedRegex)) {
//             // Image Captured detected
//             ROS_INFO("------------------------------------------------------------------");

//             ROS_INFO("Image Captured Detected");
//             ros::Time imageCaptureTimestamp = ros::Time::now();

//             // Print image capture number and time
//             ROS_INFO("Image Capture Number: %d", imageCaptureNumber);
            
//             ROS_INFO("Time of Image Capture : %f", imageCaptureTimestamp.toSec());

//             associateImageWithPreviousGPS(imageCaptureTimestamp);

//             // Increment image capture numbber
//             imageCaptureNumber++;
//         }
//     }

//     // Close the pipe
//     pclose(pipe);
// }

// int main(int argc, char *argv[]) {
//     // Redirect stdout to a pipe
//     ros::init(argc, argv, "Mavros_Subscriber_Node_Main");
//     ros::NodeHandle nh;

//     // Create threads for GPS info and image capture
//     std::thread gpsThread(gpsInfoThread, argc, argv);
//     std::thread imageThread(imageCaptureThread, argc, argv);

//     // Wait for threads to finish
//     gpsThread.join();
//     imageThread.join();

//     return EXIT_SUCCESS;
// }

//4-3-24

// #include "ros/ros.h"
// #include "geotagger/MavrosData.h"
// #include <vector>
// #include <regex>
// #include <thread>
// #include <fstream>
// #include <filesystem>
// #include <algorithm>
// #include <iostream> //
// #include <chrono> //
// #include <ctime> //

// namespace fs = std::filesystem;

// std::vector<geotagger::MavrosData> mavrosDataVector;
// std::mutex mavrosDataMutex;  // Mutex to synchronize access to mavrosDataVector
// std::vector<std::string> imageFileNames;  // Vector to store image file names
// //int imageCaptureNumber = 1;  // Declare imageCaptureNumber as a global variable
// std::string csvFileName = "/home/edhitha/mavros_geotagger_ws/src/geotagger/logs/geotagged_images.csv";
// std::string imageDirectory = "/home/edhitha/Images_21_2_24";  // Specify the path to the directory containing images
// std::string destinationBaseDirectory = "/home/edhitha/mavros_geotagger_ws/src/geotagger/logs/image_log";


// void gpsInfoThread(int argc, char *argv[]);

// void mavrosDataCallback(const geotagger::MavrosData::ConstPtr& mavros_data_msg) {
//     // Append data to the vector
//     std::lock_guard<std::mutex> lock(mavrosDataMutex);
//     mavrosDataVector.push_back(*mavros_data_msg);

//     ROS_INFO("------------------------------------------------------------------");
//     ROS_INFO("Received Data: "
//               "Latitude: %.6f, Longitude: %.6f, Altitude: %.6f, Compass Heading: %.2f, Yaw: %.6f, Timestamp: %.6f",
//               mavros_data_msg->latitude, mavros_data_msg->longitude, mavros_data_msg->altitude,
//               mavros_data_msg->compass_heading, mavros_data_msg->yaw, mavros_data_msg->timestamp.toSec());
// }

// void initializeCSVFile() {
//     std::ofstream csvFile(csvFileName, std::ios::trunc);
//     // Write CSV headers
//     csvFile << "file,lat,lon,alt,head,yaw,time\n";

//     csvFile.close();
// }

// void listAndSortImageFiles() {
//     try {
//         for (const auto& entry : std::filesystem::directory_iterator(imageDirectory)) {
//             if (entry.is_regular_file()) {
//                 std::string fileName = entry.path().filename();
//                 if (fileName.find("_00000") == std::string::npos) {
//                     imageFileNames.push_back(fileName);
//                 }
//             }
//         }

//         // Sort the file names based on the 5-digit number at the end
//         std::sort(imageFileNames.begin(), imageFileNames.end(), [](const std::string& a, const std::string& b) {
//             return std::stoi(a.substr(a.find_last_of('_') + 1)) < std::stoi(b.substr(b.find_last_of('_') + 1));
//         });

//     } catch (const std::filesystem::filesystem_error& e) {
//         ROS_ERROR("Error accessing image directory: %s", e.what());
//     }
// }


// void associateImageWithPreviousGPS(ros::Time imageCaptureTimestamp) {
//     // Find the index of the last GPS entry before the image capture
//     std::lock_guard<std::mutex> lock(mavrosDataMutex);

//     if (mavrosDataVector.empty()) {
//         ROS_WARN("No GPS entries available to associate with the image.");
//         return;
//     }

//     // Find the index of the last GPS entry before the image capture
//     int lastIndexBeforeImageCapture = -1;
//     for (size_t i = 0; i < mavrosDataVector.size(); ++i) {
//         if (mavrosDataVector[i].timestamp < imageCaptureTimestamp) {
//             lastIndexBeforeImageCapture = static_cast<int>(i);
//         } else {
//             break;  // Stop iterating when we reach an entry after the image capture
//         }
//     }

//     // Check if a suitable previous entry is found
//     if (lastIndexBeforeImageCapture != -1) {
//         ROS_INFO("Associating Image Capture with Previous GPS Entry:");
//         ROS_INFO("Previous GPS Data: "
//                   "Latitude: %.6f, Longitude: %.6f, Altitude: %.6f, Compass Heading: %.2f, Yaw: %.6f, Timestamp: %.6f",
//                   mavrosDataVector[lastIndexBeforeImageCapture].latitude, mavrosDataVector[lastIndexBeforeImageCapture].longitude,
//                   mavrosDataVector[lastIndexBeforeImageCapture].altitude, mavrosDataVector[lastIndexBeforeImageCapture].compass_heading,
//                   mavrosDataVector[lastIndexBeforeImageCapture].yaw, mavrosDataVector[lastIndexBeforeImageCapture].timestamp.toSec());

//         // Append the associated data to the CSV file
//         std::ofstream csvFile(csvFileName, std::ios::app);  // Open in append mode

//         if (csvFile.is_open()) {

//             // Get the most recent image file name
//             std::string recentImageFileName = imageFileNames.empty() ? "" : imageFileNames.back();
            
//             csvFile << recentImageFileName << ","
//                     << std::fixed << std::setprecision(9) 
//                      << mavrosDataVector[lastIndexBeforeImageCapture].latitude << ","
//                      << mavrosDataVector[lastIndexBeforeImageCapture].longitude << ","
//                      << mavrosDataVector[lastIndexBeforeImageCapture].altitude << ","
//                      << mavrosDataVector[lastIndexBeforeImageCapture].compass_heading << ","
//                      << mavrosDataVector[lastIndexBeforeImageCapture].yaw << ","
//                      << imageCaptureTimestamp.toSec() << "\n";

//             csvFile.close();
//         } else {
//             ROS_ERROR("Unable to open CSV file for appending.");
//         }
//     } else {
//         ROS_WARN("No suitable previous GPS entry found to associate with the image.");
//     }
// }

//     // Check if a suitable previous entry is found
// //     if (lastIndexBeforeImageCapture != -1) {
// //         ROS_INFO("Associating Image Capture with Previous GPS Entry:");
// //         ROS_INFO("Previous GPS Data: "
// //                   "Latitude: %.6f, Longitude: %.6f, Altitude: %.6f, Compass Heading: %.2f, Yaw: %.6f, Timestamp: %.6f",
// //                   mavrosDataVector[lastIndexBeforeImageCapture].latitude, mavrosDataVector[lastIndexBeforeImageCapture].longitude,
// //                   mavrosDataVector[lastIndexBeforeImageCapture].altitude, mavrosDataVector[lastIndexBeforeImageCapture].compass_heading,
// //                   mavrosDataVector[lastIndexBeforeImageCapture].yaw, mavrosDataVector[lastIndexBeforeImageCapture].timestamp.toSec());
// //     } else {
// //         ROS_WARN("No suitable previous GPS entry found to associate with the image.");
// //     }
// // }

// void gpsInfoThread(int argc, char *argv[]) {
//     //ros::init(argc, nullptr, "Mavros_Subscriber_Node_GPS_Thread");

//     // ROS initialization for the thread
//     ros::NodeHandle nh;

//     // Subscribe to the final GPS data topic
//     ros::Subscriber mavros_data_sub = nh.subscribe<geotagger::MavrosData>(
//         "final_gps_data_topic",
//         10,
//         mavrosDataCallback
//     );

//     ros::spin();
// }

// void imageCaptureThread(int argc, char *argv[]) {

//     initializeCSVFile();
//     listAndSortImageFiles();

//     FILE *pipe = popen("cd /home/edhitha/Images_21_2_24 && nvgstcapture-1.0 --automate --count 50 --capture-gap 1000 --capture-auto 50 --image-res 12", "r");
//     if (!pipe) {
//         ROS_ERROR("Error opening pipe.");
//         return;
//     }

//     // Define the regular expression pattern to match
//     std::regex imageCapturedRegex("Image Captured");

//     // Read the output line by line
//     char buffer[1024];
//     int imageCaptureNumber = 1;
//     while (fgets(buffer, sizeof(buffer), pipe) != NULL && ros::ok()) {
//         std::string line = buffer;

//         // Check if the line matches the pattern
//         if (std::regex_search(line, imageCapturedRegex)) {
//             // Image Captured detected
//             ROS_INFO("------------------------------------------------------------------");

//             ROS_INFO("Image Captured Detected");
//             ros::Time imageCaptureTimestamp = ros::Time::now();

//             // Print image capture number and time
//             ROS_INFO("Image Capture Number: %d", imageCaptureNumber);
            
//             ROS_INFO("Time of Image Capture : %f", imageCaptureTimestamp.toSec());

//             associateImageWithPreviousGPS(imageCaptureTimestamp);

//             // Increment image capture number
//             imageCaptureNumber++;
//         }
//     }

//     // Close the pipe
//     pclose(pipe);
// }

// void flushDirectoryContents(const std::string& imageDirectory, const std::string& destinationBaseDir) {
//     // Check if the source directory exists
//     if (!fs::exists(imageDirectory) || !fs::is_directory(imageDirectory)) {
//         std::cerr << "Source directory does not exist or is not a directory." << std::endl;
//         return;
//     }

//     // Check if the destination base directory exists, create it if not
//     if (!fs::exists(destinationBaseDir)) {
//         fs::create_directory(destinationBaseDir);
//     }

//     // Get the current system date and time
//     auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
//     std::tm* localTime = std::localtime(&now);
//     char dateTimeBuffer[20];
//     std::strftime(dateTimeBuffer, sizeof(dateTimeBuffer), "%H%M%S_%Y%m%d", localTime);

//     // Create the new directory with the format {systemtime}_{systemdate}
//     std::string newDirName = destinationBaseDir + "/" + dateTimeBuffer;
//     fs::create_directory(newDirName);

//     // Iterate over the contents of the source directory
//     for (const auto& entry : fs::directory_iterator(imageDirectory)) {
//         // Move each item to the new directory
//         fs::rename(entry.path(), fs::path(newDirName) / entry.path().filename());

//         //fs::rename(entry.path(), newDirName + "/" + entry.path().filename());
//     }

//     std::cout << "Contents flushed from " << destinationBaseDir << " to " << newDirName << std::endl;
// }

// int main(int argc, char *argv[]) {

//     flushDirectoryContents(imageDirectory,destinationBaseDirectory);
//     // Redirect stdout to a pipe
//     ros::init(argc, argv, "Mavros_Subscriber_Node_Main");
//     ros::NodeHandle nh;

//     // Create threads for GPS info and image capture
//     std::thread gpsThread(gpsInfoThread, argc, argv);
//     std::thread imageThread(imageCaptureThread, argc, argv);

//     // Wait for threads to finish
//     gpsThread.join();
//     imageThread.join();

//     return EXIT_SUCCESS;
// }

