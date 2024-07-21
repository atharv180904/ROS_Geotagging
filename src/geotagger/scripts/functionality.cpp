/*

// Define a structure to store GPS data with timestamp
struct GpsEntry {
    double latitude;
    double longitude;
    double altitude;
    double compass_heading;
    double yaw;
    ros::Time timestamp; 
}; 

std::vector<GpsEntry> gpsEntries;

// Callback function for receiving GPS data from the topic
void gpsDataCallback(const geotagger::MavrosData::ConstPtr &gps_msg) {
    GpsEntry newEntry;
    newEntry.latitude = gps_msg->latitude;
    newEntry.longitude = gps_msg->longitude;
    newEntry.altitude = gps_msg->altitude;
    newEntry.compass_heading = gps_msg->compass_heading;
    newEntry.yaw = gps_msg->yaw;
    newEntry.timestamp = gps_msg->timestamp;
    std::cout << "Received GPS Data: Lat=" << newEntry.latitude << ", Lon=" << newEntry.longitude
            << ", Alt=" << newEntry.altitude << ", Heading=" << newEntry.compass_heading
            << ", Yaw=" << newEntry.yaw << ", Timestamp=" << newEntry.timestamp << std::endl;

    gpsEntries.push_back(newEntry);
}

int main(int argc,char* argv[]) {
    ros::init(argc, argv, "GPS_Processor_Node");
    ros::NodeHandle nh;

    // Subscribe to the final GPS data topic
    ros::Subscriber gps_sub = nh.subscribe<geotagger::MavrosData>(
        "final_gps_data_topic",
        1000,
        gpsDataCallback
    );


    

    ros::Duration(1.0).sleep();
    // Redirect stdout to a pipe
    FILE* pipe = popen("rosrun geotagger camera_capture", "r");
    if (!pipe) {
        std::cerr << "Error opening pipe." << std::endl;
        return EXIT_FAILURE;
    }

    // Define the regular expression pattern to match
    std::regex imageCapturedRegex("Image Captured");

    // Read the output line by line
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL && ros::ok()) {
        std::string line = buffer;

        // Check if the line matches the pattern
        if (std::regex_search(line, imageCapturedRegex)) {
            // Do something when "Image Captured" is detected
            std::cout << "Image Captured detected! Extracting closest GPS data." << std::endl;

            ros::Time imageCaptureTimestamp = ros::Time::now(); 

            // Print the original line
            //std::cout << line;

            // Get the timestamp of the image capture

            // Find the closest GPS entry
            if (!gpsEntries.empty()) {
                GpsEntry closestEntry;
                double minTimeDifference = std::numeric_limits<double>::max();

                for (const auto& entry : gpsEntries) {
                    double timeDifference = std::abs((imageCaptureTimestamp - entry.timestamp).toSec());
                    if (timeDifference < minTimeDifference) {
                        minTimeDifference = timeDifference;
                        closestEntry = entry;
                    }
                }

                // Print the tuple
                std::cout << "Image Capture {Number}, Lat: " << closestEntry.latitude
                        << ", Lon: " << closestEntry.longitude
                        << ", Alt: " << closestEntry.altitude
                        << ", Heading: " << closestEntry.compass_heading
                        << ", Yaw: " << closestEntry.yaw
                        << ", Image Capture Time: " << imageCaptureTimestamp
                         << ", GPS Timestamp: " << closestEntry.timestamp << std::endl;
            }

            // Store the timestamp in the GPS entry
            GpsEntry newEntry;
            newEntry.timestamp = imageCaptureTimestamp;
            gpsEntries.push_back(newEntry);
        }
    }

    // Close the pipe
    pclose(pipe);
    
    return EXIT_SUCCESS;
}*/
// #include <iostream>
// #include <string>
// #include <regex>
// #include <vector>
// #include <cmath>
// #include <limits> 
// #include "ros/ros.h" 
// #include "geotagger/MavrosData.h"
// #include <cstdlib>

// // Define a structure to store GPS data with timestamp
// struct GpsEntry {
//     double latitude;
//     double longitude;
//     double altitude;
//     double compass_heading;
//     double yaw;
//     ros::Time timestamp; 
// }; 

// std::vector<GpsEntry> gpsEntries;

// // Callback function for receiving GPS data from the topic
// void gpsDataCallback(const geotagger::MavrosData::ConstPtr &gps_msg) {
//     GpsEntry newEntry;
//     newEntry.latitude = gps_msg->latitude;
//     newEntry.longitude = gps_msg->longitude;
//     newEntry.altitude = gps_msg->altitude;
//     newEntry.compass_heading = gps_msg->compass_heading;
//     newEntry.yaw = gps_msg->yaw;
//     newEntry.timestamp = gps_msg->timestamp;
//     std::cout << "Received GPS Data: Lat=" << newEntry.latitude << ", Lon=" << newEntry.longitude
//             << ", Alt=" << newEntry.altitude << ", Heading=" << newEntry.compass_heading
//             << ", Yaw=" << newEntry.yaw << ", Timestamp=" << newEntry.timestamp << std::endl;

//     gpsEntries.push_back(newEntry);
// }

// int main(int argc,char* argv[]) {
//     ros::init(argc, argv, "GPS_Processor_Node");
//     ros::NodeHandle nh;

//     // Subscribe to the final GPS data topic
//     ros::Subscriber gps_sub = nh.subscribe<geotagger::MavrosData>(
//         "final_gps_data_topic",
//         1000,
//         gpsDataCallback
//     );

