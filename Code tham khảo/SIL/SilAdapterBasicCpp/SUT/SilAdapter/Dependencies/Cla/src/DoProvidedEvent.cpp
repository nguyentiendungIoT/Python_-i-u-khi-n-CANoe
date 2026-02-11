// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "DoProvidedEvent.hpp"
#include "SilKitCommunicationInfo.hpp"
#include "ValueEntity.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

DoProvidedEvent::DoProvidedEvent(std::string fullPath,
  TxTrigger txTrigger,
  std::shared_ptr<spdlog::logger> logger,
  std::shared_ptr<WorkerThreadService> workerThreadService) :
  DoEvent(std::move(fullPath), MemberDirection::Provided, std::move(logger), std::move(workerThreadService)),
  mTxTrigger(txTrigger)
{
}

DoProvidedEvent::DoProvidedEvent(std::string fullPath,
  TxTrigger txTrigger,
  std::shared_ptr<ValueEntity> valueEntity,
  std::shared_ptr<spdlog::logger> logger,
  std::shared_ptr<WorkerThreadService> workerThreadService) :
  DoEvent(std::move(fullPath),
    MemberDirection::Provided,
    std::move(valueEntity),
    std::move(logger),
    std::move(workerThreadService)),
  mTxTrigger(txTrigger)
{
}

// -------------------
// DoMember
// -------------------

void DoProvidedEvent::Connect(SilKit::IParticipant* participant)
{
  mParticipant = participant;

  const SilKitCommunicationInfo info(mFullPath);
  const auto spec = info.CreatePubSubSpec();
  mPublisher = participant->CreateDataPublisher(info.GetCanonicalName(), spec, 1);

  const auto updateMode = mTxTrigger == TxTrigger::OnChange ? ValueUpdateMode::OnChange : ValueUpdateMode::OnUpdate;
  mOnTransmitHandle =
    mValue->RegisterCallback([this](IValueEntity&) { mPublisher->Publish(mValue->CopyData()); }, updateMode);

  mLogger->debug(fmt::format("Member connected: {}", GetFullPath())); // Explicit fmt::format because of VS2017
}

void DoProvidedEvent::Disconnect()
{
  mValue->UnregisterCallback(mOnTransmitHandle);
  mParticipant = nullptr;
  mPublisher = nullptr;
  mLogger->debug(fmt::format("Member disconnected: {}", GetFullPath())); // Explicit fmt::format because of VS2017
}

// -------------------
// IDoMember
// -------------------

MemberType DoProvidedEvent::GetType() const
{
  return DoMember::GetType();
}
MemberDirection DoProvidedEvent::GetDirection() const
{
  return DoMember::GetDirection();
}
bool DoProvidedEvent::IsConnected() const
{
  return DoMember::IsConnected();
}

// -------------------
// IDoEvent
// -------------------

std::shared_ptr<IValueEntity> DoProvidedEvent::GetValue() const
{
  return DoEvent::GetValue();
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
