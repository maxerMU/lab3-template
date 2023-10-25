#pragma once

#include "base_exception.h"

PROJECT_EXCEPTION(ServerException, BaseException, "(Server error) ")
PROJECT_EXCEPTION(JsonParserException, ServerException, "(Json parser error) ")
PROJECT_EXCEPTION(ConfigReaderException, ServerException, "(Config read error) ")
PROJECT_EXCEPTION(ContextPtrCastException, ServerException, "(context cast error) ")
PROJECT_EXCEPTION(ProcessingResponseException, ServerException, "(process response error) ")