//     ros::Duration(1.0).sleep();
//     // Redirect stdout to a pipe
//     FILE* pipe = popen("rosrun geotagger camera_capture", "r");
//     if (!pipe) {
//         std::cerr << "Error opening pipe." << std::endl;
//         return EXIT_FAILURE;
//     }

//     // Define the regular expression pattern to match
//     std::regex imageCapturedRegex("Image Captured");

//     // Read the output line by line
//     char buffer[1024];
//     while (fgets(buffer, sizeof(buffer), pipe) != NULL && ros::ok()) {
//         std::string line = buffer;

//         // Check if the line matches the pattern
//         if (std::regex_search(line, imageCapturedRegex)) {
//             // Do something when "Image Captured" is detected
//             std::cout << "Image Captured detected! Extracting closest GPS data." << std::endl;

//             ros::Time imageCaptureTimestamp = ros::Time::now(); 

//             // Print the original line
//             //std::cout << line;

//             // Find the closest GPS entry
//             if (!gpsEntries.empty()) {
//                 GpsEntry closestEntry;
//                 double minTimeDifference = std::numeric_limits<double>::max();

//                 for (const auto& entry : gpsEntries) {
//                     double timeDifference = std::abs((imageCaptureTimestamp - entry.timestamp).toSec());
//                     if (timeDifference < minTimeDifference) {
//                         minTimeDifference = timeDifference;
//                         closestEntry = entry;
//                     }
//                 }

//                 // Print the tuple
//                 std::cout << "Image Capture {Number}, Lat: " << closestEntry.latitude
//                         << ", Lon: " << closestEntry.longitude
//                         << ", Alt: " << closestEntry.altitude
//                         << ", Heading: " << closestEntry.compass_heading
//                         << ", Yaw: " << closestEntry.yaw
//                         << ", Image Capture Time: " << imageCaptureTimestamp
//                         << ", Nearest GPS Entry Time: " << closestEntry.timestamp << std::endl;
//             }

//             // Store the timestamp in the GPS entry
//             GpsEntry newEntry;
//             newEntry.timestamp = imageCaptureTimestamp;
//             gpsEntries.push_back(newEntry);
//         }
//     }

//     // Close the pipe
//     pclose(pipe);
    
//     return EXIT_SUCCESS;
// }

#include <ros/ros.h>
#include <geotagger/MavrosData.h>
#include <regex>
#include <vector>

// Define a structure to store GPS data with timestamp
struct GpsEntry {
    double latitude;
    double longitude;
    double altitude;
    double compass_heading;
    double yaw;
    ros::Time timestamp;
};

std::vector<GpsEntry> gpsEntries;

// Callback function for receiving GPS data from the topic
void gpsDataCallback(const geotagger::MavrosData::ConstPtr &gps_msg) {
    GpsEntry newEntry;
    newEntry.latitude = gps_msg->latitude;
    newEntry.longitude = gps_msg->longitude;
    newEntry.altitude = gps_msg->altitude;
    newEntry.compass_heading = gps_msg->compass_heading;
    newEntry.yaw = gps_msg->yaw;
    newEntry.timestamp = gps_msg->timestamp;

    gpsEntries.push_back(newEntry);
}

int main(int argc, char *argv[]) {
    ros::init(argc, argv, "GPS_Processor_Node");
    ros::NodeHandle nh;

    // Subscribe to the final GPS data topic
    ros::Subscriber gps_sub = nh.subscribe<geotagger::MavrosData>(
        "final_gps_data_topic",
        1000,
        gpsDataCallback
    );

    // Redirect stdout to a pipe
    FILE *pipe = popen("cd /home/edhitha/Images_21_2_24 && nvgstcapture-1.0 --automate --count 10 --capture-gap 1000 --capture-auto 10 --image-res 12", "r");
    if (!pipe) {
        ROS_ERROR("Error opening pipe.");
        return EXIT_FAILURE;
    }

    // Define the regular expression pattern to match
    std::regex imageCapturedRegex("Image Captured");

    // Read the output line by line
    char buffer[1024];
    int imageCaptureNumber = 1;
    while (fgets(buffer, sizeof(buffer), pipe) != NULL && ros::ok()) {
        std::string line = buffer;

        // Check if the line matches the pattern
        if (std::regex_search(line, imageCapturedRegex)) {
            // Image Captured detected
            ROS_INFO("Image Captured Detected");

            // Get the most recent GPS entry
            if (!gpsEntries.empty()) {
                GpsEntry latestEntry = gpsEntries.back();
                // Print GPS data along with image capture number and time
                ROS_INFO("Nearest GPS Data is: Lat=%.6f Lon=%.6f Alt=%.6f Heading=%.2f Yaw=%.6f Time of Image Capture: %.6f",
                         latestEntry.latitude, latestEntry.longitude, latestEntry.altitude,
                         latestEntry.compass_heading, latestEntry.yaw, latestEntry.timestamp.toSec());
            }

            // Print image capture number and time
            ROS_INFO("Image Capture Number: %d", imageCaptureNumber);
            ros::Time imageCaptureTimestamp = ros::Time::now();
            ROS_INFO("Time of Image Capture Detection: %.6f", imageCaptureTimestamp.toSec());

            // Increment image capture number
            imageCaptureNumber++;
        }
    }

    // Close the pipe
    pclose(pipe);

    return EXIT_SUCCESS;
}