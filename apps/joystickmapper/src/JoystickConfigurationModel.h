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

#ifndef JOYSTICKCONFIGURATIONMODEL_H
#define JOYSTICKCONFIGURATIONMODEL_H

#include <QStandardItemModel>
#include <QSharedPointer>

class cJoystick;
struct jsItem ;

class JoystickConfigurationModel : public QAbstractItemModel
{
    Q_OBJECT
    
    QSharedPointer<cJoystick> js ;
    jsItem * axisItem ;
    jsItem * buttonItem ;

public:

    enum AxisActions 
    {
        NoAxisAction = 99 ,
        AxisFrontBack = 0 ,
        AxisLeftRight = 1 ,
        AxisRotate = 2
    } ;
    
    enum ButtonActions
    {
       NoButtonAction = 99 ,
       ButtonFront = 0 ,
       ButtonBack = 1 ,
       ButtonLeft = 2 ,
       ButtonRight = 3 ,
       ButtonRotateLeft = 4 ,
       ButtonRotateRight = 5 ,
       ButtonStart = 6 ,
       ButtonStop = 7
    } ;
    
    explicit JoystickConfigurationModel( QObject * parent );
    ~JoystickConfigurationModel();
    
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const ;
    virtual QModelIndex parent(const QModelIndex& child) const ;
    virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const ;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const ; 
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const ;
    virtual Qt::ItemFlags flags( const QModelIndex & index ) const ;
    
    bool openDevice( const QString& device ) ;
    void startAutoRefresh() ;
    QString name() const ;
    QWeakPointer<cJoystick> ptr() const { return js.toWeakRef() ; };
    
protected :
    void timerEvent( QTimerEvent * event ) ;
    
private :
    jsItem * getItem( const QModelIndex & index ) const ;

};

#endif // JOYSTICKCONFIGURATIONMODEL_H
