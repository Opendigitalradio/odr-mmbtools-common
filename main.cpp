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

using namespace std;

int main(int argc, char **argv)
{
    ClockTAI ct({"https://127.0.0.1", "https://example.com", "https://raw.githubusercontent.com/eggert/tz/master/leap-seconds.lis"});
    etiLog.level(info) << "TAI offset = " << ct.get_offset();

    ReedSolomon rs(255, 207);

    RemoteControllerTelnet telnet(2121);
    etiLog.level(info) << "RC telnet started";

    rcs.enrol(&ct);

    CharsetConverter charset_converter;
    charset_converter.utf8_to_ebu("From UTF to EBU", false);

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
