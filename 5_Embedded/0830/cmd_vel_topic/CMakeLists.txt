project(cmd_vel_topic)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  message_generation
  message_runtime
  roscpp
)
find_package(Boost REQUIRED COMPONENTS system)
add_message_files(
  FILES
  Cmd_vel_msg.msg
)
generate_messages(
  DEPENDENCIES
  geometry_msgs
)
catkin_package(
 INCLUDE_DIRS include
 CATKIN_DEPENDS geometry_msgs  roscpp
)

###########
## Build ##
###########
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(cmd_vel_topic_pub src/cmd_vel_topic_pub.cpp)
add_dependencies( cmd_vel_topic_pub ${cmd_vel_topic_pub_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(cmd_vel_topic_pub ${catkin_LIBRARIES})

add_executable(cmd_vel_topic_sub src/cmd_vel_topic_sub.cpp)
add_dependencies( cmd_vel_topic_sub ${cmd_vel_topic_sub_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(cmd_vel_topic_sub ${catkin_LIBRARIES})
