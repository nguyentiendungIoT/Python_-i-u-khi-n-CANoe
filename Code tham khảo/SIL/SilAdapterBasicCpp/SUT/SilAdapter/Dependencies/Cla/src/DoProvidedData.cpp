// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "DoProvidedData.hpp"

#include "SilKitCommunicationInfo.hpp"
#include "ValueEntity.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

DoProvidedData::DoProvidedData(std::string fullPath,
  TxTrigger txTrigger,
  std::shared_ptr<spdlog::logger> logger,
  std::shared_ptr<WorkerThreadService> workerThreadService) :
  DoData(std::move(fullPath), MemberDirection::Provided, std::move(logger), std::move(workerThreadService)),
  mTxTrigger(txTrigger)
{
}

// -------------------
// DoMember
// -------------------

void DoProvidedData::Connect(SilKit::IParticipant* participant)
{
  mParticipant = participant;

  const SilKitCommunicationInfo communicationInfo(mFullPath);
  const auto spec = communicationInfo.CreatePubSubSpec();
  mPublisher = participant->CreateDataPublisher(communicationInfo.GetCanonicalName(), spec, 1);

  const auto updateMode = mTxTrigger == TxTrigger::OnChange ? ValueUpdateMode::OnChange : ValueUpdateMode::OnUpdate;
  mOnTransmitHandle =
    mValue->RegisterCallback([this](IValueEntity&) { mPublisher->Publish(mValue->CopyData()); }, updateMode);

  mLogger->debug(fmt::format("Member connected: {}", GetFullPath())); // Explicit fmt::format because of VS2017
}
void DoProvidedData::Disconnect()
{
  mValue->UnregisterCallback(mOnTransmitHandle);
  mParticipant = nullptr;
  mPublisher = nullptr;
  mLogger->debug(fmt::format("Member disconnected: {}", GetFullPath())); // Explicit fmt::format because of VS2017
}

// -------------------
// IDoMember
// -------------------

MemberType DoProvidedData::GetType() const
{
  return DoMember::GetType();
}
MemberDirection DoProvidedData::GetDirection() const
{
  return DoMember::GetDirection();
}
bool DoProvidedData::IsConnected() const
{
  return DoMember::IsConnected();
}

// -------------------
// IDoData
// -------------------

std::shared_ptr<IValueEntity> DoProvidedData::GetValue() const
{
  return DoData::GetValue();
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
