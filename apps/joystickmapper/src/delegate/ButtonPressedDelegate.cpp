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

#include "ButtonPressedDelegate.h"

#include <QPainter>

using namespace mogs ;

/*!
 *
 */
ButtonPressedDelegate::ButtonPressedDelegate(QObject* parent) :
    QStyledItemDelegate(parent)
{

}

/*!
 *
 */
ButtonPressedDelegate::~ButtonPressedDelegate()
{

}

/*!
 * Size of the delegate;
 */
QSize ButtonPressedDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(25,25) ;
}

/*!
 * Painting the delegate
 */
void ButtonPressedDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if ( !index.data().toInt() )
        painter->setBrush( Qt::darkRed );
    else 
        painter->setBrush( Qt::red );
    
    painter->setPen( Qt::black );
    painter->drawEllipse( option.rect );

    QFont f = option.font ;
    f.setBold(true);
    painter->setPen(Qt::white);
    painter->setFont(f);
    painter->drawText( option.rect , Qt::AlignCenter , QString::number(index.row()));
}
