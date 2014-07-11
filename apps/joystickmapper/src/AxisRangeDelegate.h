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

#ifndef AXISRANGEDELEGATE_H
#define AXISRANGEDELEGATE_H

#include <QStyledItemDelegate>

class AxisRangeDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    qreal max_v ;
    qreal min_v ;
    
public:
    explicit AxisRangeDelegate( QObject * parent = 0 );
    virtual ~AxisRangeDelegate();
    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const ;
    virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const ;

private:
    qreal computeRelativePosition( const short & v ) const ;
    
};

#endif // AXISRANGEDELEGATE_H
