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

#include "JoystickConfigurationModel.h"
#include "Joystick.h"
#include <vector>
#include <QDebug>

struct jsItem 
{
    QList<jsItem*> childs ;
    std::vector<int16_t>::iterator it ;
    jsItem * parent ;
    int row ;
    int action ;
} ;


/*!
 * Constructor
 */
JoystickConfigurationModel::JoystickConfigurationModel(QObject* parent) :
    QAbstractItemModel(parent) ,
    axisItem(0) ,
    buttonItem(0)
{
    
}

/*!
 * 
 */
JoystickConfigurationModel::~JoystickConfigurationModel()
{
    
}

/*!
 *
 */
bool JoystickConfigurationModel::openDevice( const QString & device )
{
    cJoystick * js_ = new cJoystick ;
        
    const bool ok = js_->init(device.toAscii()) ;
    if ( ok ) 
    {
        emit layoutAboutToBeChanged() ;
        js = QSharedPointer<cJoystick>(js_) ;
        
        // Top items
        beginInsertRows( QModelIndex() , 0 , 1 );
       
        axisItem = new jsItem ;
        axisItem->parent = 0 ;
        axisItem->row = 0 ;
        axisItem->action = NoAxisAction ;
        
        buttonItem = new jsItem ;
        buttonItem->parent = 0 ;
        buttonItem->row = 0 ;
        buttonItem->action = NoButtonAction ;
        endInsertRows();
        emit dataChanged( axisIndex() , buttonIndex() );
        
        
        // Axes items 
        beginInsertRows( axisIndex() , 0 , js->axes );
        beginInsertColumns( axisIndex() , 0 , 2 );
        int r = 0 ;
        for ( std::vector<int16_t>::iterator i = js->joystick_st->axis.begin() ; i != js->joystick_st->axis.end() ; i++ )
        {
            jsItem * a = new jsItem ;
            a->parent = axisItem ;
            a->row = r++ ;
            a->it = i ;
            a->action = NoAxisAction ;
            axisItem->childs.push_back(a);
        }
        endInsertColumns();
        endInsertRows() ;
        emit dataChanged( index(0,0,axisIndex()) , index(js->axes,2,axisIndex()) );
        
        // button
        beginInsertRows( buttonIndex() , 0 , js->buttons );
        beginInsertColumns( buttonIndex() , 0 , 2 );
        r = 0 ;
        for ( std::vector<int16_t>::iterator i = js->joystick_st->button.begin() ; i != js->joystick_st->button.end() ; i++ )
        {
            qDebug() << "loading row :" << r ;
            jsItem * b = new jsItem ;
            b->parent = buttonItem ;
            b->row = r++ ;
            b->it = i ;
            b->action = NoButtonAction ;
            buttonItem->childs.push_back( b );
        }
        endInsertColumns();
        endInsertRows() ;
        emit dataChanged( index(0,0,buttonIndex()) , index(js->buttons,2,buttonIndex()) );
        
        emit layoutChanged();
    }
    
    return ok ;
}

/*!
 * Return item associated with the index
 */
jsItem * JoystickConfigurationModel::getItem(const QModelIndex& index) const
{
    if ( !js ) return 0 ;
    
    if ( index.isValid() )
    {
       jsItem * item = static_cast<jsItem*>( index.internalPointer() ) ;
       return item ;
    }
    else 
        return 0 ;
}


/*!
 * 
 */
QModelIndex JoystickConfigurationModel::index( int row , int column , const QModelIndex & parent) const
{
    if ( !js ) return QModelIndex() ;
    
    if (parent.isValid() && parent.column() != 0)
         return QModelIndex();   
    
    jsItem * parentItem = getItem(parent) ;
    
    if ( parentItem == 0 ) // looking for top items 
    {
        return (!row)? axisIndex() : buttonIndex() ;
    }
    else  // loking for child item
    {
        if ( parentItem->childs.isEmpty() ) 
            return QModelIndex() ;
        else 
        {             
            jsItem * childItem = parentItem->childs.at( row ) ;
            return createIndex( row , column , childItem ) ;
        }
    }
    
    return QModelIndex() ;
}

/*!
 * Get index of parent item
 */
