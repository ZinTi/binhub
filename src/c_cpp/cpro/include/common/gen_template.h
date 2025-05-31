#ifndef GEN_TEMPLATE_H
#define GEN_TEMPLATE_H

#include <string>

int write_template_src(const std::string filename);

/**
 * @brief Write a CMakeLists.txt file for a C/C++ project
 *
 * @param project_name The name of the project
 * @param is_cpp_project Whether the project is a C++ project or not
 * @return 0 if successful, -1 otherwise
 */
int write_template_cmakelists(const std::string project_name, bool is_cpp_project);

#endif