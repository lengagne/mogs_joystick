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

#ifndef __JOYSTICKVISUALIZER_H__
#define __JOYSTICKVISUALIZER_H__

#include <QWidget>
#include <QMap>

#include <memory>

class cJoystick;

class QTimerEvent;
class QProgressBar ;
class QSlider ;
class QPushButton ;

namespace Ui
{
  class JoystickVisualizer ;
}

namespace mogs 
{
class JoystickWrapper ;

class JoystickVisualizer : public QWidget
{
    Q_OBJECT   
    
    Ui::JoystickVisualizer * ui ;
    
    QSharedPointer<cJoystick> js ;
    QMap<int,QSlider *> m_axisState ;
    QMap<int,QPushButton *> m_buttonsState ;
    
public:
    explicit JoystickVisualizer( QWidget * parent = 0 );
    virtual ~JoystickVisualizer();
    
    void setJoystick( JoystickWrapper * js ) ;

protected :
    virtual void resizeEvent(QResizeEvent * );
    
};

} ;

#endif // JOYSTICKWRAPPER_H
