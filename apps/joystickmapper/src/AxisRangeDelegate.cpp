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

#include "AxisRangeDelegate.h"
#include <QPainter>

/*!
 * 
 */
AxisRangeDelegate::AxisRangeDelegate( QObject * parent ) :
  QStyledItemDelegate(parent)
{
  
}

/*!
 * 
 */

AxisRangeDelegate::~AxisRangeDelegate()
{
  
}

/*!
 * 
 */
QSize AxisRangeDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(150,50) ;
//     return QStyledItemDelegate::sizeHint(option, index);
}

/*!
 * 
 */
void AxisRangeDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   QBrush bg = option.palette.background() ;
   painter->drawRect( option.rect );
   
   painter->drawText(option.rect,Qt::AlignCenter,index.data().toString());
//     QStyledItemDelegate::paint(painter, option, index);
}

