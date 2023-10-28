#pragma once

#include "base_exception.h"

PROJECT_EXCEPTION(ServerException, BaseException, "(Server error) ")
PROJECT_EXCEPTION(JsonParserException, ServerException, "(Json parser error) ")
PROJECT_EXCEPTION(ConfigReaderException, ServerException, "(Config read error) ")
PROJECT_EXCEPTION(ContextPtrCastException, ServerException, "(context cast error) ")
PROJECT_EXCEPTION(ProcessingResponseException, ServerException, "(process response error) ")
PROJECT_EXCEPTION(ClientNotFoundException, ServerException, "(client not found error) ")
PROJECT_EXCEPTION(ClientNotConnectedException, ServerException, "(client connection error) ")
PROJECT_EXCEPTION(ClientOpenedCircuitBreakerException, ServerException, "(client circuit breaker is openned error) ")
