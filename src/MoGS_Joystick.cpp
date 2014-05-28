//      MoGS_Joystick.cpp
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

#include "MoGS_Joystick.h"
#include "GreenAsian_Sanwa_pad.h"
#include "Mega_World_USB_2_Axis_8_Button_Gamepad.h"
#include "XBox_pad.h"
#include "HORI_PAD_3_TURBO.h"

#include <string.h>
#include <stdlib.h>
#include <iostream>

MoGS_Joystick::MoGS_Joystick ()
{
	bool undefined_joystick = true;
	bool next_joystick = true;
	int cpt = 0;
	while ( undefined_joystick && next_joystick)
	{
		char tmp_path[20];
		sprintf(tmp_path,"/dev/input/js%d",cpt);
		if (open (tmp_path, O_RDONLY | O_NONBLOCK))
		{		
			Joystick = new Generic_Joystick (tmp_path);
			std::string name = Joystick->get_name();
			if (name.compare ("GreenAsia Sanwa Supply 12 button game pad") == 0)
			{
				delete Joystick;
				Joystick = new GreenAsian_Sanwa_pad (tmp_path);
				undefined_joystick = false;
			}
			else if (name.compare ("Microsoft X-Box 360 pad") == 0)
			{
				delete Joystick;
				Joystick = new XBox_pad (tmp_path);
				undefined_joystick = false;
			}
			else if (name.compare ("Mega World USB 2-Axis 8-Button Gamepad") == 0)
			{
				delete Joystick;
				Joystick = new Mega_World_USB_2_Axis_8_Button_Gamepad(tmp_path);
				undefined_joystick = false;	
			}
			else if (name.compare ("HORO CO.,LTD. HORI PAD 3 TURBO") == 0)
			{
				delete Joystick;
				Joystick = new HORI_PAD_3_TURBO(tmp_path);
				undefined_joystick = false;
			}else if (Joystick->check())	// check if the pas has the good buttons
			{
// 				delete Joystick;
// 				Joystick = new Generic_Joystick();
				undefined_joystick = false;
			}
		}
		else{
			next_joystick = false;
		}		
		cpt++;
	}	
	if (undefined_joystick)
	{
		std::cerr <<"There is no known pad, so we use the generic configuration pad"<< std::endl;
	}
	Joystick->check();
}

MoGS_Joystick::~MoGS_Joystick ()
{
	delete Joystick;
}

/// use to stop the process
bool
MoGS_Joystick::get_stop ()
{
	return (Joystick->get_stop ());
}

/// use to pause the process
bool
MoGS_Joystick::get_pause ()
{
	return (Joystick->get_pause ());
}

/// get the forward velocity (positive if forward, negative if backward)
double
MoGS_Joystick::get_forward_velocity ()
{
	return (Joystick->get_forward_velocity ());
}

/// get the side velocity (positive if one the right, negative if on the left)
double
MoGS_Joystick::get_side_velocity ()
{
	return (Joystick->get_side_velocity ());
}

/// get the rotate velocity (positive if one the right, negative if on the left)
double
MoGS_Joystick::get_rotate_velocity ()
{
	return Joystick->get_rotate_velocity ();
}

/// get the up velocity (positive if up, negative if down)
double
MoGS_Joystick::get_up_velocity ()
{
	return Joystick->get_up_velocity ();
}
