// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "cla/ClaError.hpp"

#include "Do.hpp"
#include "DoConsumedData.hpp"
#include "DoConsumedEvent.hpp"
#include "DoConsumedField.hpp"
#include "DoConsumedMethod.hpp"
#include "DoProvidedData.hpp"
#include "DoProvidedEvent.hpp"
#include "DoProvidedField.hpp"
#include "DoProvidedMethod.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

// -------------------
// Do
// -------------------

Do::Do(std::string path,
  std::shared_ptr<spdlog::logger> logger,
  std::shared_ptr<WorkerThreadService> workerThreadService) :
  mPath(std::move(path)),
  mLogger(std::move(logger)),
  mWorkerThreadService(std::move(workerThreadService))
{
}

void Do::Connect(SilKit::IParticipant* participant)
{
  mLogger->debug(fmt::format("Connecting members of {}...", mPath)); // Explicit fmt::format because of VS2017
  mParticipant = participant;
  for (auto& member : mMembers)
  {
    member.second->Connect(mParticipant);
  }
}

void Do::Disconnect()
{
  for (auto& member : mMembers)
  {
    member.second->Disconnect();
  }
  mParticipant = nullptr;
}

// -------------------
// IDo
// -------------------

std::shared_ptr<IDoConsumedData> Do::GetConsumedData(const std::string& path)
{
  return GetMember<DoConsumedData>(path);
}

std::shared_ptr<IDoProvidedData> Do::GetProvidedData(const std::string& path, TxTrigger txTrigger)
{
  return GetMember<DoProvidedData>(path, txTrigger);
}

std::shared_ptr<IDoConsumedMethod> Do::GetConsumedMethod(const std::string& path)
{
  return GetMember<DoConsumedMethod>(path);
}

std::shared_ptr<IDoProvidedMethod> Do::GetProvidedMethod(const std::string& path)
{
  return GetMember<DoProvidedMethod>(path);
}

std::shared_ptr<IDoConsumedEvent> Do::GetConsumedEvent(const std::string& path)
{
  return GetMember<DoConsumedEvent>(path);
}

std::shared_ptr<IDoProvidedEvent> Do::GetProvidedEvent(const std::string& path, TxTrigger txTrigger)
{
  return GetMember<DoProvidedEvent>(path, txTrigger);
}

std::shared_ptr<IDoConsumedField> Do::GetConsumedField(const std::string& path)
{
  return GetMember<DoConsumedField>(path);
}

std::shared_ptr<IDoProvidedField> Do::GetProvidedField(const std::string& path, TxTrigger txTrigger)
{
  return GetMember<DoProvidedField>(path, txTrigger);
}

std::string Do::GetPath()
{
  return mPath;
}

template <class T, class... Args>
std::shared_ptr<T> Do::GetMember(const std::string& path, Args&&... args)
{
  auto&& it = mMembers.find(path);
  if (it != mMembers.end())
  {
    auto member = std::dynamic_pointer_cast<T>(it->second);

    if (!member)
    {
      throw ClaError("DO member with existing path '" + path + "' but different type was requested");
    }

    return member;
  }

  if (mParticipant)
  {
    throw ClaError("Cannot get new DO member while the DO is connected.");
  }

  const auto fullPath = mPath + "." + path;
  auto newMember = std::make_shared<T>(fullPath, std::forward<Args>(args)..., mLogger, mWorkerThreadService);
  mMembers[path] = newMember;

  mLogger->debug(fmt::format("Created DO member '{}'", fullPath)); // Explicit fmt::format because of VS2017
  return newMember;
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
