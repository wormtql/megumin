add_library(keystone SHARED IMPORTED)

set_target_properties(keystone PROPERTIES IMPORTED_LOCATION keystone.dll)
set_target_properties(keystone PROPERTIES IMPORTED_IMPLIB keystone.lib)
#set_target_properties(keystone PROPERTIES LINKER_LANGUAGE C)
