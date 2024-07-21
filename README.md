# ROS_Geotagging
A ROS package for geotagging drone-captured images using custom ROS message parsing. Features include a launch file for multi-node operations: camera capture, GPS data retrieval, CSV file appending, and logging of previous images and data. Ensures real-time geotagging for efficient aerial data collection.

## Geotagger ROS Package

This ROS package geotags images captured by the NVIDIA Jetson camera based on GPS data obtained from MAVROS. It consists of three main components:

1. **Camera Capture**: Captures images using `nvgstcapture-1.0`.
2. **GPS Data Collection**: Collects GPS data from MAVROS topics and synchronizes it with image timestamps.
3. **Geotagging and CSV Logging**: Associates each captured image with the most recent GPS data and logs the information in a CSV file.

### Features

- Captures images at specified intervals.
- Collects GPS, altitude, compass heading, and yaw data.
- Associates captured images with the most recent GPS data.
- Logs geotagged information in a CSV file.
- Utilizes custom ROS message parsing for data handling.

### Prerequisites

- ROS (Robot Operating System)
- MAVROS package
- NVIDIA Jetson platform with `nvgstcapture-1.0` installed
- GCC (GNU Compiler Collection)

### Installation

1. Clone the repository into your ROS workspace:

```bash
cd ~/catkin_ws/src
git clone <repository_url>
```

2. Build the package:

```bash
cd ~/catkin_ws
catkin_make
source devel/setup.bash
```

### Usage

#### Camera Capture

The camera capture command is executed using the `system()` function to run `nvgstcapture-1.0` with specified parameters. The captured images are stored in a specified directory.

```cpp
#include <cstdlib>
#include <iostream>

int main() {
    const char* capture_command = "cd /home/edhitha/images_test && nvgstcapture-1.0 --automate --count 10 --capture-gap 1000 --capture-auto 10 --image-res 12";
    int return_code = system(capture_command);

    if (return_code == 0) {
        std::cout << "Camera capture command executed successfully!" << std::endl;
    } else {
        std::cerr << "Error executing camera capture command" << std::endl;
    }

    return 0;
}
```

#### Geotagger Node

This node handles GPS data collection, image capture, and geotagging.

1. **Flush Directory Contents**: Moves existing images to a new directory based on the current timestamp.

2. **GPS Data Collection**: Subscribes to MAVROS topics to collect GPS, altitude, compass heading, and yaw data. 

3. **Image Capture**: Captures images and associates them with the most recent GPS data.

4. **CSV Logging**: Logs the geotagged information in a CSV file.

```cpp
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
#include <mutex>

// Function prototypes
void flushDirectoryContents(const std::string& imageDirectory, const std::string& destinationBaseDir);
void gpsInfoThread(int argc, char *argv[]);
void mavrosDataCallback(const geotagger::MavrosData::ConstPtr& mavros_data_msg);
void imageCaptureThread(int argc, char *argv[]);
void initializeCSVFile(const std::string& csvFileName, const std::string& csvFilePath, const std::string& residuePath);
std::string findFileByKey(const std::string& directoryPath, int key);
void associateImageWithPreviousGPS(ros::Time imageCaptureTimestamp,const std::string& foundFileName);

// Main function
int main(int argc, char *argv[]) {
    flushDirectoryContents(imageDirectory,destinationBaseDirectory);
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

// Function definitions (flushDirectoryContents, gpsInfoThread, mavrosDataCallback, imageCaptureThread, initializeCSVFile, findFileByKey, associateImageWithPreviousGPS)
```

#### Mavros Publisher Node

This node publishes synchronized GPS data to a custom topic.

1. **GPS Callback**: Updates global variable `mavros_data` with GPS data.
2. **Relative Altitude Callback**: Updates global variable `mavros_data` with altitude data.
3. **Compass Heading Callback**: Updates global variable `mavros_data` with compass heading data.
4. **Yaw Callback**: Updates global variable `mavros_data` with yaw data and publishes the data to the custom topic.

```cpp
#include "ros/ros.h"
#include <sensor_msgs/NavSatFix.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/PoseStamped.h>
#include "geotagger/MavrosData.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <queue>

ros::Publisher final_gps_pub;
geotagger::MavrosData mavros_data;
std::mutex csvMutex;
std::ofstream csvFile;
std::queue<geotagger::MavrosData> dataQueue;

// Function prototypes
void appendToCSV();
void gpsCallback(const sensor_msgs::NavSatFix::ConstPtr &gps_msg);
void relAltCallback(const std_msgs::Float64::ConstPtr &rel_alt_msg);
void compassCallback(const std_msgs::Float64::ConstPtr &compass_msg);
void yawCallback(const std_msgs::Float64::ConstPtr &pose_msg);

// Main function
int main(int argc, char *argv[]) {
    ros::init(argc, argv, "Mavros_Publisher_Node");
    ros::NodeHandle nh;

    final_gps_pub = nh.advertise<geotagger::MavrosData>("final_gps_data_topic", 10);

    ros::Subscriber gps_sub = nh.subscribe<sensor_msgs::NavSatFix>(
        "mavros/global_position/global",
        10,
        gpsCallback
    );

    ros::Subscriber rel_alt_sub = nh.subscribe<std_msgs::Float64>(
        "mavros/global_position/rel_alt",
        10,
        relAltCallback
    );

    ros::Subscriber compass_sub = nh.subscribe<std_msgs::Float64>(
        "mavros/global_position/compass_hdg",
        10,
        compassCallback
    );

    ros::Subscriber yaw_sub = nh.subscribe<std_msgs::Float64>(
        "mavros/global_position/compass_hdg",
        10,
        yawCallback
    );

    ros::spin();

    return 0;
}

// Function definitions (appendToCSV, gpsCallback, relAltCallback, compassCallback, yawCallback)
```

### Custom Message Parsing

This package uses custom message parsing to handle the synchronization of GPS data with image capture timestamps, ensuring accurate geotagging of images.

### Launching the Nodes

1. **Start ROS Master**:

```bash
roscore
```

2. **Run the Mavros Publisher Node**:

```bash
rosrun geotagger mavros_publisher_node
```

3. **Run the Geotagger Node**:

```bash
rosrun geotagger geotagger_node
```

### License

This project is licensed under the MIT License - see the LICENSE file for details.

### Acknowledgments

- MAVROS package
- NVIDIA Jetson platform
- ROS community

For any queries or issues, please free to contact me: atharvamolkulkarni@gmail.com
