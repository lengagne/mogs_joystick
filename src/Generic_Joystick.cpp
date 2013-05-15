//      Generic_Joystick.cpp
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

#include "Generic_Joystick.h"
#include <stdlib.h>
#include <limits.h>

Generic_Joystick::Generic_Joystick ()
{
	// set some assumption
	if ((int) Joystick.axes >= 2 && (int) Joystick.buttons >= 6)
		std::cout << " You can use this pad as a generic device " <<
			std::endl;
	else
	  {
		  std::cerr <<
			  " You cannot use this pad, the generic device requires 2 axes and 6 buttons"
			  << std::endl;
		  exit (0);
	  }
}

Generic_Joystick::~Generic_Joystick ()
{

}

/// use to stop the process
bool
Generic_Joystick::get_stop ()
{
	return (Joystick.joystick_st->button[0]);
}

/// use to pause the process
bool
Generic_Joystick::get_pause ()
{
	return (Joystick.joystick_st->button[3]);
}

/// get the forward velocity (positive if forward, negative if backward)
double
Generic_Joystick::get_forward_velocity ()
{
	return (((double) -Joystick.joystick_st->axis[1]) / (SHRT_MAX));
}

/// get the side velocity (positive if one the right, negative if on the left)
double
Generic_Joystick::get_side_velocity ()
{
	return (((double) Joystick.joystick_st->axis[0]) / (SHRT_MAX));
}

/// get the rotate velocity (positive if one the right, negative if on the left)
double
Generic_Joystick::get_rotate_velocity ()
{
	if (Joystick.joystick_st->button[1])
		return -1;
	if (Joystick.joystick_st->button[2])
		return 1;
	return 0;
}

/// get the up velocity (positive if up, negative if down)
double
Generic_Joystick::get_up_velocity ()
{
	if (Joystick.joystick_st->button[4])
		return 1;
	if (Joystick.joystick_st->button[5])
		return -1;
	return 0;
}
