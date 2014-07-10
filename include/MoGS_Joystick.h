//      MoGS_Joystick.h
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
//      2012-2013:  IUT de Beziers/ LIRMM, Beziers, France
//	from 2013:  Université Blaise Pascal / axis : ISPR / theme MACCS

#ifndef __MOGS_JOYSTICK_H__
#define __MOGS_JOYSTICK_H__

#include "Joystick.h"

enum	push_type{
	AXIS = true,
	BUTTON = false,
};

struct	type{
	push_type push;
	unsigned int id;
	unsigned int id_neg;
	double sign;
};

struct	pad_control{
	std::string name;
	type stop_button;
	type pause_button;
	type forward;
	type side;
	type up;
	type rotate;
};

class MoGS_Joystick
{
      public:
	MoGS_Joystick ();

	~MoGS_Joystick ();

	/// use to stop the process
	bool get_stop ();

	/// use to pause the process
	bool get_pause ();

	/// get the forward velocity (positive if forward, negative if backward)
	double get_forward_velocity ();

	/// get the side velocity (positive if one the right, negative if on the left)
	double get_side_velocity ();

	/// get the rotate velocity (positive if one the right, negative if on the left)
	double get_rotate_velocity ();

	/// get the up velocity (positive if up, negative if down)
	double get_up_velocity ();

      protected:
      private:
	      
	double get_double( const type & in) const;

	pad_control config_;
	cJoystick * Joystick_;
};

#endif
