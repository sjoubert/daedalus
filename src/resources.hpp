#ifndef DAEDALUS_RESOURCES_HPP
#define DAEDALUS_RESOURCES_HPP

#include <string>
#include <filesystem>

namespace daedalus
{

void initResourceDir(std::filesystem::path p_executablePath);
std::string getResource(std::string const& p_resource);

}

#endif
