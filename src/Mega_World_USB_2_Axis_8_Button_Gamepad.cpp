//      Mega_World_USB_2_Axis_8_Button_Gamepad.cpp
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

#include "Mega_World_USB_2_Axis_8_Button_Gamepad.h"
#include <stdlib.h>
#include <limits.h>

Mega_World_USB_2_Axis_8_Button_Gamepad::Mega_World_USB_2_Axis_8_Button_Gamepad (char * path)
{
	Joystick = new cJoystick(path);
	// set some assumption
	if ((int) Joystick->axes == 2 && (int) Joystick->buttons == 8)
		std::cout << " You are using the Mega World USB 2-Axis 8-Button Gamepad" <<std::endl;
	else
	  {
		  std::cerr << "Error in " << __FILE__ << " at line " << __LINE__ << ". This is not the data for the Mega World USB 2-Axis 8-Button Gamepad."  << std::endl;
		  exit (0);
	  }
}

Mega_World_USB_2_Axis_8_Button_Gamepad::~Mega_World_USB_2_Axis_8_Button_Gamepad ()
{
	if (Joystick)	delete Joystick;
}

/// use to stop the process
bool
Mega_World_USB_2_Axis_8_Button_Gamepad::get_stop ()
{
	return (Joystick->joystick_st->button[7]);
}

/// use to pause the process
bool
Mega_World_USB_2_Axis_8_Button_Gamepad::get_pause ()
{
	return (Joystick->joystick_st->button[6]);
}

/// get the forward velocity (positive if forward, negative if backward)
double
Mega_World_USB_2_Axis_8_Button_Gamepad::get_forward_velocity ()
{
	return (((double) -Joystick->joystick_st->axis[1]) / (SHRT_MAX));
}

/// get the side velocity (positive if one the right, negative if on the left)
double
Mega_World_USB_2_Axis_8_Button_Gamepad::get_side_velocity ()
{
	return (((double) Joystick->joystick_st->axis[0]) / (SHRT_MAX));
}

/// get the rotate velocity (positive if one the right, negative if on the left)
double
Mega_World_USB_2_Axis_8_Button_Gamepad::get_rotate_velocity ()
{
	if (Joystick->joystick_st->button[2])
		return 1.0;
	if (Joystick->joystick_st->button[1])
		return -1.0;

	return 0;
}

/// get the up velocity (positive if up, negative if down)
double
Mega_World_USB_2_Axis_8_Button_Gamepad::get_up_velocity ()
{
	if (Joystick->joystick_st->button[3])
		return 1.0;
	if (Joystick->joystick_st->button[0])
		return -1.0;

	return 0;
}
