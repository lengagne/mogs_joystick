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

#include "JoystickVisualizer.h"
#include "ui_JoystickVisualizer.h"

#include "Joystick.h"
#include "JoystickConfigurationModel.h"
#include "delegate/AxisRangeDelegate.h"
#include "delegate/ButtonPressedDelegate.h"

#include <memory>
#include <climits>

#include <QVBoxLayout>
#include <QSlider>
#include <QPushButton>
#include <QLayout>
#include <QListView>

using namespace mogs ;

/*!
 * 
 */
JoystickVisualizer::JoystickVisualizer( QWidget * parent) :
    QWidget(parent) ,
    js(0)
{
    ui = new Ui::JoystickVisualizer ;
    ui->setupUi(this) ;
}

/*!
 * 
 */
JoystickVisualizer::~JoystickVisualizer()
{
    
}

/*!
 * 
 */
void JoystickVisualizer::setJoystick( JoystickConfigurationModel * js )
{   
    ui->axisView->setModel(js);
    ui->axisView->setRootIndex(js->axisIndex());
    ui->axisView->setModelColumn(1);
    ui->axisView->setItemDelegate( new AxisRangeDelegate(this) );
    
    ui->buttonView->setModel(js);
    ui->buttonView->setRootIndex(js->buttonIndex());
    ui->buttonView->setModelColumn(1);
    ui->buttonView->setItemDelegate( new ButtonPressedDelegate(this) );
}

/*!
 * 
 */
void JoystickVisualizer::resizeEvent( QResizeEvent * event )
{
    ui->buttonView->resize( ui->buttonView->contentsRect().size() );
}
