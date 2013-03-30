//      joystick.h
//      Copyright (C) 2012 lengagne (lengagne@gmail.com)
// 
//      This program is free software: you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation, either version 3 of the License, or
//      (at your option) any later version.
// 
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
// 
//      You should have received a copy of the GNU General Public License
//      along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//      This program was developped in the following labs:
//      from 2012: IUT de Beziers/ LIRMM, Beziers, France

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <iostream>
#include <fcntl.h>
#include <pthread.h>
#include <math.h>
#include <linux/joystick.h>
#include <vector>

#define JOYSTICK_DEV "/dev/input/js0"


struct joystick_state
{
	std::vector < signed short >button;
	  std::vector < signed short >axis;
};

class cJoystick
{
      public:
	pthread_t thread;
	bool active;
	int joystick_fd;
	js_event *joystick_ev;
	joystick_state *joystick_st;
	__u32 version;
	__u8 axes;
	__u8 buttons;
	char name[256];

      protected:
      public:
	  cJoystick ();
	 ~cJoystick ();
	static void *loop (void *obj);
	void readEv ();
	bool buttonPressed (int n);
};

#endif
