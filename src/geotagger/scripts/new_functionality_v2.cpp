#include "ros/ros.h"
#include "geotagger/MavrosData.h"
#include <vector>
#include <regex>
#include <thread>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <iostream> 
#include <chrono> 
#include <ctime> 
/*#define RED_TEXT "\033[1;31m"
#define GREEN_TEXT "\033[1;32m"
#define YELLOW_TEXT "\033[1;33m"
#define BLUE_TEXT "\033[1;34m"
#define MAGENTA_TEXT "\033[1;35m"
#define CYAN_TEXT "\033[1;36m"
#define WHITE_TEXT "\033[1;37m"
#define RESET_TEXT "\033[0m"*/
#define ROS_INFO_GREEN(...) ROS_INFO("\033[1;32m" __VA_ARGS__ "\033[0m") 
#define ROS_INFO_RED(...) ROS_INFO("\033[1;31m" __VA_ARGS__ "\033[0m")
#define ROS_INFO_BLUE(...) ROS_INFO("\033[1;34m" __VA_ARGS__ "\033[0m")
#define ROS_INFO_YELLOW(...) ROS_INFO("\033[1;33m" __VA_ARGS__ "\033[0m")


namespace fs = std::filesystem;

std::vector<geotagger::MavrosData> mavrosDataVector; //MavrosDataVec
std::mutex mavrosDataMutex;  // Mutex to synchronize access to mavrosDataVector
//std::vector<std::string> imageFileNames;  // Vector to store image file names
//int imageCaptureNumber = 1;  // Declare imageCaptureNumber as a global variable
std::string imageDirectory = "/home/edhitha/Images_13_3_24";  // Specify the path to the directory containing images
std::string destinationBaseDirectory = "/home/edhitha/mavros_geotagger_ws/src/geotagger/logs/image_log"; 
std::string cameraCaptureComm = "cd /home/edhitha/Images_13_3_24/ && nvgstcapture-1.0 --automate --count 500 --capture-gap 1000 --capture-auto 500 --image-res 12"; 
std::string csvFilePath = "/home/edhitha/mavros_geotagger_ws/src/geotagger/logs/";
std::string csvFileName = "geotagged_images.csv"; // Set the name of the CSV file
std::string residuePath = "/home/edhitha/mavros_geotagger_ws/src/geotagger/logs/geotagged_images_log/"; // Set the path where the residue file should be stored


void flushDirectoryContents(const std::string& imageDirectory, const std::string& destinationBaseDir);
void gpsInfoThread(int argc, char *argv[]);
void mavrosDataCallback(const geotagger::MavrosData::ConstPtr& mavros_data_msg);
void imageCaptureThread(int argc, char *argv[]);
void initializeCSVFile(const std::string& csvFileName, const std::string& csvFilePath, const std::string& residuePath);
std::string findFileByKey(const std::string& directoryPath, int key);
void associateImaNogeWithPreviousGPS(ros::Time imageCaptureTimestamp,const std::string& foundFileName);


int main(int argc, char *argv[]) {

    flushDirectoryContents(imageDirectory,destinationBaseDirectory);
    // Redirect stdout to a pipe
    ros::init(argc, argv, "Mavros_Functionality_Node");
    ros::NodeHandle nh;

    // Create threads for GPS info and image capture
    std::thread gpsThread(gpsInfoThread, argc, argv);
    std::thread imageThread(imageCaptureThread, argc, argv);

    // Wait for threads to finish
    gpsThread.join();
    imageThread.join();

    return EXIT_SUCCESS;
}

void flushDirectoryContents(const std::string& imageDirectory, const std::string& destinationBaseDir) {
    // Check if the source directory exists
    if (!fs::exists(imageDirectory) || !fs::is_directory(imageDirectory)) {
        ROS_INFO_RED("Source directory does not exist or is not a directory.\n");
        return;
    }

    // Check if the destination base directory exists, create it if not
    if (!fs::exists(destinationBaseDir)) {
        fs::create_directory(destinationBaseDir);
    }

    // Check if the source directory is not empty
    if (fs::is_empty(imageDirectory)) {
        ROS_INFO_YELLOW("Source directory is empty. No contents to flush.\n");
        return;
    }

    // Get the current system date and time
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm* localTime = std::localtime(&now);
    char dateTimeBuffer[20];
    std::strftime(dateTimeBuffer, sizeof(dateTimeBuffer), "%H%M%S_%Y%m%d", localTime);

    // Create the new directory with the format {systemtime}_{systemdate}
    std::string newDirName = destinationBaseDir + "/" + dateTimeBuffer;
    fs::create_directory(newDirName);

    // Iterate over the contents of the source directory
    for (const auto& entry : fs::directory_iterator(imageDirectory)) {
        // Move each item to the new directory
        fs::rename(entry.path(), fs::path(newDirName) / entry.path().filename());
    }

    std::cout << GREEN_TEXT << "Contents flushed from " << destinationBaseDir << " to " << newDirName << std::endl;
}

