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

template <typename T1, typename T2>
struct comparator {
    typedef std::pair<T1, T2> type;
    virtual bool operator ()(type const& A, type const& B) const {
    	return A.second.time > B.second.time;
    }

};


///////////////////////////////////////////////////////////
//|						__TIMER__						|//
///////////////////////////////////////////////////////////
/**
 * Do NOT use this class if you are not totally confident in what you are doing
 * User the singleton class Timer (see below)
 */
class __Timer__ {
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


public:
	__Timer__();
	__Timer__(const char* filename);

	virtual ~__Timer__();

	long breakpoint(const char* name);
	long stopTimer(char* name = const_cast<char *>("") );

	inline void setSaveFile(const char* name) { mFilename = name; };

	float fps();

	void dump(bool r = false);
	void reset();

};



///////////////////////////////////////////////////////////
//|							TIMER						|//
///////////////////////////////////////////////////////////
/**
 * A debugger pretty easy to use
 * Instantiate a Timer, add breakpoints and give them a name
 * use the function fps() once in your main loop to get the fps infos
 * It will save datas from breakpoints
 * and extract, sort and display those datas on the timer destruction
 */
class Timer {
private:

	/** Singleton variable, do not use the __Timer__ class by yourself */
	static __Timer__* timer;


public:

	/**
	 * Singleton constructor, instantiate the private member
	 *
	 * @param const char* name : the path file if you desire to save your data into a file.
	 */
	Timer(const char* name = const_cast<char *>(""));

	/**
	 *	Destructor, it will call __Timer__::dump() to display the data
	 *	on the chosen output (file or standard)
	 *	This function will delete the current __Timer__
	 */
	virtual ~Timer();




	/**
	 * Will create or update the breakpoint going by the name passed in arg
	 * Use a understandable name since it will be used for the dump() display.
	 *
	 * @param const char* name : The name of the breakpoint and the key for the map used to store breakpoints.
	 */
	inline long breakpoint(const char* name) {
		if(strcmp(name, "") == 0)		return 0;
		return timer->breakpoint(name);
	};

	/**
	 * Stop The current Timer since only one timer can be launched at a time for now
	 * TODO allow multiple run of breakpoint if asked in the constructor
	 *
	 * @return the time since last breakpoint call in ms
	 */
	inline long stopTimer(char* name = const_cast<char *>("") ) { return timer->stopTimer(name); };

	/**
	 * If called once every loop will calculate fps every second.
	 *
	 * @return average fps from the last second
	 */
	inline float fps() { return timer->fps(); };



	/**
	 * Display all the data stored inside the singleton __Timer__
	 *
	 * @param bool reset, will reset all the breakpoints data stored inside the singleton
	 */
	inline void dump(bool reset = false) { timer->dump(reset); };

	/**
	 * Reset all the breakpoints data stored inside the singleton
	 */
	inline void reset() { timer->reset(); };
};



} /* namespace fouch */
#endif /* TIMER_HPP_ */
