#include "resources.hpp"

#include <filesystem>

namespace daedalus
{

static std::filesystem::path const RESOURCE_DIR =
  std::filesystem::canonical("/proc/self/exe") // Resolve real executable location
    .parent_path().parent_path() // Move up to the install prefix
    / "share" / "daedalus"; // Move down to the resource dir

std::string getResource(std::string const& p_resource)
{
  return (RESOURCE_DIR / p_resource).string();
}

}
