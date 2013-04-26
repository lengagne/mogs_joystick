//      joystick.cpp
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

#include "joystick.h"
#include <unistd.h>

cJoystick::cJoystick ()
{
	active = false;
	joystick_fd = 0;
	joystick_ev = new js_event ();
	joystick_st = new joystick_state ();
	joystick_fd = open (JOYSTICK_DEV, O_RDONLY | O_NONBLOCK);
	if (joystick_fd > 0)
	  {
		  ioctl (joystick_fd, JSIOCGNAME (256), name);
		  ioctl (joystick_fd, JSIOCGVERSION, &version);
		  ioctl (joystick_fd, JSIOCGAXES, &axes);
		  ioctl (joystick_fd, JSIOCGBUTTONS, &buttons);
//              std::cout << "   Name: " << name << std::endl;
//              std::cout << "Version: " << version << std::endl;
//              std::cout << "   Axes: " << (int)axes << std::endl;
//              std::cout << "Buttons: " << (int)buttons << std::endl;
		  joystick_st->axis.reserve (axes);
		  joystick_st->button.reserve (buttons);
		  active = true;
		  pthread_create (&thread, 0, &cJoystick::loop, this);
	  }
	else
	  {
		  std::cerr << "Error in " << __FILE__ << " at line " << __LINE__ << "." << std::endl;
		  std::cerr << "There is no Joystick found." << std::endl;
		  exit (0);
	  }
}

cJoystick::~cJoystick ()
{
	if (joystick_fd > 0)
	  {
		  active = false;
		  pthread_join (thread, 0);
		  close (joystick_fd);
	  }
	delete joystick_st;
	delete joystick_ev;
	joystick_fd = 0;
}

void *cJoystick::loop (void *obj)
{
	while (reinterpret_cast < cJoystick * >(obj)->active)
		reinterpret_cast < cJoystick * >(obj)->readEv ();
	
	return obj;
}

void
cJoystick::readEv ()
{
	int bytes = read (joystick_fd, joystick_ev, sizeof (*joystick_ev));
	if (bytes > 0)
	  {
		  joystick_ev->type &= ~JS_EVENT_INIT;
		  if (joystick_ev->type & JS_EVENT_BUTTON)
		    {
			    joystick_st->button[joystick_ev->number] =
				    joystick_ev->value;
		    }
		  if (joystick_ev->type & JS_EVENT_AXIS)
		    {
			    joystick_st->axis[joystick_ev->number] =
				    joystick_ev->value;
		    }
	  }
}

bool
cJoystick::buttonPressed (int n)
{
	return n > -1 && n < buttons ? joystick_st->button[n] : 0;
}
