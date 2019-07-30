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

using namespace std;

int main(int argc, char **argv)
{
    ClockTAI ct({"https://www.ietf.org/timezones/data/leap-seconds.list", "https://raw.githubusercontent.com/eggert/tz/master/leap-seconds.list"});
    etiLog.level(info) << "TAI offset = " << ct.get_offset();

    ReedSolomon rs(255, 207);

    RemoteControllerTelnet telnet(2121);
    etiLog.level(info) << "RC telnet started";

    rcs.enrol(&ct);

    CharsetConverter charset_converter;
    charset_converter.utf8_to_ebu("From UTF to EBU", false);

    EdiDecoder::STIWriter edi_stiwriter;
    EdiDecoder::STIDecoder edi_decoder(edi_stiwriter, true);

    etiLog.level(info) << "Sleep for 60s";
    this_thread::sleep_for(chrono::seconds(60));
}
