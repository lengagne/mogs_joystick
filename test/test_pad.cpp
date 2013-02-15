//     	test_pad.cpp
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

#include <SDL/SDL.h>
#include <sstream>
#include <fstream>

#include "joystick.h"
#include "Generic_Joystick.h"


int main(int argc, char *argv[]) {

	cJoystick js; 
	int nb_axis = (int)js.axes;
	int nb_bouton = (int)js.buttons;

	printf("Number of Axis = %d\n", (int)js.axes);
	printf("Number of Buttons = %d\n", (int)js.buttons);
	
	printf("Axis Values --- Buttons values\n");
	while(true) {


		for ( int i=0; i<nb_axis; i++) 
			printf ("%.2f\t", js.joystick_st->axis[i] ) ;
		printf("---");
		for ( int i=0; i<nb_bouton; i++) 
			printf ("%d\t", js.joystick_st->button[i] ) ;		

	printf("\r");

	}

	return 0;
}
