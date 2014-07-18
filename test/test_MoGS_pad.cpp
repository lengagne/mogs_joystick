//      test_MoGS_pad.cpp
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
//      2012-2013:  IUT de Beziers/ LIRMM, Beziers, France
//	from 2013:  Université Blaise Pascal / axis : ISPR / theme MACCS

#include <sstream>
#include <fstream>

#include "Joystick.h"
#include "MoGS_Joystick.h"

int main (int argc, char *argv[])
{

	MoGS_Joystick test;

	while (!test.get_stop ())
	{
		  printf ("Forward: %.2f\t", test.get_forward_velocity ());
		  printf ("Side : %.2f\t", test.get_side_velocity ());
		  printf ("Rotate %.2f\t", test.get_rotate_velocity ());
		  printf ("Up %.2f\t", test.get_up_velocity ());
		  printf ("Pause %d\t", test.get_pause ());
		  printf ("Stop %d\t", test.get_stop ());
		  printf ("\r");
	}
	printf ("\nYou pushed on the stop button\n");

	return 0;
}
