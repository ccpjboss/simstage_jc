
#include <iostream>

#include <cstdlib>

#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"

class ReactiveController
{
private:
    ros::NodeHandle n;
    ros::Publisher cmd_vel_pub;

    ros::Subscriber sonar_f;
    ros::Subscriber sonar_e;
    ros::Subscriber sonar_d;
    ros::Subscriber sonar_fe;
    ros::Subscriber sonar_fd;

    double dist_f;
    double dist_e;
    double dist_d;
    double dist_fe;
    double dist_fd;

    bool robot_stopped = false;

    ros::Time rotate_start;    //Time when the rotation starts
    ros::Duration rotate_time; //Rotation duration
    int rotation_orientation;

    geometry_msgs::Twist calculateCommand()
    {
        auto msg = geometry_msgs::Twist();
        double dist_wall = 2.5;

        if (dist_f > dist_wall  && dist_fe > dist_wall  && dist_fd > dist_wall )
        {
            msg.linear.x = 0.5;
            msg.angular.z = -0.3;
            ROS_INFO("Finding wall");
        }
        else if (dist_f < dist_wall && dist_fe > dist_wall && dist_fd > dist_wall )
        {
            msg.angular.z = 0.7;
            ROS_INFO("Turning left");
        }
        else if (dist_f < dist_wall && dist_fe > dist_wall && dist_fd < dist_wall )
        {
            msg.angular.z = 0.7;
            ROS_INFO("Turning left");
        }
        else if (dist_f < dist_wall && dist_fe < dist_wall && dist_fd > dist_wall )
        {
            msg.angular.z = 0.7;
            ROS_INFO("Turning left");
        }
        else if (dist_f < dist_wall && dist_fe < dist_wall && dist_fd < dist_wall )
        {
            msg.angular.z = 0.7;
            ROS_INFO("Turning left");
        }
        else if (dist_f > dist_wall && dist_fe > dist_wall && dist_fd < dist_wall && dist_d<dist_wall )
        {
            msg.linear.x = 0.5;
            ROS_INFO("Follow wall");
        }
        else if (dist_f > dist_wall && dist_fe < dist_wall && dist_fd > dist_wall )
        {
            msg.linear.x = 0.5;
            msg.angular.z = -0.3;
            ROS_INFO("Finding wall");
        }
        else if (dist_f > dist_wall && dist_fe < dist_wall && dist_fd < dist_wall )
        {
            msg.linear.x = 0.5;
            msg.angular.z = -0.3;
            ROS_INFO("Finding wall");
        }
        else
        {
            ROS_INFO("No state!");
        }

        return msg;
    }

    void sonarFrente(const sensor_msgs::LaserScan::ConstPtr &msg)
    {
        dist_f = *std::min_element(msg->ranges.begin(), msg->ranges.end());
    }

    void sonarEsq(const sensor_msgs::LaserScan::ConstPtr &msg)
    {
        dist_e = *std::min_element(msg->ranges.begin(), msg->ranges.end());
    }

    void sonarDir(const sensor_msgs::LaserScan::ConstPtr &msg)
    {
        dist_d = *std::min_element(msg->ranges.begin(), msg->ranges.end());
    }

    void sonarFEsq(const sensor_msgs::LaserScan::ConstPtr &msg)
    {
        dist_fe = *std::min_element(msg->ranges.begin(), msg->ranges.end());
    }

    void sonarFDir(const sensor_msgs::LaserScan::ConstPtr &msg)
    {
        dist_fd = *std::min_element(msg->ranges.begin(), msg->ranges.end());
    }

public:
    ReactiveController()
    {
        // Initialize ROS
        this->n = ros::NodeHandle();

        // Create a publisher object, able to push messages
        this->cmd_vel_pub = this->n.advertise<geometry_msgs::Twist>("cmd_vel", 5);

        // Create a subscriber for laser scans
        this->sonar_f = n.subscribe("sonar_frente", 10, &ReactiveController::sonarFrente, this);
        this->sonar_e = n.subscribe("sonar_esquerda",10, &ReactiveController::sonarEsq, this);
        this->sonar_d = n.subscribe("sonar_direita", 10, &ReactiveController::sonarDir, this);
        this->sonar_fe = n.subscribe("sonar_fesquerda",10, &ReactiveController::sonarFEsq, this);
        this->sonar_fd = n.subscribe("sonar_fdireita",10, &ReactiveController::sonarFDir, this);
    }

    void run()
    {
        // Send messages in a loop
        ros::Rate loop_rate(10);
        while (ros::ok())
        {
            // Calculate the command to apply
            auto msg = calculateCommand();

            // Publish the new command
            this->cmd_vel_pub.publish(msg);

            ros::spinOnce();

            // And throttle the loop
            loop_rate.sleep();
        }
    }
};

int main(int argc, char **argv)
{
    // Initialize ROS
    ros::init(argc, argv, "wall_controller");
    srand(time(0));

    // Create our controller object and run it
    auto controller = ReactiveController();
    controller.run();

    // And make good on our promise
    return 0;
}
