// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "cla/IDoMember.hpp"

#include "silkit/SilKit.hpp"

#include "SpdlogLibrary.hpp"
#include "WorkerThreadService.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

/*! \brief Class representing a member of a distributed object. */
class DoMember : public IDoMember
{
public:
  /*! \brief Instantiates a new member of a distributed object.
   *  \param fullPath The full path of the member, i.e. "namespace::do.member"
   *  \param type The type of the member.
   *  \param direction The direction of the member.
   *  \param logger The logger used by the member and its child elements for logging.
   *  \param workerThreadService service to add custom tasks to be executed by a worker thread. */
  DoMember(std::string fullPath, MemberType type, MemberDirection direction, std::shared_ptr<spdlog::logger> logger, std::shared_ptr<WorkerThreadService> workerThreadService);

  /*! \brief Returns the full path of the member, i.e. "namespace::do.member".
   *  \returns the full path of the member. */
  const std::string& GetFullPath() const;

  /*! \brief Returns the name of the virtual network of the data member.
   *  \returns The name of the virtual network of this data member. */
  std::string GetVirtualNetworkName() const;

  /*! \brief Connect Member to CANoe via passed participant
   *  \param participant for which connection should be established */
  virtual void Connect(SilKit::IParticipant* participant) = 0;

  /*! \brief Disconnects the member with CANoe. */
  virtual void Disconnect() = 0;

  /*! \brief Returns true if participant of member is set.
   *  \returns true if participant is set. */
  bool IsConnected() const override;

  // -------------------
  // IDoMember
  // -------------------

  MemberType GetType() const override;
  MemberDirection GetDirection() const override;

protected:
  // Protected Members
  // -------------------

  std::shared_ptr<spdlog::logger> mLogger;
  SilKit::IParticipant* mParticipant = nullptr;
  std::string mFullPath;
  std::shared_ptr<WorkerThreadService> mWorkerThreadService;

private:
  // Private Members
  // -------------------

  MemberType mType;
  MemberDirection mDirection;
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
