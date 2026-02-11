// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <stdexcept>

#include "DoData.hpp"

#include "cla/IDoConsumedData.hpp"
#include "cla/IValueEntity.hpp"

namespace Vector {
namespace CANoe {
namespace TypeLib {

/*! \brief Class representing a consumed data member of a distributed object.
 *  \tparam T The type of the data managed by the data member. */
template <class T>
class DoConsumedData : public DoData<T>
{
public:
  // Public Members
  // -------------------

  /*! \brief Instantiates a new uninitialized consumed data member. */
  DoConsumedData() = default;

  /*!
   * \brief The data member blocks on SIL Adapter when a connection
   *        to CANoe is established until the first value from CANoe is received.
   * \param timeout When a timeout for the waiting shall appear.
   */
  void BlockAtConnect(std::chrono::nanoseconds timeout);

  T GetValue() const override;

protected:
  // Protected Methods
  // -------------------

  friend class Accessor; // To initialize / reset
  void _Init(Cla::IDo* distributedObject, const std::string& path);

private:
  // Private Methods
  // -------------------

  Cla::IValueEntity* _GetValueEntity() const override;
  void _WaitForFirstTransmission();

  // Private Members
  // -------------------

  std::shared_ptr<Cla::IDoConsumedData> mData = nullptr;
  std::shared_ptr<Cla::IValueEntity> mValueEntity = nullptr;

  // For blocking if "BlockAtConnect" is enabled
  std::chrono::nanoseconds mWaitForFirstTransmissionTimeout;
  bool mWaitForFirstTransmission = false;
  std::condition_variable mWaitForFirstTransmissionConditionVariable;
  std::mutex mWaitForFirstTransmissionMutex;
  CallbackHandle mWaitForFirstTransmissionCallbackHandle{0};
};

// ===== Inline Implementation =====

// Public methods

template <class T>
inline void DoConsumedData<T>::BlockAtConnect(std::chrono::nanoseconds timeout)
{
  mWaitForFirstTransmissionTimeout = timeout;
  if (!mWaitForFirstTransmission)
  {
    mWaitForFirstTransmissionCallbackHandle = DoData<T>::RegisterOnUpdateHandler([this]() {
      std::unique_lock<std::mutex> lock(mWaitForFirstTransmissionMutex);
      mWaitForFirstTransmission = false;
      lock.unlock();
      mWaitForFirstTransmissionConditionVariable.notify_all();
      DoData<T>::UnregisterHandler(mWaitForFirstTransmissionCallbackHandle);
    });
    mWaitForFirstTransmission = true;
  }
}

template <class T>
inline T DoConsumedData<T>::GetValue() const
{
  if (!mData->IsConnected())
  {
    throw std::runtime_error("Cannot get value of " + this->mPath + ". SIL Adapter not connected.");
  }

  return DoData<T>::GetValue();
}

// Protected Members
// -------------------

template <class T>
inline void DoConsumedData<T>::_Init(Cla::IDo* distributedObject, const std::string& path)
{
  DoData<T>::_Init(path);
  mData = distributedObject->GetConsumedData(path);
  mValueEntity = mData->GetValue();
  DoData<T>::_Set(T{});
}

// Private Methods
// -------------------

template <class T>
inline Cla::IValueEntity* DoConsumedData<T>::_GetValueEntity() const
{
  return mValueEntity.get();
}

template <class T>
inline void DoConsumedData<T>::_WaitForFirstTransmission()
{
  std::unique_lock<std::mutex> lock(mWaitForFirstTransmissionMutex);
  auto predicate = [this]() { return !mWaitForFirstTransmission; };
  if (!mWaitForFirstTransmissionConditionVariable.wait_for(lock, mWaitForFirstTransmissionTimeout, predicate))
  {
    throw std::runtime_error("Waiting for first value from CANoe: Timeout");
  }
}

} // namespace TypeLib
} // namespace CANoe
} // namespace Vector
