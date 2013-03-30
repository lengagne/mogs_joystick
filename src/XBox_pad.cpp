//      XBox_pad.cpp
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

#include "XBox_pad.h"

#include <stdlib.h>
#include <limits.h>

XBox_pad::XBox_pad ()
{
	// set some assumption
	if ((int) Joystick.axes == 8 && (int) Joystick.buttons == 11)
		std::cout << " You are using the XBOX pad" << std::endl;
	else
	  {
		  std::cout << " You are using the XBOX pad" << std::endl;
		  std::cerr << "Error in " << __FILE__ << " at line " <<
			  __LINE__ <<
			  ". This is not the data for the XBox pad." << std::
			  endl;
		  exit (0);
	  }
}

XBox_pad::~XBox_pad ()
{

}

/// use to stop the process
bool
XBox_pad::get_stop ()
{
	return (!Joystick.joystick_st->button[6]);
}

/// use to stop the process
bool
XBox_pad::get_pause ()
{
	return (Joystick.joystick_st->button[7]);
}

/// get the forward velocity (positive if forward, negative if backward)
double
XBox_pad::get_forward_velocity ()
{
	double tmp = -((double) Joystick.joystick_st->axis[1]) / SHRT_MAX;
	if (tmp < 0.2 && tmp > -0.2)
		tmp = 0.0;
	return (tmp);
}

/// get the side velocity (positive if one the right, negative if on the left)
double
XBox_pad::get_side_velocity ()
{
	double tmp = ((double) Joystick.joystick_st->axis[0]) / SHRT_MAX;
	if (tmp < 0.2 && tmp > -0.2)
		tmp = 0.0;
	return (tmp);
}

/// get the rotate velocity (positive if one the right, negative if on the left)
double
XBox_pad::get_rotate_velocity ()
{
	double tmp = ((double) Joystick.joystick_st->axis[3]) / SHRT_MAX;
	if (tmp < 0.2 && tmp > -0.2)
		tmp = 0.0;
	return (tmp);
}

/// get the up velocity (positive if up, negative if down)
double
XBox_pad::get_up_velocity ()
{
	double tmp = -((double) Joystick.joystick_st->axis[4]) / SHRT_MAX;
	if (tmp < 0.2 && tmp > -0.2)
		tmp = 0.0;
	return (tmp);
}
