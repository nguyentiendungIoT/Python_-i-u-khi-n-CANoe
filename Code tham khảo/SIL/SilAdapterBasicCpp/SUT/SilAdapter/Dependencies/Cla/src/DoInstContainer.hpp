// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "cla/IDoInstContainer.hpp"

#include "DoRegistry.hpp"
#include "DoInstContainerElement.hpp"
#include "SpdlogLibrary.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

/*! \brief Class representing a container of distributed objects or nested containers. */
class DoInstContainer : public IDoInstContainer
{
public:
  // Public methods
  // -------------------

  /*! \brief Instantiates a new distributed object container.
   *  \param fullPath The full path of the container.
   *  \param doRegistry The DO registry on which new distributed objects of the container are registered. */
  DoInstContainer(std::string fullPath, DoRegistry& doRegistry);

  // -------------------
  // IDoInstContainer
  // -------------------

  ContainerSize GetSize() const override;
  IDoInstContainerElement* GetElement(ContainerSize index) override;

private:
  // Private members
  // -------------------

  DoRegistry& mDoRegistry;
  std::vector<std::unique_ptr<DoInstContainerElement>> mDoInstContainerElements;
  std::string mPath;
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
