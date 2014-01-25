/* 
 * File:   Debugger.cpp
 * Author: samuel
 * 
 * Created on 24 January, 2014, 3:25 AM
 */

#include <Debugger.hpp>

Debugger::Debugger() {
}

Debugger::Debugger(ros::NodeHandle node_handle) {
    initialize();

    cv::namedWindow(window_name.c_str(), 0);
    image = cv::Mat(cv::Size(map_height, map_width), CV_8UC3, cv::Scalar::all(255));

    pose_subscriber = node_handle.subscribe("localization/pose", 2, &Debugger::setCurrent_pose, this);
    maneuver_subscriber = node_handle.subscribe("situational_planner/maneuver", 2, &Debugger::setManeuver, this);
}

Debugger::Debugger(const Debugger& orig) {
}

Debugger::~Debugger() {
}

void Debugger::display(int debug_mode) {
    image = cv::Scalar(255, 255, 255);

    for (unsigned int pose_id = 0; pose_id + 1 < maneuver.poses.size(); pose_id++) {
        cv::line(image,
                 cv::Point((maneuver.poses.at(pose_id).pose.position.x - current_pose.position.x) * scale + map_width / 2,
                           (maneuver.poses.at(pose_id).pose.position.y - current_pose.position.y) * scale + map_height / 2),
                 cv::Point((maneuver.poses.at(pose_id + 1).pose.position.x - current_pose.position.x) * scale + map_width / 2,
                           (maneuver.poses.at(pose_id + 1).pose.position.y - current_pose.position.y) * scale + map_height / 2),
                 cv::Scalar(0, 0, 255), 2, CV_AA, 0);
    }

    for (unsigned int pose_id = 0; pose_id < maneuver.poses.size(); pose_id++) {
        cv::circle(image,
                   cv::Point((maneuver.poses.at(pose_id).pose.position.x - current_pose.position.x) * scale + map_width / 2,
                             (maneuver.poses.at(pose_id).pose.position.y - current_pose.position.y) * scale + map_height / 2),
                   1, cv::Scalar(0, 255, 0), 1, CV_AA, 0);
    }

    cv::circle(image, cv::Point(map_width / 2, map_height / 2), 5, cv::Scalar(255, 0, 0), 3, CV_AA, 0);

    if (debug_mode == 1) {
        cv::imshow(window_name.c_str(), image);
        cv::waitKey(10);
    }
}

void Debugger::initialize() {
    map_width = 500;
    map_height = 500;
    scale = 25;
    window_name = std::string("Situational Planner");
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "situational_planner_debugger");
    ros::NodeHandle node_handle;
    Debugger debugger(node_handle);

    ros::Rate loop_rate(100);
    while (ros::ok()) {
        int debug_mode;
        node_handle.getParam("/situational_planner_debugger/debug_mode", debug_mode);
        debugger.display(debug_mode);
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}