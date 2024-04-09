#pragma once
#include <memory>
#include <exception>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>

// OpenSRT
#include "srt.h"

namespace srt
{

using buffer = std::vector<uint8_t>;

class exception : public std::exception
{
    public:
        explicit exception(const std::string &&err)
            : m_error_msg(std::move(err))
        { }

    public:
        virtual const char *what() const throw() {
            return m_error_msg.c_str(); }

    private:
        const std::string m_error_msg;
};

class socket : public std::enable_shared_from_this<socket>
{
    using string     = std::string;
    using shared_srt = std::shared_ptr<socket>;

    public:
    explicit socket(std::string host,
            uint16_t port, bool blocking,
            std::map<string, string> options);

    socket(const int sock, bool blocking);

    virtual ~socket();

    public:

    shared_srt connect();
    shared_srt accept();

    /**
     * Start listening on the incomming connection requests.
     *
     * May throw a socket::exception.
     */
    void listen() noexcept(false);

    /// Verifies URI options provided are valid.
    ///
    /// @param [in] options  a map of options key-value pairs to validate
    /// @param [in] extra a set of extra options that are valid, e.g. "mode", "bind"
    /// @throw socket::exception on failure
    ///
    static void assert_options_valid(const std::map<string, string>& options, const std::unordered_set<string>& extra);

    static std::string print_negotiated_config(SRTSOCKET);

    private:
    void configure(const std::map<string, string>& options);

    void assert_options_valid() const;
    int  configure_pre(SRTSOCKET sock);
    int  configure_post(SRTSOCKET sock) const;
    void handle_hosts();

    public:

    /**
     * @returns A buffer containing received bytes.
     *
     * @throws socket_exception Thrown on failure.
     */
    buffer read(size_t max_len, int timeout_ms = -1);
    int    write(const buffer& buffer, int timeout_ms = -1);

    enum connection_mode
    {
        FAILURE    = -1,
        LISTENER   = 0,
        CALLER     = 1,
        RENDEZVOUS = 2
    };

    connection_mode mode() const;

    bool is_caller() const { return m_mode == CALLER; }

    public:
    SRTSOCKET                   id() const { return m_bind_socket; }
    int                         statistics(SRT_TRACEBSTATS& stats, bool instant = true);
    bool                        supports_statistics() const { return true; }
    static const std::string    stats_to_csv(int socketid, const SRT_TRACEBSTATS& stats, bool print_header);

    private:
    void raise_exception(const string&& place) const;
    void raise_exception(const string&& place, const string&& reason) const;

    private:
    SRTSOCKET m_bind_socket = SRT_INVALID_SOCK;
    int m_epoll_connect = -1;
    int m_epoll_io      = -1;

    connection_mode          m_mode          = FAILURE;
    bool                     m_blocking_mode = false;
    string                   m_host;
    int                      m_port = -1;
    std::map<string, string> m_options; // All other options, as provided in the URI
};

} // namespace srt
