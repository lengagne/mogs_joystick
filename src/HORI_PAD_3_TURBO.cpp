//      HORI_PAD_3_TURBO.cpp
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

#include "HORI_PAD_3_TURBO.h"
#include <stdlib.h>
#include <limits.h>

HORI_PAD_3_TURBO::HORI_PAD_3_TURBO (char * path)
{
	Joystick = new cJoystick(path);
	// set some assumption
	if ((int) Joystick->axes == 6 && (int) Joystick->buttons == 13)
		std::cout << " You are using the HORI_PAD_3_TURBO" <<std::endl;
	else
	  {
		  std::cerr << "Error in " << __FILE__ << " at line " << __LINE__ << ". This is not the data for the HORI_PAD_3_TURBO."  << std::endl;
		  exit (0);
	  }
}

HORI_PAD_3_TURBO::~HORI_PAD_3_TURBO ()
{
	if (Joystick)	delete Joystick;
}

/// use to stop the process
bool
HORI_PAD_3_TURBO::get_stop ()
{
	return (Joystick->joystick_st->button[8]);
}

/// use to pause the process
bool
HORI_PAD_3_TURBO::get_pause ()
{
	return (Joystick->joystick_st->button[9]);
}

/// get the forward velocity (positive if forward, negative if backward)
double
HORI_PAD_3_TURBO::get_forward_velocity ()
{
	return (((double) -Joystick->joystick_st->axis[1]) / (SHRT_MAX));
}

/// get the side velocity (positive if one the right, negative if on the left)
double
HORI_PAD_3_TURBO::get_side_velocity ()
{
	return (-((double) Joystick->joystick_st->axis[0]) / (SHRT_MAX));
}

/// get the rotate velocity (positive if one the right, negative if on the left)
double
HORI_PAD_3_TURBO::get_rotate_velocity ()
{
	return (-((double) Joystick->joystick_st->axis[2]) / (SHRT_MAX));
}

/// get the up velocity (positive if up, negative if down)
double
HORI_PAD_3_TURBO::get_up_velocity ()
{
	return (-((double) Joystick->joystick_st->axis[3]) / (SHRT_MAX));
}
