// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <map>
#include <string>
#include <vector>

#include "silkit/services/pubsub/PubSubSpec.hpp"
#include "silkit/services/rpc/RpcSpec.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

/*! \brief Class holding all the relevant identifiers to address specific communication participants. */
class SilKitCommunicationInfo
{
public:
  // Public methods
  // ------------------

  /*! \brief Instantiates a new SilKitCommunicationInfo for holding identifying information about communication
   * participant. \param fullPath Member path which is used for parsing communication info. */
  SilKitCommunicationInfo(const std::string& fullPath);
  SilKitCommunicationInfo(SilKitCommunicationInfo&) = delete;
  SilKitCommunicationInfo(SilKitCommunicationInfo&&) = delete;
  void operator=(SilKitCommunicationInfo&) = delete;
  void operator=(SilKitCommunicationInfo&&) = delete;
  ~SilKitCommunicationInfo() = default;

  SilKit::Services::PubSub::PubSubSpec CreatePubSubSpec() const;
  SilKit::Services::Rpc::RpcSpec CreateRpcSpec() const;

  /*! \brief Returns Data Mime Type. */
  static std::string GetDataMimeType();

  /*! \brief Returns Method Mime Type. */
  static std::string GetMethodMimeType();

  size_t GetNumLabels() const
  {
    return mLabels.size();
  }

  const std::string& GetTopic() const
  {
    return mTopic;
  }

  const std::string& GetNamespace() const
  {
    return mLabels.at("Namespace");
  }

  const std::string& GetVirtualNetwork() const
  {
    return mLabels.at("VirtualNetwork");
  }

  const std::string& GetInstance() const
  {
    return mLabels.at("Instance");
  }

  std::string GetCanonicalName() const
  {
    return mCanonicalName;
  }

private:
  // Private members
  // ------------------

  std::map<std::string, std::string> mLabels{};
  std::string mTopic;
  std::string mCanonicalName;
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
