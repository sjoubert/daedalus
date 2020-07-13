#include "resources.hpp"

namespace daedalus
{

static std::filesystem::path sg_resourceDir;

void initResourceDir(std::filesystem::path p_executablePath)
{
  sg_resourceDir = p_executablePath.parent_path().parent_path() / "share" / "daedalus";
}

std::string getResource(std::string const& p_resource)
{
  return (sg_resourceDir / p_resource).string();
}

}
