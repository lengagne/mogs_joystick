/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2014  ClÃ©ment Fouque <email>
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

#ifndef JOYSTICKWRAPPER_H
#define JOYSTICKWRAPPER_H

#include <QWidget>
#include <QMap>

#include <memory>

class cJoystick;
class QTimerEvent;
class QProgressBar ;
class QSlider ;
class QPushButton ;

namespace mogs 
{

class JoystickWrapper : public QWidget
{
    Q_OBJECT   
    
    QSharedPointer<cJoystick> js ;
    QMap<int,QSlider*> m_axisState ;
    QMap<int,QPushButton*> m_buttonsState ;
    
public:
    explicit JoystickWrapper( QWidget * parent = 0 );
    ~JoystickWrapper();
    
    void setJoystick( const QWeakPointer< cJoystick > & js ) ;

protected :
    void timerEvent( QTimerEvent * event ) ;
    void run() ;
    
};

} ;

#endif // JOYSTICKWRAPPER_H
