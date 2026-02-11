// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "DoConsumedEvent.hpp"

#include "SilKitCommunicationInfo.hpp"
#include "ValueEntity.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

// -------------------
// DoConsumedEvent
// -------------------

DoConsumedEvent::DoConsumedEvent(std::string fullPath,
  std::shared_ptr<spdlog::logger> logger,
  std::shared_ptr<WorkerThreadService> workerThreadService) :
  DoEvent(std::move(fullPath), MemberDirection::Consumed, std::move(logger), std::move(workerThreadService))
{
}

DoConsumedEvent::DoConsumedEvent(std::string fullPath,
  std::shared_ptr<ValueEntity> valueEntity,
  std::shared_ptr<spdlog::logger> logger,
  std::shared_ptr<WorkerThreadService> workerThreadService) :
  DoEvent(std::move(fullPath),
    MemberDirection::Consumed,
    std::move(valueEntity),
    std::move(logger),
    std::move(workerThreadService))
{
}

// -------------------
// DoMember
// -------------------

void DoConsumedEvent::Connect(SilKit::IParticipant* participant)
{
  mParticipant = participant;
  const SilKitCommunicationInfo communicationInfo(mFullPath);
  const auto spec = communicationInfo.CreatePubSubSpec();
  const auto dataMessageHandler = [this](SilKit::Services::PubSub::IDataSubscriber*,
                                    const SilKit::Services::PubSub::DataMessageEvent& messageEvent) {
    mValue->SetData(ToStdVector(messageEvent.data));
  };

  mSubscriber = mParticipant->CreateDataSubscriber(communicationInfo.GetCanonicalName(), spec, dataMessageHandler);
  mLogger->debug(fmt::format("Member connected: {}", GetFullPath())); // Explicit fmt::format because of VS2017
}
void DoConsumedEvent::Disconnect()
{
  mParticipant = nullptr;
  mSubscriber = nullptr;
  mLogger->debug(fmt::format("Member disconnected: {}", GetFullPath())); // Explicit fmt::format because of VS2017
}

// -------------------
// IDoMember
// -------------------

MemberType DoConsumedEvent::GetType() const
{
  return DoMember::GetType();
}
MemberDirection DoConsumedEvent::GetDirection() const
{
  return DoMember::GetDirection();
}
bool DoConsumedEvent::IsConnected() const
{
  return DoMember::IsConnected();
}

// -------------------
// IDoData
// -------------------

std::shared_ptr<IValueEntity> DoConsumedEvent::GetValue() const
{
  return DoEvent::GetValue();
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
