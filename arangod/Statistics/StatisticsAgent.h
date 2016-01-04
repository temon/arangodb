////////////////////////////////////////////////////////////////////////////////
/// @brief statistics agents
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2014-2016 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Dr. Frank Celler
/// @author Copyright 2014-2016, ArangoDB GmbH, Cologne, Germany
/// @author Copyright 2012-2013, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef ARANGODB_STATISTICS_STATISTICS_AGENT_H
#define ARANGODB_STATISTICS_STATISTICS_AGENT_H 1

#include "Basics/Common.h"

#include "Statistics/statistics.h"

// -----------------------------------------------------------------------------
// --SECTION--                                             class StatisticsAgent
// -----------------------------------------------------------------------------

namespace triagens {
namespace rest {

////////////////////////////////////////////////////////////////////////////////
/// @brief request statistics agent
////////////////////////////////////////////////////////////////////////////////

template <typename STAT, typename FUNC>
class StatisticsAgent {
  StatisticsAgent(StatisticsAgent const&) = delete;
  StatisticsAgent& operator=(StatisticsAgent const&) = delete;

  // ---------------------------------------------------------------------------
  // --SECTION--                                    constructors and destructors
  // ---------------------------------------------------------------------------

 public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief constructs a new agent
  //////////////////////////////////////////////////////////////////////////////

  StatisticsAgent() : _statistics(nullptr), _lastReadStart(0.0) {}

  //////////////////////////////////////////////////////////////////////////////
  /// @brief destructs an agent
  //////////////////////////////////////////////////////////////////////////////

  ~StatisticsAgent() {
    if (_statistics != nullptr) {
      FUNC::release(_statistics);
    }
  }

  // ---------------------------------------------------------------------------
  // --SECTION--                                                  public methods
  // ---------------------------------------------------------------------------

 public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief acquires a new statistics block
  //////////////////////////////////////////////////////////////////////////////

