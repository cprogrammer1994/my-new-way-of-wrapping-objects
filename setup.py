from setuptools import setup, Extension

cppfoo = Extension(
    name='foobar.cppfoo',
    sources=['foobar/cppfoo.cpp'],
)

setup(
    name='foobar',
    version='0.1.0',
    packaged=['foobar'],
    ext_modules=[cppfoo],
)
