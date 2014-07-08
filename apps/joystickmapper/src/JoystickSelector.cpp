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

#include "JoystickSelector.h"
#include "ui_JoystickSelector.h"

#include "Joystick.h"
#include "MoGS_Config_Joystick.h"

#include <memory>
#include <QDataWidgetMapper>
#include <QStandardItemModel>
#include <QDebug>

using namespace mogs;

/*!
 * 
 */
JoystickSelector::JoystickSelector( QWidget * parent ) :
    QDialog( parent ) ,
    m_avJs(0)
{
    ui = new Ui::JoystickSelector;
    ui->setupUi(this);
}

/*!
 * 
 */
JoystickSelector::~JoystickSelector()
{
    delete ui;
}

/*!
 * 
 */
QString JoystickSelector::selectedPort() const
{
    return ui->portSelect->currentText() ;
}

/*!
 * Lists all available devices :
 */
void JoystickSelector::findValidJoysticks()
{
    initializeModel() ;
    
    MoGS_Config_Joystick configFinder ;
    bool valid = true ;
    int cpt = 0;
    char tmp_path[20];
    
    do
    {
        sprintf(tmp_path,"/dev/input/js%d",cpt) ;
        std::unique_ptr<cJoystick> js( new cJoystick ) ;
        
        valid = js->init(tmp_path) ;
        if ( valid )
        {
            std::string name = js->name;
            // check compatibility
            if ( (int) js->axes + (int) js->buttons >= 8 )
            {
                bool hasConfig = configFinder.has_pad(name) ;
                QList<QStandardItem*> row ;
                row << new QStandardItem( tmp_path ) ;
                row << new QStandardItem( QString::fromStdString(name) ) ;
                row << new QStandardItem( QString::number(js->axes) ) ;
                row << new QStandardItem( QString::number(js->buttons) ) ;
                row << new QStandardItem( hasConfig ) ;
                if ( hasConfig )
                    row.at(0)->setIcon( style()->standardIcon(QStyle::SP_DialogApplyButton) );
                qDebug() << tmp_path << ":" << QString::fromStdString(name) ;
                m_avJs->appendRow(row);
            }
        }   
        cpt++ ;
    }
    while( valid ) ;
}

/*!
 * Configure the data model as follow :
 * 
 * |  0        | 1         | 2       | 3       | 4           |
 * |-----------|-----------|---------|---------|-------------|
 * | string    | string  | int     | int        | bool       |
 * | port_name | js_name | nb_axis | nb_buttons | has_config |
 * 
 */
void JoystickSelector::initializeModel()
{
    if( !m_avJs )
        m_avJs = new QStandardItemModel( 0 , 5 , this ) ;
    else 
        m_avJs->clear() ;
    
    ui->portSelect->setModel( m_avJs );
    ui->portSelect->setModelColumn( 0 );
    
    QDataWidgetMapper * mapper = findChild<QDataWidgetMapper*>() ;
    if  ( !mapper )
    {
        mapper = new QDataWidgetMapper(this) ;
        mapper->setModel( m_avJs );
        mapper->setSubmitPolicy( QDataWidgetMapper::AutoSubmit );
        mapper->addMapping( ui->nameLineEdit , 1 );
        mapper->addMapping( ui->axisLineEdit , 2 ) ;
        mapper->addMapping( ui->buttonsLineEdit , 3 ) ;
        connect( ui->portSelect , SIGNAL(currentIndexChanged(int)) , mapper , SLOT(setCurrentIndex(int)) );
    }
}

/*!
 * return number of available joysticks. 
 * Return -1 if never initialized.
 */
int JoystickSelector::jsCount() const
{
    return (!m_avJs)? -1 : m_avJs->rowCount() ;
}

