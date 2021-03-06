# ################################################################
# zstd - Makefile
# Copyright (C) Yann Collet 2014-2016
# All rights reserved.
#
# BSD license
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice, this
#   list of conditions and the following disclaimer in the documentation and/or
#   other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# You can contact the author at :
#  - zstd homepage : http://www.zstd.net/
# ################################################################

# Get library version based on information from input content (use regular exp)
function(GetLibraryVersion _content _outputVar1 _outputVar2 _outputVar3)
    string(REGEX MATCHALL ".*define ZSTD_VERSION_MAJOR+.* ([0-9]+).*define ZSTD_VERSION_MINOR+.* ([0-9]+).*define ZSTD_VERSION_RELEASE+.* ([0-9]+)" VERSION_REGEX "${_content}")
    SET(${_outputVar1} ${CMAKE_MATCH_1} PARENT_SCOPE)
    SET(${_outputVar2} ${CMAKE_MATCH_2} PARENT_SCOPE)
    SET(${_outputVar3} ${CMAKE_MATCH_3} PARENT_SCOPE)
endfunction()

# Define library directory, where sources and header files are located
SET(LIBRARY_DIR include/zstd)
INCLUDE_DIRECTORIES(${LIBRARY_DIR} ${LIBRARY_DIR}/common)

# Read file content
FILE(READ ${LIBRARY_DIR}/zstd.h HEADER_CONTENT)

# Parse version
GetLibraryVersion("${HEADER_CONTENT}" LIBVER_MAJOR LIBVER_MINOR LIBVER_RELEASE)
MESSAGE("ZSTD VERSION ${LIBVER_MAJOR}.${LIBVER_MINOR}.${LIBVER_RELEASE}")

SET(Sources
        ${LIBRARY_DIR}/common/entropy_common.c
        ${LIBRARY_DIR}/common/zstd_common.c
        ${LIBRARY_DIR}/common/xxhash.c
        ${LIBRARY_DIR}/common/fse_decompress.c
        ${LIBRARY_DIR}/compress/fse_compress.c
        ${LIBRARY_DIR}/compress/huf_compress.c
        ${LIBRARY_DIR}/compress/zbuff_compress.c
        ${LIBRARY_DIR}/compress/zstd_compress.c
        ${LIBRARY_DIR}/decompress/huf_decompress.c
        ${LIBRARY_DIR}/decompress/zbuff_decompress.c
        ${LIBRARY_DIR}/decompress/zstd_decompress.c
        ${LIBRARY_DIR}/dictBuilder/divsufsort.c
        ${LIBRARY_DIR}/dictBuilder/zdict.c)

SET(Headers
        ${LIBRARY_DIR}/common/bitstream.h
        ${LIBRARY_DIR}/common/error_private.h
        ${LIBRARY_DIR}/common/error_public.h
        ${LIBRARY_DIR}/common/fse.h
        ${LIBRARY_DIR}/common/huf.h
        ${LIBRARY_DIR}/common/mem.h
        ${LIBRARY_DIR}/common/zbuff.h
        ${LIBRARY_DIR}/common/zstd_internal.h
        ${LIBRARY_DIR}/zstd.h
        ${LIBRARY_DIR}/dictBuilder/zdict.h)

SET(ZSTD_LEGACY_SUPPORT true)

IF (ZSTD_LEGACY_SUPPORT)
    SET(LIBRARY_LEGACY_DIR ${LIBRARY_DIR}/legacy)
    INCLUDE_DIRECTORIES(${LIBRARY_LEGACY_DIR})
    ADD_DEFINITIONS(-D ZSTD_LEGACY_SUPPORT=1)

    SET(Sources ${Sources}
            ${LIBRARY_LEGACY_DIR}/zstd_v06.c)

    SET(Headers ${Headers}
            ${LIBRARY_LEGACY_DIR}/zstd_legacy.h
            ${LIBRARY_LEGACY_DIR}/zstd_v06.h)
ENDIF (ZSTD_LEGACY_SUPPORT)

ADD_LIBRARY(zstd ${Sources} ${Headers})
