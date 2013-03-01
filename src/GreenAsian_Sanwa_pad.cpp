//     	GreenAsian_Sanwa_pad.cpp
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
//	This program was developped in the following labs:
//	from 2012: IUT de Beziers/ LIRMM, Beziers, France

#include "GreenAsian_Sanwa_pad.h"
#include <stdlib.h>
#include <limits.h>

GreenAsian_Sanwa_pad::GreenAsian_Sanwa_pad() 
{
	// set some assumption
	if ( (int)Joystick.axes == 2 && (int)Joystick.buttons ==12)
		std::cout<<" You are using the GreenAsian_Sanwa pad"<< std::endl;
	else
	{
		std::cerr<<"Error in "<< __FILE__<<" at line "<< __LINE__<<". This is not the data for the GreenAsian_Sanwa pad."<<std::endl;
		exit(0);
	}
}

GreenAsian_Sanwa_pad::~GreenAsian_Sanwa_pad() 
{

}

/// use to stop the process
bool GreenAsian_Sanwa_pad::get_stop()
{
	return ( ! Joystick.joystick_st->button[10]);
}

/// use to pause the process
bool GreenAsian_Sanwa_pad::get_pause()
{
	return ( Joystick.joystick_st->button[11]);
}

/// get the forward velocity (positive if forward, negative if backward)
double GreenAsian_Sanwa_pad::get_forward_velocity()
{
	return ( ((double) -Joystick.joystick_st->axis[1]) / ( SHRT_MAX));
}

/// get the side velocity (positive if one the right, negative if on the left)
double GreenAsian_Sanwa_pad::get_side_velocity()
{
	return ( ((double)Joystick.joystick_st->axis[0]) / ( SHRT_MAX) );
}

/// get the rotate velocity (positive if one the right, negative if on the left)
double GreenAsian_Sanwa_pad::get_rotate_velocity()
{
	if (Joystick.joystick_st->button[2])
		return 1.0;
	if (Joystick.joystick_st->button[1])
		return -1.0;
	
	return 0;
}

/// get the up velocity (positive if up, negative if down)
double GreenAsian_Sanwa_pad::get_up_velocity()
{
	if (Joystick.joystick_st->button[4])
		return 1.0;
	if (Joystick.joystick_st->button[5])
		return -1.0;
	
	return 0;
}