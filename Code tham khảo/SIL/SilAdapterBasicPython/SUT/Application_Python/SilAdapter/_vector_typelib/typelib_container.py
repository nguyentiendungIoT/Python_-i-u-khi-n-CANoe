#
# Copyright (c) Vector Informatik GmbH. All rights reserved.
#
# This code was generated automatically. Changes to this file may
# cause incorrect behavior and will be lost if the code is regenerated.

from typing import TypeVar, Generic, Callable
from . import typelib_do as typelib_do
from . import typelib_cla_service as typelib_cla_service

T = TypeVar("T")

class DOGeneralContainerImpl(Generic[T]):
    def __init__(self, path, dt, min_size, max_size, do_container = None) -> None:
        if do_container == None:
            self._do_container = typelib_cla_service.service.GetDoInstContainer(path)
        else: 
            self._do_container = do_container
        self._dt = dt        
        self._path = path
        self._min_size = min_size
        self._max_size = max_size

    @property
    def path(self) -> str:
        return self._path

    def __len__(self) -> int:
        return self._do_container.GetSize()

    def __getitem__(self, index: int) -> T:
        if index >= len(self) or index < 0:
           raise IndexError(f"Index out of range.")

        val = self._do_container.GetElement(index)
        if val == None:
           raise IndexError(f"Index out of range.") 
        
        if DOContainerImpl in self._dt.__bases__:
            subcontainer = val.GetContainer()
            if subcontainer != None:
                return self._dt(self._path + f"[{index}]", subcontainer)
            else:
                raise RuntimeError(f"Internal error.") 
        else:
            do = val.GetDo()
            if do != None:
                return self._dt(self._path + f"[{index}]", "", do)
            else:
                raise RuntimeError(f"Internal error.") 


class DOContainerImpl(DOGeneralContainerImpl[T], Generic[T]):
    def __init__(self, path, dt, min_size, max_size, do_container = None) -> None:
        super().__init__(path, dt, min_size, max_size, do_container)
