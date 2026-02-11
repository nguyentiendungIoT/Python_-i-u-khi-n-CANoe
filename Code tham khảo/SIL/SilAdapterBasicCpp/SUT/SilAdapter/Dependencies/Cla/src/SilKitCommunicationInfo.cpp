// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "SilKitCommunicationInfo.hpp"

#include "silkit/util/serdes/Serialization.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

SilKitCommunicationInfo::SilKitCommunicationInfo(const std::string& fullPath)
{
  mLabels["VirtualNetwork"] = "Default";
  const auto namespaceIdentifierPos = fullPath.find_last_of("::");
  const auto instanceIdentifierPos = fullPath.find_last_of('.');
  mLabels["Instance"] = fullPath.substr(namespaceIdentifierPos + 1, instanceIdentifierPos - namespaceIdentifierPos - 1);
  mLabels["Namespace"] = fullPath.substr(0, namespaceIdentifierPos - 1);
  mTopic = fullPath.substr(instanceIdentifierPos + 1, fullPath.length());
  mCanonicalName = fullPath;
}

template <class SpecT>
static SpecT MakeSpec(const std::string& topic,
  const std::string& mimeType,
  const std::map<std::string, std::string>& labels)
{
  SpecT spec(topic, mimeType);
  for (const auto& label : labels)
  {
    spec.AddLabel(label.first, label.second, SilKit::Services::MatchingLabel::Kind::Mandatory);
  }
  return spec;
}

SilKit::Services::PubSub::PubSubSpec SilKitCommunicationInfo::CreatePubSubSpec() const
{
  return MakeSpec<SilKit::Services::PubSub::PubSubSpec>(mTopic, GetDataMimeType(), mLabels);
}

SilKit::Services::Rpc::RpcSpec SilKitCommunicationInfo::CreateRpcSpec() const
{
  return MakeSpec<SilKit::Services::Rpc::RpcSpec>(mTopic, GetMethodMimeType(), mLabels);
}

std::string SilKitCommunicationInfo::GetDataMimeType()
{
  return SilKit::Util::SerDes::MediaTypeData();
}

std::string SilKitCommunicationInfo::GetMethodMimeType()
{
  return SilKit::Util::SerDes::MediaTypeRpc();
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
