/*
 * Copyright (C) 2003-2014 The Music Player Daemon Project
 * http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MPD_FFMPEG_TIME_HXX
#define MPD_FFMPEG_TIME_HXX

#include "Chrono.hxx"
#include "Compiler.h"

extern "C" {
#include <libavutil/avutil.h>
#include <libavutil/mathematics.h>
}

#include <assert.h>
#include <stdint.h>

/* suppress the ffmpeg compatibility macro */
#ifdef SampleFormat
#undef SampleFormat
#endif

/**
 * Convert a FFmpeg time stamp to a floating point value (in seconds).
 */
gcc_const
static inline double
FfmpegTimeToDouble(int64_t t, const AVRational time_base)
{
	assert(t != (int64_t)AV_NOPTS_VALUE);

	return (double)av_rescale_q(t, time_base, (AVRational){1, 1024})
		/ (double)1024;
}

/**
 * Convert a std::ratio to a #AVRational.
 */
template<typename Ratio>
static inline constexpr AVRational
RatioToAVRational()
{
	return { Ratio::num, Ratio::den };
}

/**
 * Convert a #SongTime to a FFmpeg time stamp with the given base.
 */
gcc_const
static inline int64_t
ToFfmpegTime(SongTime t, const AVRational time_base)
{
	return av_rescale_q(t.count(),
			    RatioToAVRational<SongTime::period>(),
			    time_base);
}

/**
 * Replace #AV_NOPTS_VALUE with the given fallback.
 */
static constexpr int64_t
FfmpegTimestampFallback(int64_t t, int64_t fallback)
{
	return gcc_likely(t != int64_t(AV_NOPTS_VALUE))
		? t
		: fallback;
}

#endif