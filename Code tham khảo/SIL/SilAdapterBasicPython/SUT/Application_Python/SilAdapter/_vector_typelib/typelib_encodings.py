#
# Copyright (c) Vector Informatik GmbH. All rights reserved.
#
# This code was generated automatically. Changes to this file may
# cause incorrect behavior and will be lost if the code is regenerated.

class Encoding:
    def __init__(self, factor, offset):
        self.factor = factor
        self.offset = offset

    def Encode(self, v):
        if (self.factor == 1.0):
            return v + self.offset
        else:
            return self.factor * v + self.offset

    def Decode(self, v):
        if (self.factor == 1.0):
            return v - self.offset
        else:
            return (v - self.offset) / self.factor


IdentityEncoding = Encoding(1.0, 0)
