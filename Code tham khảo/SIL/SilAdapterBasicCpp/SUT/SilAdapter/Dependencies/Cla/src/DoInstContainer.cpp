// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "DoInstContainer.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

// -------------------
// DoInstContainer
// -------------------

DoInstContainer::DoInstContainer(std::string fullPath, DoRegistry& doRegistry) :
  mDoRegistry(doRegistry),
  mPath(std::move(fullPath))
{
}

// -------------------
// IDoInstContainer
// -------------------

ContainerSize DoInstContainer::GetSize() const
{
  return static_cast<ContainerSize>(mDoInstContainerElements.size());
}

IDoInstContainerElement* DoInstContainer::GetElement(ContainerSize index)
{
  if (index < mDoInstContainerElements.size() && mDoInstContainerElements.at(index) != nullptr)
  {
    return mDoInstContainerElements.at(index).get();
  }
  if (index >= mDoInstContainerElements.size())
  {
    mDoInstContainerElements.resize(static_cast<size_t>(index) + 1);
  }
  mDoInstContainerElements[index] = std::make_unique<DoInstContainerElement>(mPath + "[" + std::to_string(index) + "]", mDoRegistry);
  return mDoInstContainerElements[index].get();
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
