// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "DoEvent.hpp"

#include "cla/IDoProvidedEvent.hpp"

namespace Vector {
namespace CANoe {
namespace TypeLib {

/*! \brief Class describing a consumed DO event member. */
template <class T>
class DoConsumedEvent final : public DoEvent<T>
{
public:
  // Public Methods
  // -------------------

  DoConsumedEvent() = default;

  T GetValue() const override;

  /*!
   * \brief The event member blocks on SIL Adapter when a connection
   *        to CANoe is established until the first value from CANoe is received.
   * \param timeout When a timeout for the waiting shall appear.
   */
  void BlockAtConnect(std::chrono::nanoseconds timeout);

protected:
  // Protected Methods
  // -------------------

  friend class Accessor; // To initialize / reset
  void _Init(Cla::IDo* distributedObject, const std::string& path);
  Cla::IValueEntity* _GetValueEntity() const override;
  void _WaitForFirstTransmission();

private:
  // Private members
  // -------------------

  std::shared_ptr<Cla::IDoConsumedEvent> mEvent = nullptr;
  std::shared_ptr<Cla::IValueEntity> mValueEntity = nullptr;

  // For blocking if "BlockAtConnect" is enabled
  std::chrono::nanoseconds mWaitForFirstTransmissionTimeout;
  bool mWaitForFirstTransmission = false;
  std::condition_variable mWaitForFirstTransmissionConditionVariable;
  std::mutex mWaitForFirstTransmissionMutex;
  CallbackHandle mWaitForFirstTransmissionCallbackHandle{0};
};

/*! \brief Class describing a consumed DO event member. */
template <>
class DoConsumedEvent<void> final : public DoEvent<void>
{
public:
  // Public Methods
  // -------------------

  DoConsumedEvent() = default;

  /*!
   * \brief The event member blocks on SIL Adapter when a connection
   *        to CANoe is established until the first value from CANoe is received.
   * \param timeout When a timeout for the waiting shall appear.
   */
  void BlockAtConnect(std::chrono::nanoseconds timeout);

protected:
  // Protected Methods
  // -------------------

  friend class Accessor; // To initialize / reset
  void _Init(Cla::IDo* distributedObject, const std::string& path);
  Cla::IValueEntity* _GetValueEntity() const override;
  void _WaitForFirstTransmission();

private:
  // Private members
  // -------------------

  std::shared_ptr<Cla::IDoConsumedEvent> mEvent = nullptr;
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
inline void DoConsumedEvent<T>::BlockAtConnect(std::chrono::nanoseconds timeout)
{
  mWaitForFirstTransmissionTimeout = timeout;
  if (!mWaitForFirstTransmission)
  {
    mWaitForFirstTransmissionCallbackHandle = DoEvent<T>::RegisterOnUpdateHandler([this]() {
      std::unique_lock<std::mutex> lock(mWaitForFirstTransmissionMutex);
      mWaitForFirstTransmission = false;
      lock.unlock();
      mWaitForFirstTransmissionConditionVariable.notify_all();
      DoEvent<T>::UnregisterHandler(mWaitForFirstTransmissionCallbackHandle);
    });
    mWaitForFirstTransmission = true;
  }
}

template <class T>
inline T DoConsumedEvent<T>::GetValue() const
{
  if (!mEvent->IsConnected())
  {
    throw std::runtime_error("Cannot get value of " + this->mPath + ". SIL Adapter not connected.");
  }

  return DoEvent<T>::GetValue();
}

// Protected Methods
// -------------------

template <class T>
inline void DoConsumedEvent<T>::_Init(Cla::IDo* distributedObject, const std::string& path)
{
  DoEvent<T>::_Init(path);
  mEvent = distributedObject->GetConsumedEvent(path);
  mValueEntity = mEvent->GetValue();
  DoEvent<T>::_Set(T{});
}

template <class T>
inline Cla::IValueEntity* DoConsumedEvent<T>::_GetValueEntity() const
{
  return mValueEntity.get();
}

template <class T>
inline void DoConsumedEvent<T>::_WaitForFirstTransmission()
{
  std::unique_lock<std::mutex> lock(mWaitForFirstTransmissionMutex);
  auto predicate = [this]() { return !mWaitForFirstTransmission; };
  if (!mWaitForFirstTransmissionConditionVariable.wait_for(lock, mWaitForFirstTransmissionTimeout, predicate))
  {
    throw std::runtime_error("Waiting for first transmission from CANoe: Timeout");
  }
}

inline void DoConsumedEvent<void>::BlockAtConnect(std::chrono::nanoseconds timeout)
{
  mWaitForFirstTransmissionTimeout = timeout;
  if (!mWaitForFirstTransmission)
  {
    mWaitForFirstTransmissionCallbackHandle = RegisterOnUpdateHandler([this]() {
      std::unique_lock<std::mutex> lock(mWaitForFirstTransmissionMutex);
      mWaitForFirstTransmission = false;
      lock.unlock();
      mWaitForFirstTransmissionConditionVariable.notify_all();
      UnregisterHandler(mWaitForFirstTransmissionCallbackHandle);
    });
    mWaitForFirstTransmission = true;
  }
}

inline void DoConsumedEvent<void>::_Init(Cla::IDo* distributedObject, const std::string& path)
{
  DoEvent<void>::_Init(path);
  mEvent = distributedObject->GetConsumedEvent(path);
  mValueEntity = mEvent->GetValue();
}

inline Cla::IValueEntity* DoConsumedEvent<void>::_GetValueEntity() const
{
  return mValueEntity.get();
}

inline void DoConsumedEvent<void>::_WaitForFirstTransmission()
{
  std::unique_lock<std::mutex> lock(mWaitForFirstTransmissionMutex);
  auto predicate = [this]() { return !mWaitForFirstTransmission; };
  if (!mWaitForFirstTransmissionConditionVariable.wait_for(lock, mWaitForFirstTransmissionTimeout, predicate))
  {
    throw std::runtime_error("Waiting for first event trigger from CANoe: Timeout");
  }
}

} // namespace TypeLib
} // namespace CANoe
} // namespace Vector
