/*#include "ros/ros.h"
#include <sensor_msgs/NavSatFix.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/PoseStamped.h>
#include "geotagger/MavrosData.h"  // Replace 'your_package_name' with the actual name of your package



/ Global variables to store MAVROS data
double latitude = 0.0;
double longitude = 0.0;
double altitude = 0.0;
double compass_heading = 0.0;
double yaw = 0.0;


// Global variables for final GPS data
ros::Publisher final_gps_pub;
//geometry_msgs::PoseStamped final_gps_data;

// Callback function for GPS data
void gpsCallback(const sensor_msgs::NavSatFix::ConstPtr &gps_msg) {
    /*latitude = gps_msg->latitude;
    longitude = gps_msg->longitude;
    geotagger::MavrosData mavros_data;  // Replace 'your_package_name'
    mavros_data.latitude = gps_msg->latitude;
    mavros_data.longitude = gps_msg->longitude;
    mavros_data.altitude = altitude;  // You need to update altitude accordingly
    mavros_data.compass_heading = compass_heading;
    mavros_data.yaw = yaw;
    mavros_data.timestamp = gps_msg->header.stamp;

    final_gps_pub.publish(mavros_data);

    // Update final GPS data
    /*final_gps_data.pose.position.x = longitude;
    final_gps_data.pose.position.y = latitude;
    final_gps_data.pose.position.z = altitude;  // Assuming altitude is in meters
    final_gps_data.header.stamp = ros::Time::now();
    final_gps_pub.publish(final_gps_data);
}

// Callback function for relative altitude
void relAltCallback(const std_msgs::Float64::ConstPtr &rel_alt_msg) {
    altitude = rel_alt_msg->data;
}

// Callback function for compass heading
void compassCallback(const std_msgs::Float64::ConstPtr &compass_msg) {
    compass_heading = compass_msg->data;
}

// Callback function for yaw
void yawCallback(const geometry_msgs::PoseStamped::ConstPtr &pose_msg) {
    yaw = pose_msg->pose.orientation.y;
}

int main(int argc, char *argv[]) {
    ros::init(argc, argv, "MAVROS_Publisher_Node");
    ros::NodeHandle nh;
    final_gps_pub = nh.advertise<geotagger::MavrosData>("final_gps_data_topic", 10);


    // Subscribe to GPS data
    ros::Subscriber gps_sub = nh.subscribe<sensor_msgs::NavSatFix>(
        "mavros/global_position/global",
        10,
        gpsCallback
    );

    // Subscribe to relative altitude data
    ros::Subscriber rel_alt_sub = nh.subscribe<std_msgs::Float64>(
        "mavros/global_position/rel_alt",
        10,
        relAltCallback
    );

    // Subscribe to compass heading data
    ros::Subscriber compass_sub = nh.subscribe<std_msgs::Float64>(
        "mavros/global_position/compass_hdg",
        10,
        compassCallback
    );

    // Subscribe to yaw data
    ros::Subscriber yaw_sub = nh.subscribe<geometry_msgs::PoseStamped>(
        "mavros/local_position/pose",
        10,
        yawCallback
    );

    // Advertise the final GPS data topic
    //final_gps_pub = nh.advertise<geometry_msgs::PoseStamped>("final_gps_data", 10);

    ros::Rate rate(50);  // Adjust the rate based on your requirements

    while (ros::ok()) {
        // Print data with timestamp
        //ROS_INFO("Time: %f, Latitude: %f, Longitude: %f, Altitude: %f, Compass Heading: %f, Yaw: %f",
                //ros::Time::now().toSec(), latitude, longitude, altitude, compass_heading, yaw);

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}
*/
// #include "ros/ros.h"
// #include <sensor_msgs/NavSatFix.h>
// #include <std_msgs/Float64.h>
// #include <geometry_msgs/PoseStamped.h>
// #include "geotagger/MavrosData.h"

// ros::Publisher final_gps_pub;
// geotagger::MavrosData mavros_data;  // Global variable to store synchronized data

// void gpsCallback(const sensor_msgs::NavSatFix::ConstPtr &gps_msg) {
//     mavros_data.latitude = gps_msg->latitude;
//     mavros_data.longitude = gps_msg->longitude;
//     mavros_data.timestamp = gps_msg->header.stamp;
// }

// void relAltCallback(const std_msgs::Float64::ConstPtr &rel_alt_msg) {
//     mavros_data.altitude = rel_alt_msg->data;
// }

// void compassCallback(const std_msgs::Float64::ConstPtr &compass_msg) {
//     mavros_data.compass_heading = compass_msg->data;
// }

// void yawCallback(const geometry_msgs::PoseStamped::ConstPtr &pose_msg) {
//     mavros_data.yaw = pose_msg->pose.orientation.y;

//     // Once all data is updated, publish to the final topic
//     final_gps_pub.publish(mavros_data);
// }

// int main(int argc, char *argv[]) {
//     ros::init(argc, argv, "Mavros_Publisher_Node");
//     ros::NodeHandle nh;

//     final_gps_pub = nh.advertise<geotagger::MavrosData>("final_gps_data_topic", 10);

//     ros::Subscriber gps_sub = nh.subscribe<sensor_msgs::NavSatFix>(
//         "mavros/global_position/global",
//         10,
//         gpsCallback
//     );

//     ros::Subscriber rel_alt_sub = nh.subscribe<std_msgs::Float64>(
//         "mavros/global_position/rel_alt",
//         10,
//         relAltCallback
//     );

//     ros::Subscriber compass_sub = nh.subscribe<std_msgs::Float64>(
//         "mavros/global_position/compass_hdg",
//         10,
//         compassCallback
//     );

//     ros::Subscriber yaw_sub = nh.subscribe<geometry_msgs::PoseStamped>(
//         "mavros/local_position/pose",
//         10,
//         yawCallback
//     );

//     ros::Rate rate(50);

//     while (ros::ok()) {
//         ros::spinOnce();
//         rate.sleep();
//     }

//     return 0;
// }





#include "ros/ros.h"
#include <sensor_msgs/NavSatFix.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/PoseStamped.h>
#include "geotagger/MavrosData.h"
#include <tf/transform_datatypes.h>

ros::Publisher final_gps_pub;
geotagger::MavrosData mavros_data;  // Global variable to store synchronized data

void gpsCallback(const sensor_msgs::NavSatFix::ConstPtr &gps_msg) {
    mavros_data.latitude = gps_msg->latitude;
    mavros_data.longitude = gps_msg->longitude;
    mavros_data.timestamp = gps_msg->header.stamp;
}

void relAltCallback(const std_msgs::Float64::ConstPtr &rel_alt_msg) {
    mavros_data.altitude = rel_alt_msg->data;
}

void compassCallback(const std_msgs::Float64::ConstPtr &compass_msg) {
    mavros_data.compass_heading = compass_msg->data;
}

void yawCallback(const geometry_msgs::PoseStamped::ConstPtr &pose_msg) {
    double yaw = tf::getYaw(pose_msg->pose.orientation);
    mavros_data.yaw = fmod(yaw * 180.0 / M_PI, 360.0);

    // Once all data is updated, publish to the final topic
    final_gps_pub.publish(mavros_data);
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

    ros::Subscriber yaw_sub = nh.subscribe<geometry_msgs::PoseStamped>(
        "mavros/local_position/pose",
        10,
        yawCallback
    );

    ros::Rate rate(50);

    while (ros::ok()) {
        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}