void gpsInfoThread(int argc, char *argv[]) {
    //ros::init(argc, nullptr, "Mavros_Subscriber_Node_GPS_Thread");

    // ROS initialization for the thread
    ros::NodeHandle nh;

    std::cout << BLUE_TEXT << "Initiating Subscribe To Custom Topic" << std::endl;

    // Subscribe to the final GPS data topic
    ros::Subscriber mavros_data_sub = nh.subscribe<geotagger::MavrosData>(
        "final_gps_data_topic",
        10,
        mavrosDataCallback
    );

    ros::spin();
}
void mavrosDataCallback(const geotagger::MavrosData::ConstPtr& mavros_data_msg) {
    // Append data to the vector
    std::lock_guard<std::mutex> lock(mavrosDataMutex);
    mavrosDataVector.push_back(*mavros_data_msg);

    ROS_INFO("------------------------------------------------------------------");
    ROS_INFO("Received Data: "
              "Latitude: %.6f, Longitude: %.6f, Altitude: %.6f, Compass Heading: %.2f, Yaw: %.6f, Timestamp: %.6f",
              mavros_data_msg->latitude, mavros_data_msg->longitude, mavros_data_msg->altitude,
              mavros_data_msg->compass_heading, mavros_data_msg->yaw, mavros_data_msg >timestamp.toSec());
}
void imageCaptureThread(int argc, char *argv[]) {

    initializeCSVFile(csvFileName, csvFilePath ,residuePath);
    //listAndSortImageFiles(); 

    std::cout << BLUE_TEXT << "Initiating Image Capture" << std::endl;


    FILE *pipe = popen(cameraCaptureComm.c_str(), "r"); //replace with your camera capture command 
    if (!pipe) {
        std::cout<< RED_TEXT << "Error opening pipe.";
        return;
    }

    // Define the regular expression pattern to match
    std::regex imageCapturedRegex("Image Captured");

    // Read the output line by line
    char buffer[1024];
    int imageCaptureNumber = 0;
    while (fgets(buffer, sizeof(buffer), pipe) != NULL && ros::ok()) {
        std::string line = buffer;

        // Check if the line matches the pattern
        if (std::regex_search(line, imageCapturedRegex)) {
            // Image Captured detected
            ROS_INFO("------------------------------------------------------------------");

            ROS_INFO_GREEN("Image Captured Detected");
            ros::Time imageCaptureTimestamp = ros::Time::now();

            // Print image capture number and time
            ROS_INFO_GREEN("Image Capture Number: %d", imageCaptureNumber);
            
            ROS_INFO("Time of Image Capture : %f", imageCaptureTimestamp.toSec());

            // Find the file name based on the image capture number
            int keyToFind = imageCaptureNumber;
            std::string foundFileName = findFileByKey(imageDirectory, keyToFind);

            //associateImageWithPreviousGPS(imageCaptureTimestamp);
            if (!foundFileName.empty()) {
                ROS_INFO("Image File with key %d found: %s", keyToFind, foundFileName.c_str());
                associateImageWithPreviousGPS(imageCaptureTimestamp, foundFileName);
            } else {
                ROS_INFO("Image File with key %d not found.", keyToFind);
            }

            //associateImageWithPreviousGPS(imageCaptureTimestamp, foundFileName);


            // Increment image capture number
            imageCaptureNumber++;
        }
    }

    // Close the pipe
    pclose(pipe);
}
void initializeCSVFile(const std::string& csvFileName, const std::string& csvFilePath, const std::string& residuePath) {
    // Set the full paths for the existing CSV file and the new residue file
    std::string existingCSVFilePath = csvFilePath + csvFileName;
    
    // Check if the CSV file already exists
    if (fs::exists(existingCSVFilePath)) {
        // Get the current system date and time
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm* localTime = std::localtime(&now);
        char dateTimeBuffer[20];
        std::strftime(dateTimeBuffer, sizeof(dateTimeBuffer), "%Y%m%d_%H%M%S", localTime);

        // Create a new CSV file in the residue path with a timestamped name
        std::string residueFileName = std::string(dateTimeBuffer) + "_geotaggedimages.csv";
        std::string residueFilePath = residuePath + residueFileName;

        // Move contents to residue file
        fs::rename(existingCSVFilePath, residueFilePath);
        std::cout << BLUE_TEXT << "Existing file contents moved to: " << residueFilePath << std::endl;

        // Create a new CSV file with the headers in the original path
        std::ofstream newCSVFile(existingCSVFilePath, std::ios::trunc);
        newCSVFile << "file,lat,lon,alt,head,yaw,time\n";
        newCSVFile.close();

        std::cout << GREEN_TEXT << "New CSV file created: " << csvFileName << std::endl;
    } else {
        // Create a new CSV file with the provided name and path
        std::ofstream csvFile(existingCSVFilePath, std::ios::trunc);
        csvFile << "file,lat,lon,alt,head,yaw,time\n";
        csvFile.close();

        std::cout << GREEN_TEXT << "New CSV file created: " << csvFileName << std::endl;
    }
}

