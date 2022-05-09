#pragma once

// C library
#include <cstdlib>

// Containers
#include <map>
#include <vector>
#include <forward_list>

// Input/Output
#include <filesystem>
#include <fstream>
#include <iostream>

// Multi-threading

// Other
#include <chrono>
#include <memory>
#include <utility>

// 3rd party
#include "boost/asio.hpp"
#include "boost/program_options.hpp"

// framework
#include "framework/event/Event.h"
#include "framework/network/Entity.h"
#include "framework/network/InStream.h"
#include "framework/network/NetBridge.h"
#include "framework/network/NetProtocol.h"
#include "framework/network/NetProtocolGenerator.h"
#include "framework/network/OutStream.h"
#include "framework/network/TcpClient.h"
#include "framework/network/TcpListener.h"
#include "framework/network/TcpSession.h"
#include "framework/manager/BaseManager.h"
#include "framework/manager/DBManager.h"
#include "framework/manager/EntityManager.h"
#include "framework/manager/EventManager.h"
#include "framework/manager/ProtocolManager.h"
#include "framework/manager/SessionManager.h"

// logger
#include "logger/Logger.h"
