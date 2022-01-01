#include "object_wrapper.h"

ObjectWrapper::ObjectWrapper(void* obj) : obj(reinterpret_cast<BaseObject*>(obj)) {}
