/**
@file	 AsyncTcpServerBase.h
@brief   The base class for creating a TCP server
@author  Tobias Blomberg
@date    2003-12-07

\verbatim
Async - A library for programming event driven applications
Copyright (C) 2003-2025 Tobias Blomberg / SM0SVX

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
\endverbatim
*/

#ifndef ASYNC_TCP_SERVER_BASE_INCLUDED
#define ASYNC_TCP_SERVER_BASE_INCLUDED


/****************************************************************************
 *
 * System Includes
 *
 ****************************************************************************/

#include <string>
#include <vector>
#include <map>
#include <sigc++/sigc++.h>


/****************************************************************************
 *
 * Project Includes
 *
 ****************************************************************************/

#include <AsyncTimer.h>
#include <AsyncTcpConnection.h>
#include <AsyncSslContext.h>


/****************************************************************************
 *
 * Local Includes
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Forward declarations
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Namespace
 *
 ****************************************************************************/

namespace Async
{

/****************************************************************************
 *
 * Forward declarations of classes inside of the declared namespace
 *
 ****************************************************************************/

class FdWatch;


/****************************************************************************
 *
 * Defines & typedefs
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Exported Global Variables
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Class definitions
 *
 ****************************************************************************/

/**
@brief	The base class for creating a TCP server
@author Tobias Blomberg
@date   2003-12-07
*/
class TcpServerBase : public sigc::trackable
{
  public:
    /**
     * @brief 	Default constuctor
     * @param 	port_str A port number or service name to listen to
     * @param 	bind_ip The IP to bind the server to
     */
    TcpServerBase(const std::string& port_str,
                  const Async::IpAddress &bind_ip);

    /**
     * @brief 	Destructor
     */
    virtual ~TcpServerBase(void);

    /**
     * @brief 	Get the number of clients that is connected to the server
     * @return 	The number of connected clients
     */
    int numberOfClients(void);

    /**
     * @brief 	Get the client object pointer from the server
     * @param 	index The wanted client by number 0 - numberOfClients()-1
     * @return 	The TcpConnection pointer to the client (zero if not found)
     */
    TcpConnection *getClient(unsigned int index);

    /**
     * @brief 	Write data to all connected clients 
     * @param 	buf   The data buffer
     * @param 	count The number of bytes in the data buffer
     * @return 	The number of bytes sent
     */
    int writeAll(const void *buf, int count);

    /**
     * @brief 	Send data only to the given client
     * @param 	con   The TcpConnection object to send to
     * @param 	buf   The data buffer
     * @param 	count The number of bytes in data buffer
     * @return 	The number of bytes sent
     */
    int writeOnly(TcpConnection *con, const void *buf, int count);

    /**
     * @brief 	Send data to all connected clients except the given client
     * @param 	con   The TcpConnection object not to send to
     * @param 	buf   The data buffer
     * @param 	count The number of bytes in the data buffer
     * @return 	The number of bytes sent
     */
    int writeExcept(TcpConnection *con, const void *buf, int count);

    /**
     * @brief   Set the SSL context for all new connections
     * @param   ctx The SSL context to set
     *
     * Call this function to set an SSL context that is applied automatically
     * for all client connections. If this is set up in the server all that
     * need to be done, in a client connection after it is established, is to
     * enable SSL by calling enableSsl() on the connection object.
     *
     * NOTE: The context object is neither copied nor managed by this class so
     * it must be persisted by the caller for as long as this class or any
     * connections live. It is also the responsibility for the caller to delete
     * the context object when it fills no purpose anymore.
     */
    void setSslContext(SslContext& ctx);

    /**
     * @brief   Enable connection throttling
     * @param   bucket_max The size of the bucket
     * @param   bucket_inc How much to add to the bucket on each tick
     * @param   inc_interval_ms How often, in milliseconds, to apply bucket_inc
     *
     * Use this function to put a limit on the connection rate for each IP
     * address. The "token bucket" algorithm is used to throttle connection
     * flooding. The connection will be accepted but the communication will be
     * blocked until a token is available in the bucket.
     *
     * Example: bucket_max=5, bucket_inc=0.1, inc_interval_ms=1000
     * With those settings five connections from an IP will be accepted without
     * delay. Each second (1000ms) the bucket for the IP will be filled with
     * 0.1 tokens so it will take ten seconds until another connection will be
     * allowed to communicate. If no additional connections for the IP are
     * initiated, the bucket will fill to five tokens again efter 50 seconds
     * and we are back to where we started.
     */
    void setConnectionThrottling(unsigned bucket_max, float bucket_inc,
                                 int inc_interval_ms);

  protected:
    virtual void createConnection(int sock, const IpAddress& remote_addr,
                                  uint16_t remote_port) = 0;
    void addConnection(TcpConnection *con);
    void removeConnection(TcpConnection *con);
    virtual void emitClientConnected(TcpConnection *con) = 0;

  private:
    using TcpConnectionList = std::vector<TcpConnection*>;
    struct ConThrotItem
    {
      ConThrotItem(float bucket_max) : m_bucket(bucket_max) {}
      float             m_bucket;
      TcpConnectionList m_pending_connections;
    };
    using ConThrotMap = std::map<IpAddress, ConThrotItem>;

    int               m_sock;
    FdWatch*          m_rd_watch;
    TcpConnectionList m_tcpConnectionList;
    SslContext*       m_ssl_ctx               = nullptr;

    ConThrotMap       m_con_throt_map;
    Timer             m_con_throt_timer;
    unsigned          m_con_throt_bucket_max  = 0;
    unsigned          m_con_throt_bucket_inc  = 0;

    void cleanup(void);
    void onConnection(FdWatch *watch);
    void updateConnThrotMap(Timer*);

};  /* class TcpServerBase */


} /* namespace */

#endif /* ASYNC_TCP_SERVER_BASE_INCLUDED */



/*
 * This file has not been truncated
 */



