from . import cppfoo


class WrapperClass:
    __slots__ = ['__private_slot', 'public_slot']

    def __str__(self):
        return '<%s: %r, %r>' % (type(self).__name__, self.__private_slot, self.public_slot)


def get_a_wrapped_object():
    return cppfoo.get_a_wrapped_object()
