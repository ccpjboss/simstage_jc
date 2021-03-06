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
    ros::Subscriber laser_sub;
    
    double obstacle_distance;
    bool robot_stopped=false; 

    ros::Time rotate_start;     //Time when the rotation starts
    ros::Duration rotate_time;  //Rotation duration
    int rotation_orientation;

    geometry_msgs::Twist calculateCommand()
    {
        auto msg = geometry_msgs::Twist();
        
        //If the robot is not near an obstacle and it is not stopped
        if(obstacle_distance > 0.5 && this->robot_stopped == false){
            msg.linear.x = 1.0;
        }else{ //If the robot is near an obstacle
            if (this->robot_stopped == false) //If the robot was moving
            {
                //The robot will now stop
                this->robot_stopped = true;

                //Gets the time variables for the rotation
                this->rotate_start = ros::Time::now();
                int32_t nsec = rand()%9;
                rotate_time = ros::Duration(rand()%2,nsec*exp10(8));

                ROS_INFO("Rotation time: %d.%d",rotate_time.sec,rotate_time.nsec);

                //Gets the rotation orientation
                this->rotation_orientation = rand()%3 +1;
            }

            //Checks if the robot has rotated for enough time
            if(ros::Time::now()- this->rotate_start > rotate_time)
                this->robot_stopped=false;

            //Rotation orientation
            if(this->rotation_orientation%2==0)
                msg.angular.z=2;
            else
                msg.angular.z=-2;
        }
        
        return msg;
    }


    void laserCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
    {
        obstacle_distance = *std::min_element(msg->ranges.begin(), msg->ranges.end());
    }

public:
    ReactiveController(){
        // Initialize ROS
        this->n = ros::NodeHandle();

        // Create a publisher object, able to push messages
        this->cmd_vel_pub = this->n.advertise<geometry_msgs::Twist>("cmd_vel", 5);

        // Create a subscriber for laser scans 
        this->laser_sub = n.subscribe("base_scan", 10, &ReactiveController::laserCallback, this);

    }

    void run(){
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

int main(int argc, char **argv){
    // Initialize ROS
    ros::init(argc, argv, "reactive_controller");
    srand(time(0));

    // Create our controller object and run it
    auto controller = ReactiveController();
    controller.run();

    // And make good on our promise
    return 0;
}