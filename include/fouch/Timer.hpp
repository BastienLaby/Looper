/*
 * Timer.hpp
 *
 *  Created on: Jan 31, 2014
 *      Author: florent
 */
#pragma once

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <SDL/SDL.h>
#include <algorithm>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>

namespace fouch {

struct Breakpoint {
	long time;
	long minTime;
	long maxTime;
	unsigned int nbCall;
	long timer;
};


class Timer {
protected:
	long mStartTime;
	long mLastTime;
	int mFrames;
	int mTotalFrames;
	float mFPS;
	std::string mFilename;

	std::string mLastBreakpoint;

	std::map<std::string, Breakpoint> mBreakpoints;

	void __addBreakpoint(const char* name);

	template <typename T1, typename T2>
	struct comparator {
	    typedef std::pair<T1, T2> type;
	    bool operator ()(type const& A, type const& B) const {
	    	return A.second.time > B.second.time;
	    }
	};


public:
	Timer();
	Timer(const char* filename);

	virtual ~Timer();

	long breakpoint(const char* name);
	long stopTimer(char* name = const_cast<char *>("") );

	float fps();

	void dump();
	void reset();

};

} /* namespace fouch */
#endif /* TIMER_HPP_ */
