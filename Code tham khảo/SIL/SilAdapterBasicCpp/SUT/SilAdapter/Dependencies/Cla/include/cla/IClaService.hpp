// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <memory>
#include <string>

#include "IDo.hpp"
#include "IDoInstContainer.hpp"

#ifdef _CANOECLA
#include "IDoInterface.hpp"
#include "IAttributeDefinition.hpp"
#include "IVirtualNetwork.hpp"
#endif

namespace Vector {
  namespace CANoe {
    namespace Cla {

      /*! \brief Interface which is the central entry point for the CLA and all its functionality. */
      class IClaService
      {
        public:
        virtual ~IClaService() = default;

        /*! \brief Gets the distributed object with the specified full path.
        *  \param path The full path of the distributed object which shall be get.
        *  \returns the distributed object. */
        virtual std::shared_ptr<IDo> GetDo(const std::string& path) = 0;

        /*! \brief Gets the distributed object instance container with the specified full path.
        *  \param path The full path of the distributed object instance container which shall be get.
        *  \returns the distributed object instance container. */
        virtual std::shared_ptr<IDoInstContainer> GetDoInstContainer(const std::string& path) = 0;
  
#ifdef _CANOECLA
        /*! \brief Gets the distributed object interface with the specified full path.
        *  \param path The full path of the distributed object interface which shall be get.
        *  \returns the distributed object interface. */
        virtual std::shared_ptr<IDoInterface> GetDoInterface(const std::string& path) = 0;

        /*! \brief Gets the attribute definition with the specified full path.
        *  \param path The full path of the attribute definition which shall be get.
        *  \returns the attribute definition. */
        virtual std::shared_ptr<IAttributeDefinition> GetAttributeDefinition(const std::string& path) = 0;

        /*! \brief Gets the virtual network with the specified full path.
        *  \param path The full path of the virtual network which shall be get.
        *  \returns the virtual network. */
        virtual std::shared_ptr<IVirtualNetwork> GetVirtualNetwork(const std::string& path) = 0;
#endif
      };

    } // namespace Cla
  } // namespace CANoe
} // namespace Vector