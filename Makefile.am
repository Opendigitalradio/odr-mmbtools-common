# Copyright (C) 2019 Matthias P. Braendli, http://opendigitalradio.org

if IS_GIT_REPO
GITVERSION_FLAGS = -DGITVERSION="\"`git describe --dirty || echo 'unknown'`\""
else
GITVERSION_FLAGS =
endif

bin_PROGRAMS=common-test

INCLUDE=-Isrc

common_test_CFLAGS   =-Wall $(INCLUDE) $(GITVERSION_FLAGS)
common_test_CXXFLAGS =-Wall -DTAI_TEST $(INCLUDE) $(GITVERSION_FLAGS) $(ZMQ_CPPFLAGS)
common_test_LDADD    =$(ZMQ_LIBS) -lpthread
common_test_SOURCES  = main.cpp \
					   src/crc.h src/crc.c \
					   src/ClockTAI.h src/ClockTAI.cpp \
					   src/Globals.cpp \
					   src/Log.h src/Log.cpp \
					   src/ReedSolomon.h src/ReedSolomon.cpp \
					   src/RemoteControl.h src/RemoteControl.cpp \
					   src/Json.h src/Json.cpp \
					   src/Socket.h src/Socket.cpp \
					   src/ThreadsafeQueue.h \
					   src/charset/charset.h src/charset/charset.cpp \
					   src/charset/utf8.h \
					   src/charset/utf8/checked.h \
					   src/charset/utf8/core.h \
					   src/charset/utf8/unchecked.h \
					   src/edi/PFT.hpp src/edi/PFT.cpp \
					   src/edi/STIDecoder.hpp src/edi/STIDecoder.cpp \
					   src/edi/STIWriter.hpp src/edi/STIWriter.cpp \
					   src/edi/ETIDecoder.hpp src/edi/ETIDecoder.cpp \
					   src/edi/eti.hpp src/edi/eti.cpp \
					   src/edi/buffer_unpack.hpp \
					   src/edi/common.hpp src/edi/common.cpp \
					   src/edioutput/AFPacket.cpp \
					   src/edioutput/AFPacket.h \
					   src/edioutput/EDIConfig.h \
					   src/edioutput/PFT.cpp \
					   src/edioutput/PFT.h \
					   src/edioutput/TagItems.cpp \
					   src/edioutput/TagItems.h \
					   src/edioutput/TagPacket.cpp \
					   src/edioutput/TagPacket.h \
					   src/edioutput/Transport.cpp \
					   src/edioutput/Transport.h \
					   src/fec/char.h \
					   src/fec/decode_rs.h src/fec/decode_rs_char.c \
					   src/fec/encode_rs.h src/fec/encode_rs_char.c \
					   src/fec/fec.h \
					   src/fec/init_rs.h src/fec/init_rs_char.c \
					   src/fec/rs-common.h

EXTRA_DIST	= COPYING NEWS README.md ChangeLog \
			  lib/fec/README.md src/fec/LICENSE \
			  src/charset/README

