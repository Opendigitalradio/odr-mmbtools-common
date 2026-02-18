/* Testing file
 *
   Copyright (C) 2019
   Matthias P. Braendli, matthias.braendli@mpb.li

   A test main.
 */
/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include "ClockTAI.h"
#include "Log.h"
#include "ReedSolomon.h"
#include "RemoteControl.h"
#include "charset/charset.h"
#include "edi/STIDecoder.hpp"
#include "edi/STIWriter.hpp"
#include "edioutput/AFPacket.h"
#include "srt/netaddr_any.hpp"
#include "srt/srt_socket.hpp"
#include "src/webserver.h"

#include <srt.h>

using namespace std;

static int usage(const char *progname)
{
    cerr << "Usage : " << progname << " (text|srt_rx)\n";
    return 1;
}

static int test()
{
    ClockTAI ct;
    ct.init({"https://127.0.0.1", "https://example.com", "https://raw.githubusercontent.com/eggert/tz/master/leap-seconds.lis"});
    etiLog.level(info) << "TAI offset = " << ct.get_offset();

    WebServer webserver("127.0.0.1", 9091, "This is the index");

    ReedSolomon rs(255, 207);

    RemoteControllerTelnet telnet(2121);
    etiLog.level(info) << "RC telnet started";

    rcs.enrol(&ct);

    CharsetConverter charset_converter;
    const auto ebu_text = charset_converter.utf8_to_ebu("From UTF to EBU: éö", false);
    const auto utf_text = charset_converter.ebu_to_utf8(ebu_text);
    etiLog.level(info) << "EBU-UTF8 conversion: " << utf_text;

    auto sti_frame_cb = [](EdiDecoder::sti_frame_t&& f) {
        etiLog.level(info) << "Got STI frame with TS = " << f.timestamp.to_string();
    };
    EdiDecoder::STIWriter edi_stiwriter(sti_frame_cb);
    EdiDecoder::STIDecoder edi_decoder(edi_stiwriter);

    etiLog.level(info) << "Sleep 3";
    this_thread::sleep_for(chrono::seconds(3));
    etiLog.level(info) << "TAI offset is " << ct.get_offset() << " " << map_to_json(ct.get_all_values());

    this_thread::sleep_for(chrono::seconds(3));
    etiLog.level(info) << "Override to 38";
    ct.set_parameter("tai_utc_offset", "38");

    for (size_t i = 0; i < 3; i++) {
        this_thread::sleep_for(chrono::seconds(1));
        etiLog.level(info) << "TAI offset is " << ct.get_offset() << " " << map_to_json(ct.get_all_values());
    }

    this_thread::sleep_for(chrono::seconds(1));
    auto new_url = "https://raw.githubusercontent.com/eggert/tz/master/leap-seconds.list";
    etiLog.level(info) << "Set URL to " << new_url;
    ct.set_parameter("url", new_url);

    while (true) {
        this_thread::sleep_for(chrono::seconds(3));
        etiLog.level(info) << "TAI offset is " << ct.get_offset() << " " << map_to_json(ct.get_all_values());
    }
}

int srt_rx()
{
    std::string host = "0.0.0.0";
    uint16_t port = 8952;
    std::map<string, string> options{
        {"mode", "listener"},
        //{"bind", "127.0.0.1"}
    };

    srt_startup();
    srt_setloglevel(srt_logging::LogLevel::debug);

    bool blocking = false;
    srt::socket s(host, port, blocking, options);
    s.listen();

    Socket::UDPSocket s2;
    Socket::InetAddress dest;
    dest.resolveUdpDestination("127.0.0.1", 8951);

    while (true) {
        try {
            auto data_socket = s.accept();

            while (true) {
                try {
                    vector<uint8_t> buf = data_socket->read(2048);
                    s2.send(buf, dest);
                }
                catch (const srt::exception& e) {
                    etiLog.level(error) << " SRT exception:" << e.what();
                    break;
                }
            }
        }
        catch (const srt::exception& e) {
            etiLog.level(error) << " SRT failed to accept:" << e.what();
        }

        this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;

    /*
    SRTSOCKET bind_socket = srt_create_socket();
    if (bind_socket == SRT_INVALID_SOCK) {
        throw srt::exception(srt_getlasterror_str());
    }

    srt::netaddr_any sa = srt::create_addr("127.0.0.1", 8952);
    const int res = srt_connect(bind_socket, sa.get(), sa.size());
    if (res == SRT_ERROR) {
        etiLog.level(error) << srt_getlasterror_str();
        return 1;
    }

    while (true) {
        vector<uint8_t> buf(2048);
        SRT_MSGCTRL mc = srt_msgctrl_default;
        const int res = srt_recvmsg2(bind_socket, reinterpret_cast<char*>(buf.data()), (int)buf.size(), &mc);
        if (res > 0) {
            buf.resize(res);
            etiLog.level(debug) << buf.size();
            if (buf.size() == 0) {
                break;
            }

            s2.send(buf, dest);
        }
        else if (res == 0) {
            break;
        }
        else {
            etiLog.level(error) << srt_getlasterror_str();
            return 1;
        }
    }
    */
}

int main(int argc, char **argv)
{
    std::string cmd;
    if (argc > 1) {
        cmd = argv[1];
    }

    if (argc == 1) {
        return usage(argv[0]);
    }
    else if (argc == 2 and cmd == "test") {
        return test();
    }
    else if (argc == 2 and cmd == "srt_rx") {
        return srt_rx();
    }
    else {
        return usage(argv[0]);
    }
}

