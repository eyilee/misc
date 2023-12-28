#pragma once

// C library
#include <cstdlib>

// Containers
#include <array>
#include <deque>
#include <forward_list>
#include <map>
#include <vector>

// Input/Output
#include <filesystem>
#include <fstream>
#include <iostream>

// Multi-threading

// Other
#include <algorithm>
#include <chrono>
#include <memory>
#include <random>
#include <string>
#include <utility>

// 3rd party
#include "boost/asio.hpp"
#include "boost/program_options.hpp"

// framework
#include "framework/Event.h"
#include "framework/Random.h"
#include "framework/Time.h"
#include "framework/network/BitConverter.h"
#include "framework/network/BitStream.h"
#include "framework/network/Entity.h"
#include "framework/network/NetBridge.h"
#include "framework/network/NetProtocol.h"
#include "framework/network/NetProtocolGenerator.h"
#include "framework/network/Serializable.h"
#include "framework/network/TcpClient.h"
#include "framework/network/TcpListener.h"
#include "framework/network/TcpSession.h"
#include "framework/network/UdpSession.h"
#include "framework/manager/BaseManager.h"
#include "framework/manager/DBManager.h"
#include "framework/manager/GameLoopManager.h"
#include "framework/manager/EntityManager.h"
#include "framework/manager/EventManager.h"
#include "framework/manager/ProtocolManager.h"
#include "framework/manager/SessionManager.h"

// logger
#include "logger/Logger.h"
