#include <Python.h>
#include <structmember.h>

#define SLOT(obj, type, offset) (*(type **)((char *)obj + offset))

PyObject * foobar;
PyTypeObject * WrapperClass_type;
int WrapperClass_private_slot;
int WrapperClass_public_slot;

PyObject * meth_get_a_wrapped_object(PyObject * self, PyObject * args) {
	static bool initialized = false;

	if (!initialized) {
		initialized = true;
		WrapperClass_type = (PyTypeObject *)PyObject_GetAttrString(foobar, "WrapperClass");
		printf("WrapperClass_type = %p\n", WrapperClass_type);

		PyMemberDef * tp_members = WrapperClass_type->tp_members;
		printf("tp_members = %p\n", tp_members);
		printf("\n");

		for (PyMemberDef * member = tp_members; member->name; ++member) {
			printf("member = %p\n", member);
			printf("name = %s\n", member->name);
			printf("offset = %d\n", member->offset);
			printf("\n");

			if (!strcmp(member->name, "_WrapperClass__private_slot")) {
				WrapperClass_private_slot = member->offset;
			}
			if (!strcmp(member->name, "public_slot")) {
				WrapperClass_public_slot = member->offset;
			}
		}
	}

	Py_INCREF(WrapperClass_type);
	PyObject * res = (WrapperClass_type->tp_flags & Py_TPFLAGS_HAVE_GC) ? PyObject_GC_New(PyObject, WrapperClass_type) : PyObject_New(PyObject, WrapperClass_type);
	SLOT(res, PyObject, WrapperClass_private_slot) = PyUnicode_FromString("private slot");
	SLOT(res, PyObject, WrapperClass_public_slot) = PyUnicode_FromString("public slot");
	return res;
}

PyMethodDef methods[] = {
	{"get_a_wrapped_object", (PyCFunction)meth_get_a_wrapped_object, METH_VARARGS, 0},
	{0},
};

PyModuleDef moduledef = {PyModuleDef_HEAD_INIT, "foobar.cppfoo", 0, -1, methods, 0, 0, 0, 0};

extern "C" PyObject * PyInit_cppfoo() {
	PyObject * module = PyModule_Create(&moduledef);

	foobar = PyImport_ImportModule("foobar");
	if (!foobar) {
		return 0;
	}

	return module;
}
