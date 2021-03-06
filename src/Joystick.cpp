//      joystick.cpp
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

// https://www.kernel.org/doc/Documentation/input/joystick-api.txt

#include "Joystick.h"
#include <unistd.h>

/*!
 * 
 */
cJoystick::cJoystick ( )
{

}

/*!
 * 
 */
cJoystick::cJoystick (const char* joystick_dev, bool verbose)
{
    init( joystick_dev , verbose ) ;
}

cJoystick::~cJoystick ()
{
    if (joystick_fd > 0)
    {
        active = false;
        if ( thread > 0 ) 
            pthread_join (thread, 0);
        
        close (joystick_fd);
    }
    delete joystick_st;
    delete joystick_ev;
    joystick_fd = 0;
}

/*!
 * Initialize the datastructure associated with the joystick
 */
bool cJoystick::init ( const char * joystick_dev , bool verbose )
{
    active = false;
    thread = 0 ;
    joystick_fd = 0;
    joystick_ev = new js_event ();
    joystick_st = new Joystick_State ();
    joystick_fd = open (joystick_dev, O_RDONLY );
    
    if (joystick_fd > 0)
    {
        ioctl (joystick_fd, JSIOCGNAME (256), name);
        ioctl (joystick_fd, JSIOCGVERSION, &version);
        ioctl (joystick_fd, JSIOCGAXES, &axes);
        ioctl (joystick_fd, JSIOCGBUTTONS, &buttons);
// 		std::cout << "   Name: " << name << std::endl;
// 		std::cout << "Version: " << version << std::endl;
// 		std::cout << "   Axes: " << (int)axes << std::endl;
// 		std::cout << "Buttons: " << (int)buttons << std::endl;
        joystick_st->axis.resize(axes);
        joystick_st->button.resize(buttons);
        return true;
    }
    else
    {
        if (verbose)
        {
            std::cerr << "Error in " << __FILE__ << " at line " << __LINE__ << "." << std::endl;
            std::cerr << "There is no Joystick found on."<<joystick_dev << std::endl;
            exit (0);
        }
        return false;
    }
}

/*!
 * 
 */
void * cJoystick::loop( void * obj )
{
    while ( static_cast< cJoystick * >(obj)->active )
    {
        static_cast < cJoystick * >(obj)->readEv ();
    }

    return obj;
}

/*!
 * 
 */
void cJoystick::readEv ()
{
    FD_ZERO( &read_flags ) ;
    FD_SET( joystick_fd , &read_flags ); 
    struct timeval wait {0 , 100} ; 
    
    int selBytes = select( joystick_fd + 1 , & read_flags , (fd_set*)0 , (fd_set*)0 , &wait ) ;
    if ( selBytes < 0 ) return ; // Nothing to do here !

    if ( FD_ISSET( joystick_fd , &read_flags ) ) // Incoming !
    {
        FD_CLR( joystick_fd , &read_flags ) ; // Clear flags ;
        int bytes = read (joystick_fd, joystick_ev, sizeof (*joystick_ev));
        if (bytes > 0)
        {
            joystick_ev->type &= ~JS_EVENT_INIT;
            if (joystick_ev->type & JS_EVENT_BUTTON)
            {
                joystick_st->button[joystick_ev->number] = joystick_ev->value;
            }
            if (joystick_ev->type & JS_EVENT_AXIS)
            {
                joystick_st->axis[joystick_ev->number] = joystick_ev->value;
            }
        }
    }    
}

/*!
 * 
 */
bool cJoystick::buttonPressed (int n)
{
    return n > -1 && n < buttons ? joystick_st->button[n] : 0;
}

/*!
 * 
 */
void cJoystick::start() 
{
   if ( joystick_fd > 0 )
   {
       pthread_create( &thread  , 0, &cJoystick::loop , this ) ;
       active = true ;
   }
} ;