  STAT* acquire() {
    if (_statistics != nullptr) {
      return _statistics;
    }

    _lastReadStart = 0.0;
    return _statistics = FUNC::acquire();
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief releases a statistics block
  //////////////////////////////////////////////////////////////////////////////

  void release() {
    if (_statistics != nullptr) {
      FUNC::release(_statistics);
      _statistics = nullptr;
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief transfers statistics information to another agent
  //////////////////////////////////////////////////////////////////////////////

  void transfer(StatisticsAgent* agent) {
    agent->replace(_statistics);
    _statistics = nullptr;
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief transfers statistics information
  //////////////////////////////////////////////////////////////////////////////

  STAT* transfer() {
    STAT* statistics = _statistics;
    _statistics = nullptr;

    return statistics;
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief returns the time elapsed since read started
  //////////////////////////////////////////////////////////////////////////////

  double elapsedSinceReadStart() {
    if (_lastReadStart != 0.0) {
      return TRI_StatisticsTime() - _lastReadStart;
    }

    return 0.0;
  }

  // ---------------------------------------------------------------------------
  // --SECTION--                                                public variables
  // ---------------------------------------------------------------------------

 public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief statistics
  //////////////////////////////////////////////////////////////////////////////

  STAT* _statistics;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief last read
  //////////////////////////////////////////////////////////////////////////////

  double _lastReadStart;

  // ---------------------------------------------------------------------------
  // --SECTION--                                                  public methods
  // ---------------------------------------------------------------------------

 protected:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief replaces a statistics block
  //////////////////////////////////////////////////////////////////////////////

  void replace(STAT* statistics) {
    if (_statistics != nullptr) {
      FUNC::release(_statistics);
    }

    _statistics = statistics;
  }
};
}
}

// -----------------------------------------------------------------------------
// --SECTION--                                      class RequestStatisticsAgent
// -----------------------------------------------------------------------------

namespace triagens {
namespace rest {

////////////////////////////////////////////////////////////////////////////////
/// @brief request statistics description
////////////////////////////////////////////////////////////////////////////////

struct RequestStatisticsAgentDesc {
  static TRI_request_statistics_t* acquire() {
    return TRI_AcquireRequestStatistics();
  }

  static void release(TRI_request_statistics_t* stat) {
    TRI_ReleaseRequestStatistics(stat);
  }
};

////////////////////////////////////////////////////////////////////////////////
/// @brief request statistics agent
////////////////////////////////////////////////////////////////////////////////

class RequestStatisticsAgent
    : public StatisticsAgent<TRI_request_statistics_t,
                             RequestStatisticsAgentDesc> {
 public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the request type
  //////////////////////////////////////////////////////////////////////////////

  void requestStatisticsAgentSetRequestType(HttpRequest::HttpRequestType b) {
    if (TRI_ENABLE_STATISTICS) {
      if (_statistics != nullptr) {
        _statistics->_requestType = b;
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @Brief sets the async flag
  //////////////////////////////////////////////////////////////////////////////

  void requestStatisticsAgentSetAsync() {
    if (TRI_ENABLE_STATISTICS) {
      if (_statistics != nullptr) {
        _statistics->_async = true;
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the read start
  //////////////////////////////////////////////////////////////////////////////

  void requestStatisticsAgentSetReadStart() {
    if (TRI_ENABLE_STATISTICS) {
      if (_statistics != nullptr && _statistics->_readStart == 0.0) {
        _lastReadStart = _statistics->_readStart = TRI_StatisticsTime();
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the read end
  //////////////////////////////////////////////////////////////////////////////

  void requestStatisticsAgentSetReadEnd() {
    if (TRI_ENABLE_STATISTICS) {
      if (_statistics != nullptr) {
        _statistics->_readEnd = TRI_StatisticsTime();
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the write start
  //////////////////////////////////////////////////////////////////////////////

  void requestStatisticsAgentSetWriteStart() {
    if (TRI_ENABLE_STATISTICS) {
      if (_statistics != nullptr) {
        _statistics->_writeStart = TRI_StatisticsTime();
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the write end
  //////////////////////////////////////////////////////////////////////////////

  void requestStatisticsAgentSetWriteEnd() {
    if (TRI_ENABLE_STATISTICS) {
      if (_statistics != nullptr) {
        _statistics->_writeEnd = TRI_StatisticsTime();
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the queue start
  //////////////////////////////////////////////////////////////////////////////

  void requestStatisticsAgentSetQueueStart() {
    if (TRI_ENABLE_STATISTICS) {
      if (_statistics != nullptr) {
        _statistics->_queueStart = TRI_StatisticsTime();
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the queue end
  //////////////////////////////////////////////////////////////////////////////

  void requestStatisticsAgentSetQueueEnd() {
    if (TRI_ENABLE_STATISTICS) {
      if (_statistics != nullptr) {
        _statistics->_queueEnd = TRI_StatisticsTime();
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the request start
  //////////////////////////////////////////////////////////////////////////////

  void requestStatisticsAgentSetRequestStart() {
    if (TRI_ENABLE_STATISTICS) {
      if (_statistics != nullptr) {
        _statistics->_requestStart = TRI_StatisticsTime();
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the request end
  //////////////////////////////////////////////////////////////////////////////

  void requestStatisticsAgentSetRequestEnd() {
    if (TRI_ENABLE_STATISTICS) {
      if (_statistics != nullptr) {
        _statistics->_requestEnd = TRI_StatisticsTime();
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets execution error
  //////////////////////////////////////////////////////////////////////////////

  void requestStatisticsAgentSetExecuteError() {
    if (TRI_ENABLE_STATISTICS) {
      if (_statistics != nullptr) {
        _statistics->_executeError = true;
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets ignore flag
  //////////////////////////////////////////////////////////////////////////////

  void requestStatisticsAgentSetIgnore() {
    if (TRI_ENABLE_STATISTICS) {
      if (_statistics != nullptr) {
        _statistics->_ignore = true;
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief adds bytes received
  //////////////////////////////////////////////////////////////////////////////

  void requestStatisticsAgentAddReceivedBytes(size_t b) {
    if (TRI_ENABLE_STATISTICS) {
      if (_statistics != nullptr) {
        _statistics->_receivedBytes += b;
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief adds bytes sent
  //////////////////////////////////////////////////////////////////////////////

  void requestStatisticsAgentAddSentBytes(size_t b) {
    if (TRI_ENABLE_STATISTICS) {
      if (_statistics != nullptr) {
        _statistics->_sentBytes += b;
      }
    }
  }
};
}
}

// -----------------------------------------------------------------------------
// --SECTION--                                   class ConnectionStatisticsAgent
// -----------------------------------------------------------------------------

namespace triagens {
namespace rest {

////////////////////////////////////////////////////////////////////////////////
/// @brief connection statistics description
////////////////////////////////////////////////////////////////////////////////

struct ConnectionStatisticsAgentDesc {
  static TRI_connection_statistics_t* acquire() {
    return TRI_AcquireConnectionStatistics();
  }

  static void release(TRI_connection_statistics_t* stat) {
    TRI_ReleaseConnectionStatistics(stat);
  }
};

////////////////////////////////////////////////////////////////////////////////
/// @brief connection statistics agent
////////////////////////////////////////////////////////////////////////////////

class ConnectionStatisticsAgent
    : public StatisticsAgent<TRI_connection_statistics_t,
                             ConnectionStatisticsAgentDesc> {
 public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the connection type
  //////////////////////////////////////////////////////////////////////////////

  void connectionStatisticsAgentSetHttp() {
    if (TRI_ENABLE_STATISTICS) {
      if (_statistics != nullptr) {
        _statistics->_http = true;
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the connection start
  //////////////////////////////////////////////////////////////////////////////

  void connectionStatisticsAgentSetStart() {
    if (TRI_ENABLE_STATISTICS) {
      if (_statistics != nullptr) {
        _statistics->_connStart = TRI_StatisticsTime();
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets the connection end
  //////////////////////////////////////////////////////////////////////////////

  void connectionStatisticsAgentSetEnd() {
    if (TRI_ENABLE_STATISTICS) {
      if (_statistics != nullptr) {
        _statistics->_connEnd = TRI_StatisticsTime();
      }
    }
  }
};
}
}

#endif

// -----------------------------------------------------------------------------
// --SECTION--                                                       END-OF-FILE
// -----------------------------------------------------------------------------
