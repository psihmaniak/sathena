// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_TRACE_HPP
#define SEAM_TRACE_HPP

// [SATHENA-SEAM helper] seam_trace — structured JSONL trace for seam call-sites and tests.
// Compiled to a no-op unless THENRO_TEST is defined (test builds only; prod pays nothing).
// Output: log/seam_trace.jsonl, one {"t":ms,"sub":...,"ev":...,"d":...} object per line —
// greppable with jq; the assert backend of the headless E2E harness (docs/testing.md).

#ifdef THENRO_TEST

#include <chrono>
#include <cstdarg>
#include <cstdio>

#ifdef WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

inline FILE* seam_trace_file(){
	// log/ may not exist on a fresh checkout — create it, else the trace silently vanishes
#ifdef WIN32
	_mkdir( "log" );
#else
	mkdir( "log", 0755 );
#endif
	return fopen( "log/seam_trace.jsonl", "a" );
}

inline void seam_trace( const char* subsys, const char* event, const char* fmt = "", ... ){
	static FILE* f = seam_trace_file();
	if( f == nullptr ){
		return;
	}

	char data[1024] = { 0 };

	if( fmt != nullptr && fmt[0] != '\0' ){
		va_list ap;
		va_start( ap, fmt );
		vsnprintf( data, sizeof( data ), fmt, ap );
		va_end( ap );
	}

	// minimal JSON escaping of the payload (quotes + backslashes)
	char escaped[2048];
	size_t j = 0;
	for( size_t i = 0; data[i] != '\0' && j < sizeof( escaped ) - 2; i++ ){
		if( data[i] == '"' || data[i] == '\\' ){
			escaped[j++] = '\\';
		}
		escaped[j++] = data[i];
	}
	escaped[j] = '\0';

	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch() ).count();

	fprintf( f, "{\"t\":%lld,\"sub\":\"%s\",\"ev\":\"%s\",\"d\":\"%s\"}\n",
		(long long)ms, subsys, event, escaped );
	fflush( f );
}

#else

inline void seam_trace( const char*, const char*, const char* = "", ... ){}

#endif /* THENRO_TEST */

#endif /* SEAM_TRACE_HPP */
