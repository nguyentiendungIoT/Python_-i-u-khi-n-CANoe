#
# Copyright (c) Vector Informatik GmbH. All rights reserved.
#
# This code was generated automatically. Changes to this file may
# cause incorrect behavior and will be lost if the code is regenerated.

from typing import TypeVar, Generic
from . import typelib_do as typelib_do
from . import typelib_cla_service as typelib_cla_service

T = TypeVar("T")

class EmbeddedContainerGeneralImpl(Generic[T]):
    def __init__(self, path, embedded_prefix, dt, min_size, max_size) -> None:
        self._embedded_container = []
        self._embedded_prefix = embedded_prefix
        self._dt = dt        
        self._path = path
        self._min_size = min_size
        self._max_size = max_size

        for i in range(0, max_size):
            newPrefixPart = f"[{i}]" if EmbeddedContainerImpl in self._dt.__bases__ else f"[{i}]."
            self._embedded_container.append(self._dt(self._path, self._embedded_prefix + newPrefixPart))

    @property
    def path(self) -> str:
        return self._path

    def __len__(self) -> int:
        return len(self._embedded_container)

    def __getitem__(self, index: int) -> T:
        if index >= len(self) or index < 0:
           raise IndexError(f"Index out of range.")
        
        newPrefixPart = f"[{index}]" if EmbeddedContainerImpl in self._dt.__bases__ else f"[{index}]."
        return self._dt(self._path, self._embedded_prefix + newPrefixPart)

class EmbeddedContainerImpl(EmbeddedContainerGeneralImpl[T], Generic[T]):
    def __init__(self, path, embedded_prefix, dt, min_size, max_size) -> None:
        super().__init__(path, embedded_prefix, dt, min_size, max_size)


