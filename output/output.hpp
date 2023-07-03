/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) 2020, Raspberry Pi (Trading) Ltd.
 *
 * output.hpp - video stream output base class
 */

#pragma once

#include <cstdio>

#include <atomic>

#include "core/metadata_handler.hpp"
#include "core/video_options.hpp"

class Output
{
public:
	static Output *Create(VideoOptions const *options);
	Output(VideoOptions const *options);
	virtual ~Output();
	virtual void Signal(); // a derived class might redefine what this means
	void OutputReady(void *mem, size_t size, int64_t timestamp_us, bool keyframe);
	void MetadataReady(libcamera::ControlList &metadata);

protected:
	enum Flag
	{
		FLAG_NONE = 0,
		FLAG_KEYFRAME = 1,
		FLAG_RESTART = 2
	};
	virtual void outputBuffer(void *mem, size_t size, int64_t timestamp_us, uint32_t flags);
	virtual void timestampReady(int64_t timestamp);
	virtual int getSegmentNum();
	VideoOptions const *options_;
	FILE *fp_timestamps_;

private:
	enum State
	{
		DISABLED = 0,
		WAITING_KEYFRAME = 1,
		RUNNING = 2
	};
	State state_;
	std::atomic<bool> enable_;
	int64_t time_offset_;
	int64_t last_timestamp_;
	int segment_num_;
	int previous_segment_num_;
	MetadataHandler metadata_handler_;
	bool metadata_startedd_;
};
