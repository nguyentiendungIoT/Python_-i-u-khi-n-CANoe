# This code was generated automatically. Changes to this file may
# cause incorrect behavior and will be lost if the code is regenerated.
import atexit
import warnings

from ._vector_typelib import typelib_cla_service as typelib_cla_service

def connect():
  typelib_cla_service.service.Connect()

def disconnect():
  typelib_cla_service.service.Disconnect()

atexit.register(typelib_cla_service.release)
