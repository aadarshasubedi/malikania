# FindMongo
# -----------
#
# Find mongo-c-driver library, this modules defines:
#
# Mongo_INCLUDE_DIRS, where to find jansson.h
# Mongo_LIBRARIES, where to find library
# Mongo_FOUND, if it is found

find_path(
	Mongo_INCLUDE_DIR
	NAMES mongoc.h
	PATH_SUFFIXES libmongoc-1.0
)

find_library(
	Mongo_LIBRARY
	NAMES
		libmongoc mongoc
		libmongoc-1.0 mongoc-1.0
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
	Mongo
	FOUND_VAR Mongo_FOUND
	REQUIRED_VARS Mongo_LIBRARY Mongo_INCLUDE_DIR
)

set(Mongo_LIBRARIES ${Mongo_LIBRARY})
set(Mongo_INCLUDE_DIRS ${Mongo_INCLUDE_DIR})

mark_as_advanced(Mongo_INCLUDE_DIR Mongo_LIBRARY)
