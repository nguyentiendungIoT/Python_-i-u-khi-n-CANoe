#
# Copyright (c) Vector Informatik GmbH. All rights reserved.
#
# This code was generated automatically. Changes to this file may
# cause incorrect behavior and will be lost if the code is regenerated.

from . import typelib_cla_service as typelib_cla_service


class DoInterfaceImpl:
    def __init__(self, doPath: str, embedded_prefix: str = "", do=None) -> None:
        self._doPath = doPath
        if do == None:
            self._do = typelib_cla_service.service.GetDo(self._doPath)
        else:
            self._do = do
        self._embedded_prefix = embedded_prefix

    @property
    def path(self):
        member_path = "." + self._embedded_prefix[:-1] if len(self._embedded_prefix) > 0 else ""
        return self._doPath + member_path