QModelIndex JoystickConfigurationModel::parent(const QModelIndex& child) const
{
    if ( !js ) 
        return QModelIndex() ;
    
    if (!child.isValid())
        return QModelIndex();

    jsItem * childItem = getItem(child);
    jsItem * parentItem = childItem->parent ;

    if ( !parentItem )
        return QModelIndex();
    else if ( parentItem == axisItem ) 
        return axisIndex() ;
    else if ( parentItem == buttonItem ) 
        return buttonIndex() ;

    return QModelIndex() ;
}

/*!
 * rowCount
 */
int JoystickConfigurationModel::rowCount( const QModelIndex & parent ) const
{
    if ( !js ) 
        return 0 ;
    
    if ( !parent.isValid() ) // Model root
        return 2 ;
    else 
    {
        jsItem * parentItem = getItem(parent) ;
        return parentItem->childs.size() ;
    }
}

/*!
 * 
 */
int JoystickConfigurationModel::columnCount(const QModelIndex& parent) const
{
    if (!js)
        return 0 ;
    
    if ( !parent.isValid() ) // Model root
        return 1 ;
    else 
    {
        jsItem * parentItem = getItem(parent) ;
        if ( !parentItem->childs.isEmpty() ) 
            return 3 ;
        else 
            return 1 ;
    }

}

/*!
 * 
 */
QVariant JoystickConfigurationModel::data( const QModelIndex& index , int role ) const
{
    if ( !js || !index.isValid() ) 
        return QVariant() ;
    
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();
    
    jsItem * item = getItem(index) ;
    if ( item == axisItem ) 
    {
        return QString("axes") ;
    }
    else if ( item == buttonItem ) 
    {
        return QString("buttons") ;
    }
    else 
    {
        if ( index.column() == 0 ) 
        {
            QString str ;
            if ( getItem(index.parent()) == axisItem )
                str = "axis %1" ;
            else 
                str = "button %1" ;
                    
            return str.arg(index.row()) ;
        }
        else if ( index.column() == 1 ) 
        {
            return QVariant::fromValue<int16_t>( *(item->it) ) ;
        }
        else if ( index.column() == 2 ) 
        {
            return QVariant::fromValue<int>( item->action ) ;
        }
            
    }
       
    return QVariant() ;
}

/*!
 * 
 */
bool JoystickConfigurationModel::setData(const QModelIndex & index, const QVariant& value, int role)
{
    if (role != Qt::EditRole)
        return false ;
    else if ( !index.isValid() || index.column() != 2 ) 
        return false ;
    
    jsItem * item = getItem( index ) ;
    if ( item ) 
    {
        item->action = value.toInt() ;
        emit dataChanged(index,index);
        return true ;
    }
    
    return QAbstractItemModel::setData(index,value,role) ;
}


/*!
 * 
 */
void JoystickConfigurationModel::timerEvent(QTimerEvent* event)
{
    if ( !js && !js->active )
        return ;
    
    emit dataChanged( axisIndex() , axisIndex() ) ;
    QModelIndex a0 = index( 0 , 1 , axisIndex() ) ;
    QModelIndex a1 = index( (int)js->axes , 1 , axisIndex() ) ;
    emit dataChanged(a0,a1);
    
    emit dataChanged( buttonIndex() , buttonIndex() ) ;
    QModelIndex b0 = index( 0 , 1 , buttonIndex() ) ;
    QModelIndex b1 = index( (int)js->axes , 1 , buttonIndex() ) ;
    emit dataChanged(b0,b1);
}

/*!
 * 
 */
void JoystickConfigurationModel::startAutoRefresh()
{
    if ( !js ) 
        return ;
    
    if ( !js->active ) 
        js->start() ;
    
    startTimer(50) ;
}

/*!
 * 
 */
QString JoystickConfigurationModel::name() const
{
    if ( !js ) 
        return QString() ;
    else 
        return QString::fromStdString( js->name ) ;
}

/*!
 * 
 */
Qt::ItemFlags JoystickConfigurationModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return 0 ;
    else 
    {
        if ( index.column() == 2 ) 
            return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable ;
        else 
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable ;
    }
}

/*!
 * 
 */
QModelIndex JoystickConfigurationModel::axisIndex() const
{
    return createIndex(0,0,axisItem) ;
}

QModelIndex JoystickConfigurationModel::buttonIndex() const
{
    return createIndex(1,0,buttonItem) ;
}

