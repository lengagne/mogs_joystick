/*
 * Copyright (C) 2014  Clément Fouque <clement.fouque@univ-bpclermont.fr>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "JoystickMapper.h"
#include "ui_JoystickMapper.h"

#include "Joystick.h"

using namespace mogs;

/*!
 * Default constructor
 */
JoystickMapper::JoystickMapper() : 
    QMainWindow() ,
    js( new cJoystick )
{
    ui = new Ui::JoystickMapper;
    ui->setupUi(this);
}

/*!
 * Destructor
 */
JoystickMapper::~JoystickMapper()
{
    delete ui;
}

/*!
 * Sets ptr to the device
 */
void JoystickMapper::setJoystick( const QString device )
{    
    // Open device
    if ( js->init( device.toAscii() ) )
    {
        // Ok, start thread :)
        js->start() ;
        
        // Fancy title. :)
        QString name = QString::fromStdString( js->name ) ;
        QString wt = windowTitle() + " [ " + name + " ]" ;
        setWindowTitle(wt);
        
        // Grabing actions :
        ui->jsWrapper->setJoystick( js.toWeakRef() );
        
    } ;
    
    
    
    ui->actionView->resetUi( js->axes , js->buttons );
}
