#include "resources.hpp"

namespace  daedalus
{

std::string getResource(std::string const& p_resource)
{
  return DAEDALUS_RESOURCE_PATH + p_resource;
}

}
