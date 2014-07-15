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
#include "JoystickWrapper.h"
#include "delegate/AxisRangeDelegate.h"
#include "delegate/ButtonPressedDelegate.h"

#include <climits>

#include <QVBoxLayout>
#include <QSlider>
#include <QPushButton>
#include <QLayout>
#include <QListView>
#include <QResizeEvent>
#include <QDebug>

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
    
    connect( ui->axisView , SIGNAL(clicked(QModelIndex)) , SIGNAL(clicked(QModelIndex)) ) ;
    connect( ui->buttonView , SIGNAL(clicked(QModelIndex)) , SIGNAL(clicked(QModelIndex)) ) ;
}

/*!
 * 
 */
JoystickVisualizer::~JoystickVisualizer()
{
    delete ui ;
}

/*!
 * 
 */
void JoystickVisualizer::setJoystick( JoystickWrapper * js )
{   
    // Axis viewport
    ui->axisView->setModel(js);
    ui->axisView->setRootIndex(js->axisIndex());
    ui->axisView->setModelColumn(1);
    ui->axisView->setItemDelegate( new AxisRangeDelegate(this) );
    ui->axisView->viewport()->setAutoFillBackground(false);
    
    // Button viewport 
    ui->buttonView->setModel(js);
    ui->buttonView->setRootIndex(js->buttonIndex());
    ui->buttonView->setModelColumn(1);
    ui->buttonView->setItemDelegate( new ButtonPressedDelegate(this) );
    ui->buttonView->viewport()->setAutoFillBackground(false);
    
}

/*!
 * 
 */
void JoystickVisualizer::resizeEvent( QResizeEvent * event )
{
    QWidget::resizeEvent(event);
    setAxisViewGeometry() ;
    setButtonViewGeometry() ;
}

/*!
 * 
 */
void JoystickVisualizer::setAxisViewGeometry()
{
    QSize size = ui->axisView->itemDelegate()->sizeHint( QStyleOptionViewItem() , QModelIndex() ) ;
    const QModelIndex rootIndex = ui->axisView->rootIndex() ;
    const int r = ui->axisView->model()->rowCount( rootIndex ) ;
    
    int h = r * ( size.height() + 2 * ui->axisView->spacing() );
    ui->axisView->setFixedHeight(h);
}

/*!
 * 
 */
void JoystickVisualizer::setButtonViewGeometry()
{
    QSize vSize = ui->buttonView->itemDelegate()->sizeHint( QStyleOptionViewItem() , QModelIndex() ) ;
    const QModelIndex rootIndex = ui->buttonView->rootIndex() ;
    const int n = ui->buttonView->model()->rowCount( rootIndex ) ;
    
    int nl = ui->buttonView->viewport()->width() / ( vSize.width() + 2 * ui->buttonView->spacing() ) ;
    int nc = n / nl + 1 ;
    int h = nc * ( vSize.height() + 2 * ui->buttonView->spacing() ) ;
    ui->buttonView->setFixedHeight(h);
}
