//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  TimeStep.h
//
//   Description:  ʱ��
//
//       Version:  1.0
//       Created:  2023��1��2�� 13ʱ16��46��
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


