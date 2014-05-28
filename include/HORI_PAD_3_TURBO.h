//      HORI_PAD_3_TURBO.h
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
//      Universite Blaise Pascal Clermont Ferrand 2


#ifndef __HORI_PAD_3_TURBO_H__
#define __HORI_PAD_3_TURBO_H__

#include <string.h>
#include "Generic_Joystick.h"

class HORI_PAD_3_TURBO:public Generic_Joystick
{
      public:
	HORI_PAD_3_TURBO (char * path);

	~HORI_PAD_3_TURBO ();

	/// use to stop the process
	virtual bool get_stop ();

	/// use to stop the process
	virtual bool get_pause ();

	/// get the forward velocity (positive if forward, negative if backward)
	virtual double get_forward_velocity ();

	/// get the side velocity (positive if one the right, negative if on the left)
	virtual double get_side_velocity ();

	/// get the rotate velocity (positive if one the right, negative if on the left)
	virtual double get_rotate_velocity ();

	/// get the up velocity (positive if up, negative if down)
	virtual double get_up_velocity ();

};

#endif
