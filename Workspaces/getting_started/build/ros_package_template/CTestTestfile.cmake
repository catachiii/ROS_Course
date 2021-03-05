# CMake generated Testfile for 
# Source directory: /home/ros/Workspaces/getting_started/src/ros_best_practices/ros_package_template
# Build directory: /home/ros/Workspaces/getting_started/build/ros_package_template
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(_ctest_ros_package_template_gtest_ros_package_template-test "/home/ros/Workspaces/getting_started/build/ros_package_template/catkin_generated/env_cached.sh" "/usr/bin/python3" "/opt/ros/noetic/share/catkin/cmake/test/run_tests.py" "/home/ros/Workspaces/getting_started/build/ros_package_template/test_results/ros_package_template/gtest-ros_package_template-test.xml" "--return-code" "/home/ros/Workspaces/getting_started/devel/.private/ros_package_template/lib/ros_package_template/ros_package_template-test --gtest_output=xml:/home/ros/Workspaces/getting_started/build/ros_package_template/test_results/ros_package_template/gtest-ros_package_template-test.xml")
set_tests_properties(_ctest_ros_package_template_gtest_ros_package_template-test PROPERTIES  _BACKTRACE_TRIPLES "/opt/ros/noetic/share/catkin/cmake/test/tests.cmake;160;add_test;/opt/ros/noetic/share/catkin/cmake/test/gtest.cmake;98;catkin_run_tests_target;/opt/ros/noetic/share/catkin/cmake/test/gtest.cmake;37;_catkin_add_google_test;/home/ros/Workspaces/getting_started/src/ros_best_practices/ros_package_template/CMakeLists.txt;131;catkin_add_gtest;/home/ros/Workspaces/getting_started/src/ros_best_practices/ros_package_template/CMakeLists.txt;0;")
subdirs("gtest")