std::string findFileByKey(const std::string& directoryPath, int key) {
    std::regex fileNameRegex(".+_(\\d{5})\\..+");
    
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        std::string fileName = entry.path().filename().string();

        std::smatch matches;
        if (std::regex_match(fileName, matches, fileNameRegex)) {
            int lastFiveDigits = std::stoi(matches[1].str());
            if (lastFiveDigits == key) {
                return fileName;
            }
        }
    }

    return "";  // Return an empty string if the file is not found
}


void associateImageWithPreviousGPS(ros::Time imageCaptureTimestamp,const std::string& foundFileName) {
    // Find the index of the last GPS entry before the image capture & append to csv
    std::lock_guard<std::mutex> lock(mavrosDataMutex);

    if (mavrosDataVector.empty()) {
        std::cout << RED_TEXT << "No GPS entries available to associate with the image.";
        return;
    }

    // Find the index of the last GPS entry before the image capture
    int lastIndexBeforeImageCapture = -1;
    for (size_t i = 0; i < mavrosDataVector.size(); ++i) {
        if (mavrosDataVector[i].timestamp < imageCaptureTimestamp) {
            lastIndexBeforeImageCapture = static_cast<int>(i);
        } else {
            break;  // Stop iterating when we reach an entry after the image capture
        }
    }

    // Check if a suitable previous entry is found
    if (lastIndexBeforeImageCapture != -1) {
        ROS_INFO("Associating Image Capture with Previous GPS Entry:");
        ROS_INFO("Previous GPS Data: "
                  "Latitude: %.6f, Longitude: %.6f, Altitude: %.6f, Compass Heading: %.2f, Yaw: %.6f, Timestamp: %.6f",
                  mavrosDataVector[lastIndexBeforeImageCapture].latitude, mavrosDataVector[lastIndexBeforeImageCapture].longitude,
                  mavrosDataVector[lastIndexBeforeImageCapture].altitude, mavrosDataVector[lastIndexBeforeImageCapture].compass_heading,
                  mavrosDataVector[lastIndexBeforeImageCapture].yaw, mavrosDataVector[lastIndexBeforeImageCapture].timestamp.toSec());


         // Print information in the desired format
        std::cout << "$'" << foundFileName << " " 
                << mavrosDataVector[lastIndexBeforeImageCapture].latitude << " "
                << mavrosDataVector[lastIndexBeforeImageCapture].longitude << " "
                << mavrosDataVector[lastIndexBeforeImageCapture].altitude << " "
                << mavrosDataVector[lastIndexBeforeImageCapture].compass_heading << " "
                << mavrosDataVector[lastIndexBeforeImageCapture].yaw << " "
                << imageCaptureTimestamp << "'$";
        std::cout<<"\n";


        // Append the associated data to the CSV file
        std::ofstream csvFile(csvFilePath + csvFileName, std::ios::app);  // Open in append mode

        if (csvFile.is_open()) {

            // Get the most recent image file name
            // std::string recentImageFileName = imageFileNames.empty() ? "" : imageFileNames.back();
            
            csvFile << foundFileName << ","
                    << std::fixed << std::setprecision(9) 
                     << mavrosDataVector[lastIndexBeforeImageCapture].latitude << ","
                     << mavrosDataVector[lastIndexBeforeImageCapture].longitude << ","
                     << mavrosDataVector[lastIndexBeforeImageCapture].altitude << ","
                     << mavrosDataVector[lastIndexBeforeImageCapture].compass_heading << ","
                     << mavrosDataVector[lastIndexBeforeImageCapture].yaw << ","
                     << imageCaptureTimestamp.toSec() << "\n";

            csvFile.close();
        } else {
            std::cout << RED_TEXT << "Unable to open CSV file for appending.";
        }
    } else {
        std::cout << YELLOW_TEXT << "No suitable previous GPS entry found to associate with the image.";
    }
}





