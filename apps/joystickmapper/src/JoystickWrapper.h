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

#ifndef __JOYSTICKWRAPPER_H__
#define __JOYSTICKWRAPPER_H__

#include <QStandardItemModel>
#include <QSharedPointer>
#include <MoGS_Joystick.h>

class cJoystick;
class jsItem ;

namespace mogs
{

class JoystickWrapper : public QAbstractItemModel
{
    Q_OBJECT
    Q_ENUMS( AxisActions ButtonActions ) ;
    
    QSharedPointer<cJoystick> js ;
    jsItem * axisItem ;
    jsItem * buttonItem ;

public:

    enum AxisActions 
    {
        NoAxisAction = 0 ,
        AxisFrontBack = 1 ,
        AxisLeftRight = 2 ,
        AxisUpDown = 3 ,
        AxisRotate = 4
    } ;
    
    enum ButtonActions
    {
       NoButtonAction = 0 ,
       ButtonFront = 1 ,
       ButtonBack = 2 ,
       ButtonLeft = 3 ,
       ButtonRight = 4 ,
       ButtonUp = 5 ,
       ButtonDown = 6 ,
       ButtonRotateLeft = 7 ,
       ButtonRotateRight = 8 ,
       ButtonStart = 9 ,
       ButtonStop = 10
    } ;
    
    explicit JoystickWrapper( QObject * parent );
    ~JoystickWrapper();
    
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const ;
    virtual QModelIndex parent(const QModelIndex& child) const ;
    virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const ;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const ; 
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const ;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole ) ;
    virtual Qt::ItemFlags flags( const QModelIndex & index ) const ;
    virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    
    bool openDevice( const QString& device ) ;
    QString name() const ;
    QWeakPointer<cJoystick> ptr() const { return js.toWeakRef() ; };
    QModelIndex axisIndex() const ;
    QModelIndex buttonIndex() const ;
    
public slots :
    void startAutoRefresh() ;
    void readConfig() ;
    void saveConfig() ;
    
protected :
    void timerEvent( QTimerEvent * event ) ;
    
private :
    jsItem * getItem( const QModelIndex & index ) const ;
    bool validRole( const int & role ) const ;
    bool actionIsSet( const ButtonActions & action ) const ;
    bool actionIsSet( const AxisActions & action ) const ;
    QStringList getAxisActions() const ;
    QStringList getButtonActions() const ;
};

};

#endif // JOYSTICKCONFIGURATIONMODEL_H
