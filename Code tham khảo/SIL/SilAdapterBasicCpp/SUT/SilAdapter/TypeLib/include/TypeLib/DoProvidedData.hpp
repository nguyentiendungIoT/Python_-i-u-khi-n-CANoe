// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "DoData.hpp"

#include "cla/IDoProvidedData.hpp"
#include "cla/IValueEntity.hpp"

namespace Vector {
namespace CANoe {
namespace TypeLib {

/*!
 * \brief Class representing a provided data member of a distributed object.
 * \tparam T The tye of the data managed by the data member.
 */
template <class T>
class DoProvidedData : public DoData<T>
{
public:
  // Public Methods
  // -------------------

  /*! \brief Instantiates a new uninitialized provided data member. */
  DoProvidedData() = default;

  /*!
   * \brief Assigns a new value to the provided data member.
   * \param value The new value of the provided data member.
   */
  DoProvidedData& operator=(T value);

  /*!
   * \brief Assigns a new value to the provided data member.
   * \param value The new value of the provided data member.
   */
  DoProvidedData& SetValue(T value);

  T GetValue() const override;

protected:
  // Protected Methods
  // -------------------

  friend class Accessor; // To initialize / reset
  void _Init(Cla::IDo* distributedObject, const std::string& path, TxTrigger txTrigger);

  // Protected Members
  // -------------------

  std::shared_ptr<Cla::IDoProvidedData> mData = nullptr;
  std::shared_ptr<Cla::IValueEntity> mValueEntity = nullptr;

private:
  // Private Methods
  // -------------------

  Cla::IValueEntity* _GetValueEntity() const override;
};

// ===== Inline Implementation =====

// Public Methods
// -------------------

template <class T>
inline DoProvidedData<T>& DoProvidedData<T>::operator=(T value)
{
  return SetValue(value);
}

template <class T>
inline DoProvidedData<T>& DoProvidedData<T>::SetValue(T value)
{
  if(!mData->IsConnected())
  {
    throw std::runtime_error("Cannot set value of " + this->mPath + ". SIL Adapter not connected.");
  }

  DoData<T>::_Set(std::move(value));
  return *this;
}

template <class T>
inline T DoProvidedData<T>::GetValue() const
{
  if (!mData->IsConnected())
  {
    throw std::runtime_error("Cannot get value of " + this->mPath + ". SIL Adapter not connected.");
  }

  return DoData<T>::GetValue();
}

// Protected Methods
// -------------------

template <class T>
inline void DoProvidedData<T>::_Init(Cla::IDo* distributedObject, const std::string& path, TxTrigger txTrigger)
{
  DoData<T>::_Init(path);
  mData = distributedObject->GetProvidedData(path, txTrigger);
  mValueEntity = mData->GetValue();
  DoData<T>::_Set(T{});
}

// Private Methods
// -------------------

template <class T>
inline Cla::IValueEntity* DoProvidedData<T>::_GetValueEntity() const
{
  return mValueEntity.get();
}

} // namespace TypeLib
} // namespace CANoe
} // namespace Vector
