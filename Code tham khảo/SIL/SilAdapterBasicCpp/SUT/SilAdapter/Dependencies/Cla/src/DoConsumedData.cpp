// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "DoConsumedData.hpp"

#include "SilKitCommunicationInfo.hpp"
#include "ValueEntity.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

// -------------------
// DoConsumedData
// -------------------

DoConsumedData::DoConsumedData(std::string fullPath,
  std::shared_ptr<spdlog::logger> logger,
  std::shared_ptr<WorkerThreadService> workerThreadService) :
  DoData(std::move(fullPath), MemberDirection::Consumed, std::move(logger), std::move(workerThreadService))
{
}

// -------------------
// DoMember
// -------------------

void DoConsumedData::Connect(SilKit::IParticipant* participant)
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
void DoConsumedData::Disconnect()
{
  mParticipant = nullptr;
  mSubscriber = nullptr;
  mLogger->debug(fmt::format("Member disconnected: {}", GetFullPath())); // Explicit fmt::format because of VS2017
}

// -------------------
// IDoMember
// -------------------

MemberType DoConsumedData::GetType() const
{
  return DoMember::GetType();
}
MemberDirection DoConsumedData::GetDirection() const
{
  return DoMember::GetDirection();
}
bool DoConsumedData::IsConnected() const
{
  return DoMember::IsConnected();
}

// -------------------
// IDoData
// -------------------

std::shared_ptr<IValueEntity> DoConsumedData::GetValue() const
{
  return DoData::GetValue();
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
