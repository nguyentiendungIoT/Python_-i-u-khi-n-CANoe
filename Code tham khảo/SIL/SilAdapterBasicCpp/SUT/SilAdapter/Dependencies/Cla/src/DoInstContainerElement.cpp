// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "DoInstContainerElement.hpp"
#include "DoInstContainer.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

// ------------------------
// DoInstContainerElement
// ------------------------

DoInstContainerElement::DoInstContainerElement(std::string fullPath, DoRegistry& doRegistry) :
  mDoRegistry(doRegistry),
  mPath(std::move(fullPath))
{
}

// ------------------------
// IDoInstContainerElement
// ------------------------

std::shared_ptr<IDo> DoInstContainerElement::GetDo()
{
  assert(!mDoContainer && "Cannot call GetDo on DoInstContainerElement that already holds a Do container.");
  auto distObj = mDoRegistry.GetDo(mPath);
  mDoHandle = distObj.get();
  return distObj;
}

std::shared_ptr<IDoInstContainer> DoInstContainerElement::GetContainer()
{
  assert(!mDoHandle && "Cannot call GetContainer on DoInstContainerElement that already holds a Do.");
  if (mDoContainer == nullptr)
  {
    mDoContainer = std::make_shared<DoInstContainer>(mPath, mDoRegistry);
  }
  return mDoContainer;
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
