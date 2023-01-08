//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  TimeStep.h
//
//   Description:  时间
//
//       Version:  1.0
//       Created:  2023年1月2日 13时16分46秒
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================

#pragma once

namespace XQuant {

	class TimeStep {
	public:
		TimeStep(float time = 0.0f)
			:_time(time)
		{

		}

		operator float() const { return _time; }
		float getSeconds() const { return _time; }
		float getMilliseconds() const { return _time * 1000.0f; }
	
	private:
		float _time;
	};
}


