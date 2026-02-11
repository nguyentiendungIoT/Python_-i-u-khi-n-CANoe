// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "cla/IDo.hpp"

#include "DoRegistry.hpp"
#include "cla/IDoInstContainer.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

/*! \brief Class representing a single element of a distributed object container.
 *         Can be a distributed object or a nested container. Its type is chosen by
 *         calling `GetDo` or `GetContainer`.
 */
class DoInstContainerElement : public IDoInstContainerElement
{
public:
  // Public methods
  // -------------------

  /*! \brief Instantiates a new container element.
   *  \param fullPath The full path of the container element, including the indices.
   *  \param doRegistry The registry, on which the distributed object is registered if
   *                    `GetDo` is called. */
  DoInstContainerElement(std::string fullPath, DoRegistry& doRegistry);

  virtual ~DoInstContainerElement() = default;

  // ------------------------
  // IDoInstContainerElement
  // ------------------------

  std::shared_ptr<IDo> GetDo() override;
  std::shared_ptr<IDoInstContainer> GetContainer() override;

private:
  // Private members
  // -------------------

  std::shared_ptr<IDoInstContainer> mDoContainer;
  IDo* mDoHandle = nullptr;
  DoRegistry& mDoRegistry;
  std::string mPath;
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
