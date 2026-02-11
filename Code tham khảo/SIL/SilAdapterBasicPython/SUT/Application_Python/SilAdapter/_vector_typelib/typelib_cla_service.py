# This code was generated automatically. Changes to this file may
# cause incorrect behavior and will be lost if the code is regenerated.

from enum import IntEnum
import canoe_sil_adapter_runtime.cla

service = canoe_sil_adapter_runtime.cla.CreateClaService()

def release():
   global service
   service = None
