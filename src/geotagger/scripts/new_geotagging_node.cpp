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
geotagger::MavrosData mavros_data;  // Global variable to store synchronized data
// ros::Time data_received_time;
// ros::Time data_received_time2;
// ros::Time data_received_time3;
// ros::Time data_received_time4;
std::mutex csvMutex;
std::ofstream csvFile;
std::queue<geotagger::MavrosData> dataQueue;

void appendToCSV() {
    
    while (true) {
        // Check if there is data in the queue
        std::unique_lock<std::mutex> lock(csvMutex);
        if (dataQueue.empty()) {
            // Release the lock and sleep to avoid busy-waiting
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        // Pop data from the queue
        geotagger::MavrosData data = dataQueue.front();
        dataQueue.pop();
        lock.unlock();

        // Open file for appending
        csvFile.open("/home/edhitha/mavros_geotagger_ws/src/geotagger/logs/gps_data_log.csv", std::ios::out | std::ios::app);
        if (!csvFile.is_open()) {
            ROS_ERROR("Failed to open CSV file for appending.");
            return;
        }

        // Writing headers if the file is empty
        if (csvFile.tellp() == 0) {
            csvFile << "lat,lon,alt,compass,yaw,time,sys_time,sys_date\n";
        }

        // Append data to the CSV file
        // Append data to the CSV file
        csvFile << data.latitude << ","
                << data.longitude << ","
                << data.altitude << ","
                << data.compass_heading << ","
                << data.yaw << ","
                << data.timestamp.toSec() << ","
                << ros::Time::now().toNSec() << ","
                << ros::Time::now().toSec() << std::endl;


        // Close the file after logging
        csvFile.close();
    }
}


void gpsCallback(const sensor_msgs::NavSatFix::ConstPtr &gps_msg) {
    mavros_data.latitude = gps_msg->latitude;
    mavros_data.longitude = gps_msg->longitude;
    mavros_data.timestamp = gps_msg->header.stamp;
    //data_received_time = ros::Time::now();  // Record the time when data is received
    // Append data to CSV buffer in a separate thread
    std::thread logThread(appendToCSV);
    logThread.detach();  // Detach the thread to let it run independently
}


void relAltCallback(const std_msgs::Float64::ConstPtr &rel_alt_msg) {
    mavros_data.altitude = rel_alt_msg->data;
    //data_received_time2 = ros::Time::now(); 
}

void compassCallback(const std_msgs::Float64::ConstPtr &compass_msg) {
    mavros_data.compass_heading = compass_msg->data;
    //data_received_time3 = ros::Time::now(); 
}

void yawCallback(const std_msgs::Float64::ConstPtr &pose_msg) {
    // // Compute yaw without using tf
    // // double quaternion_z = pose_msg->pose.orientation.z;
    // // double quaternion_w = pose_msg->pose.orientation.w;
    // // //data_received_time4 = ros::Time::now(); 
    // // mavros_data.yaw = atan2(2.0 * (quaternion_w * quaternion_z), 1.0 - 2.0 * (quaternion_z * quaternion_z));

    // // // Convert yaw to degrees and ensure it is in the range [0, 360)
    // // mavros_data.yaw = fmod(mavros_data.yaw * 180.0 / M_PI, 360.0);

    // // // Once all data is updated, publish to the final topic
    // // final_gps_pub.publish(mavros_data);
    // mavros_data.yaw = pose_msg->data;
    // // ros::Duration delay = (ros::Time::now() - data_received_time)*1000;
    // // ROS_INFO("GPS Delay: %f milliseconds", delay.toSec());
    // // ros::Duration delay2 = (ros::Time::now() - data_received_time2)*1000;
    // // ROS_INFO("Alt Delay: %f milliseconds", delay2.toSec());
    // // ros::Duration delay3 = (ros::Time::now() - data_received_time3)*1000;
    // // ROS_INFO("Compass Delay: %f milliseconds", delay3.toSec());
    // // ros::Duration delay4 = (ros::Time::now() - data_received_time4)*1000;
    // // ROS_INFO("Yaw Delay: %f milliseconds", delay4.toSec());
    // //ros::Time sentTime = ros::Time::now();
    // //ROS_INFO("Sent Time: %f",sentTime.toSec());
    // std::lock_guard<std::mutex> lock(csvMutex);
    // dataQueue.push(mavros_data);

    mavros_data.yaw = pose_msg->data;

    // Once all data is updated, publish to the final topic
    final_gps_pub.publish(mavros_data);

    std::lock_guard<std::mutex> lock(csvMutex);
    dataQueue.push(mavros_data);
}

    


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

    ros::spin();  // No need for a loop since we are using callbacks

    return 0;
}
