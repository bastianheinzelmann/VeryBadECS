#include "detail.h"

#define COMPONENT_REGISTER(TYPE, NAME)			\
	template<class T>							\
	class ComponentRegistration;				\
	template<>									\
	class ComponentRegistration<TYPE>			\
	{											\
		static const RegistryEntry<TYPE>& reg;	\									\
	};											\
												\
const RegistryEntry<TYPE>& ComponentRegistration<TYPE>::reg = \
RegistryEntry<TYPE>::Instance(NAME);			\


COMPONENT_REGISTER(Component, "String");