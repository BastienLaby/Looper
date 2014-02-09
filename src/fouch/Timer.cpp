/*
 * Timer.cpp
 *
 *  Created on: Jan 31, 2014
 *      Author: florent
 */

#include "fouch/Timer.hpp"

#include <ctime>

namespace fouch {

__Timer__* Timer::timer = NULL;

Timer::Timer(const char* name){
	if(!timer)
		timer = new __Timer__(name);
}

Timer::~Timer(){
	delete(timer);
	timer = NULL;
}

__Timer__::__Timer__() {
	// TODO Auto-generated constructor stub
	mStartTime = SDL_GetTicks();
	mLastTime = mStartTime;
	mFrames = 0;
	mTotalFrames = 0;
	mFPS = 0;
	mLastBreakpoint = "";

}

__Timer__::__Timer__(const char* filename) {
	// TODO Auto-generated constructor stub
	mStartTime = SDL_GetTicks();
	mLastTime = mStartTime;
	mFrames = 0;
	mTotalFrames = 0;
	mFPS = 0;
	mLastBreakpoint = "";
	mFilename = filename;

}

__Timer__::~__Timer__() {
	// TODO Auto-generated destructor stub
	//stopTimer();
	dump();
}


long __Timer__::stopTimer(char* name){
	if( strcmp(name, "") == 0 && mLastBreakpoint == "" ){
		return 0;
	} else if(strcmp(name, "") == 0){
		name = (char*) mLastBreakpoint.c_str();
	}

	long res = SDL_GetTicks() - mBreakpoints[name].timer;
	mBreakpoints[name].time += res;
	++mBreakpoints[name].nbCall;
	if(mBreakpoints[name].minTime > res){
		mBreakpoints[name].minTime = res;
	}else if(mBreakpoints[name].minTime < res){
		mBreakpoints[name].maxTime = res;
	}
	mBreakpoints[name].timer = 0;
	return res;
}

long __Timer__::breakpoint(const char* name){

	long res = stopTimer();

	if(mBreakpoints.find(name) == mBreakpoints.end()){
		// TODO create new breakpoint
		__addBreakpoint(name);
	} else {
		mBreakpoints[name].timer = SDL_GetTicks();;
	}

	mLastBreakpoint = name;

	return res;
}


void __Timer__::__addBreakpoint(const char* name){

	Breakpoint bp;
	bp.time = 0;
	bp.minTime = 999999;
	bp.maxTime = 0;
	bp.timer = SDL_GetTicks();
	bp.nbCall = 0;

	mBreakpoints[name] = bp;

}


float __Timer__::fps(){
	++mFrames;
	++mTotalFrames;
	float spent = (float) SDL_GetTicks() - mLastTime;
	if (spent > 1000)
	{
		mFPS=1000.0f * (float) mFrames / spent;
		mFrames = 0;
		mLastTime=SDL_GetTicks();
	}
	return mFPS;
}



void __Timer__::reset(){
	mBreakpoints = std::map<std::string, Breakpoint>();
}

void __Timer__::dump(bool r){
	// TODO write report into the file if there is one or in the standard output
	long finalTime = SDL_GetTicks();
	long duration = (float)(finalTime - mStartTime) / 1000.f;
	std::ofstream output;
	if(mFilename != ""){
		output.open(mFilename.c_str(), std::ios::out | std::ios::app);
		std::cout.rdbuf(output.rdbuf());
	}

	time_t now = time(0);
	char* dt = ctime(&now);
	std::cout<<"Data gathered on "<<dt;
	std::cout<<"Average FPS : "<< (float)mTotalFrames / duration <<std::endl;
	std::cout<<"Duration : "<<duration<<" seconds"<<std::endl<<std::endl;
	std::cout<<"\t\tDetails :"<<std::endl<<std::endl;

	std::cout<<"#\tname\t\t\t%\t\ttime\t\tnb\tmin\tmax"<<std::endl;

	std::vector<std::pair<std::string, Breakpoint> > mapcopy(mBreakpoints.begin(), mBreakpoints.end());
	std::sort(mapcopy.begin(), mapcopy.end(), comparator<std::string, Breakpoint>());

	size_t i = 0;
	for(;i < mapcopy.size();++i) {
		std::cout<<"["<<i<<"]\t"<<mapcopy[i].first<<" :\t"<<std::fixed<<(float)mapcopy[i].second.time/(duration*10.f);
		std::cout<<"%\t"<<(float)(mapcopy[i].second.time)/1000.f<<"s\t"<<mapcopy[i].second.nbCall<<"\t"<<mapcopy[i].second.minTime<<"ms\t"<<mapcopy[i].second.maxTime<<"ms"<<std::endl;
	}
	output<<std::endl<<"---------------------"<<std::endl;

	if(mFilename != ""){
		output.close();
	}

	if(r)	reset();

}


} /* namespace fouch */
