// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "cla/ClaError.hpp"

#include "DoProvidedField.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

// -------------------
// DoProvidedField
// -------------------

DoProvidedField::DoProvidedField(std::string fullPath,
  TxTrigger txTrigger,
  std::shared_ptr<spdlog::logger> logger,
  std::shared_ptr<WorkerThreadService> workerThreadService) :
  DoField(std::move(fullPath), MemberDirection::Provided, std::move(logger), std::move(workerThreadService)),
  mTxTrigger(txTrigger)
{
}

// -------------------
// DoMember
// -------------------

void DoProvidedField::Connect(SilKit::IParticipant* participant)
{
  mParticipant = participant;
  if (mGetter)
  {
    mGetter->Connect(mParticipant);
  }
  if (mSetter)
  {
    mSetter->Connect(mParticipant);
  }
  if (mEvent)
  {
    mEvent->Connect(mParticipant);
  }
}
void DoProvidedField::Disconnect()
{
  if (mEvent)
  {
    mEvent->Disconnect();
  }
  if (mSetter)
  {
    mSetter->Disconnect();
  }
  if (mGetter)
  {
    mGetter->Disconnect();
  }
  mParticipant = nullptr;
}

MemberType DoProvidedField::GetType() const
{
  return DoMember::GetType();
}
MemberDirection DoProvidedField::GetDirection() const
{
  return DoMember::GetDirection();
}
bool DoProvidedField::IsConnected() const
{
  return DoMember::IsConnected();
}

// -------------------
// IDoField
// -------------------

std::shared_ptr<IValueEntity> DoProvidedField::GetValue() const
{
  return DoField::GetValue();
}
std::shared_ptr<IDoMethod> DoProvidedField::GetGetter()
{
  return GetProvidedGetter();
}
std::shared_ptr<IDoMethod> DoProvidedField::GetSetter()
{
  return GetProvidedSetter();
}
std::shared_ptr<IDoEvent> DoProvidedField::GetEvent()
{
  return GetProvidedEvent();
}

// -------------------
// IDoConsumedField
// -------------------

std::shared_ptr<IDoProvidedMethod> DoProvidedField::GetProvidedGetter() const
{
  if (DoMember::IsConnected())
  {
    throw ClaError("Cannot get new getter while the provided field member is connected.");
  }

  if (!mGetter)
  {
    mGetter = std::make_shared<Getter>(GetGetterPath(), mValue, mLogger, mWorkerThreadService);
  }

  return mGetter;
}
std::shared_ptr<IDoProvidedMethod> DoProvidedField::GetProvidedSetter() const
{
  if (DoMember::IsConnected())
  {
    throw ClaError("Cannot get new setter while the provided field member is connected");
  }

  if (!mSetter)
  {
    mSetter = std::make_shared<Setter>(GetSetterPath(), mValue, mLogger, mWorkerThreadService);
  }

  return mSetter;
}
std::shared_ptr<IDoProvidedEvent> DoProvidedField::GetProvidedEvent() const
{
  if (DoMember::IsConnected())
  {
    throw ClaError("Cannot get new event while the provided field member is connected.");
  }

  if (!mEvent)
  {
    mEvent = std::make_shared<DoProvidedEvent>(GetEventPath(), mTxTrigger, mValue, mLogger, mWorkerThreadService);
  }

  return mEvent;
}

// ===== DoProvidedField::Getter =====

DoProvidedField::Getter::Getter(std::string fullPath,
  std::shared_ptr<ValueEntity> valueEntity,
  std::shared_ptr<spdlog::logger> logger,
  std::shared_ptr<WorkerThreadService> workerThreadService) :
  DoProvidedMethod(std::move(fullPath), std::move(logger), std::move(workerThreadService)),
  mValue(std::move(valueEntity))
{
  SetDefaultCallback();
}

void DoProvidedField::Getter::SetCallback(std::function<void(IProviderCallContext&)> callback)
{
  if (callback)
  {
    DoProvidedMethod::SetCallback(callback);
  }
  else
  {
    SetDefaultCallback();
  }
}

void DoProvidedField::Getter::SetDefaultCallback()
{
  std::lock_guard<std::mutex> lock(mCallbackMutex);
  mCallback = [this](ProviderCallContext& cc) { cc.SetOutputParametersData(mValue->CopyData()); };
}

// ===== DoProvidedField::Setter =====

DoProvidedField::Setter::Setter(std::string fullPath,
  std::shared_ptr<ValueEntity> valueEntity,
  std::shared_ptr<spdlog::logger> logger,
  std::shared_ptr<WorkerThreadService> workerThreadService) :
  DoProvidedMethod(std::move(fullPath), std::move(logger), std::move(workerThreadService)),
  mValue(std::move(valueEntity))
{
  SetDefaultCallback();
}

void DoProvidedField::Setter::SetCallback(std::function<void(IProviderCallContext&)> callback)
{
  if (callback)
  {
    DoProvidedMethod::SetCallback(callback);
  }
  else
  {
    SetDefaultCallback();
  }
}

void DoProvidedField::Setter::SetDefaultCallback()
{
  std::lock_guard<std::mutex> lock(mCallbackMutex);
  mCallback = [this](ProviderCallContext& cc) {
    auto data = cc.GetInputParametersData();
    mValue->SetData(data);
    cc.SetOutputParametersData(std::move(data));
  };
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
