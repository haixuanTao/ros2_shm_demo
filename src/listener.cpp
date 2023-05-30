// Copyright 2021 Apex.AI, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cstring>
#include <memory>
#include <chrono>

#include "rclcpp/rclcpp.hpp"

#include "ros2_shm_demo/msg/shm_topic.hpp"

class Listener : public rclcpp::Node
{
private:
    using Topic = ros2_shm_demo::msg::ShmTopic;

public:
    explicit Listener(const rclcpp::NodeOptions &options)
        : Node("shm_demo_listener", options)
    {

        // subscription callback to process arriving data
        auto callback = [this](const Topic::SharedPtr msg) -> void
        {
            // Read the message and perform operations accordingly.
            // Here we copy the data and display it.

            // Assuming you have a byte array representing the time point
            auto end = std::chrono::high_resolution_clock::now();

            // Copy the byte array with the memory representation of the time point
            // Note: Here, you would have the actual byte array obtained from a previous step

            // Initialize a time_point object to store the converted value
            std::chrono::high_resolution_clock::time_point convertedTimePoint;
            std::memcpy(&convertedTimePoint, msg->data.data(), sizeof(convertedTimePoint));
            // Calculate the duration
            std::chrono::duration<double> duration = end - convertedTimePoint;
            double elapsedmilliSeconds = duration.count() * 1000;
            last_duration += elapsedmilliSeconds / 100;

            // Print the elapsed time
            // std::cout << "Elapsed time: " << elapsedSeconds << " seconds" << std::endl;
            RCLCPP_INFO(this->get_logger(), "Received %li %f %f", msg->data.size(), elapsedmilliSeconds, last_duration);
        };

        rclcpp::QoS qos(rclcpp::KeepLast(10));
        m_subscription = create_subscription<Topic>("chatter", qos, callback);
    }

private:
    rclcpp::Subscription<Topic>::SharedPtr m_subscription;
    double last_duration;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::NodeOptions options;
    rclcpp::spin(std::make_shared<Listener>(options));
    rclcpp::shutdown();

    return 0;
}