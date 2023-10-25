#pragma once

#include "base_exception.h"

PROJECT_EXCEPTION(DatabaseException, BaseException, "(Database error) ")
PROJECT_EXCEPTION(DatabaseConnectException, DatabaseException, "(Database connection error) ")
PROJECT_EXCEPTION(DatabaseExecutionException, DatabaseException, "(Database execution error) ")
PROJECT_EXCEPTION(DatabaseNotFoundException, DatabaseException, "(Database not found) ")
