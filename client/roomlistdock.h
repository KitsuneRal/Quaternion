/**************************************************************************
 *                                                                        *
 * Copyright (C) 2015 Felix Rohrbach <kde@fxrh.de>                        *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/

#pragma once

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QListView>
#include <QtCore/QStringListModel>

#include "lib/connection.h"

class RoomListModel;
class QuaternionRoom;

class RoomListDock : public QDockWidget
{
        Q_OBJECT
    public:
        RoomListDock(QWidget* parent = nullptr);
        virtual ~RoomListDock();

        void setConnection(QMatrixClient::Connection* newConnection );

    signals:
        void roomSelected(QuaternionRoom* room);

    private slots:
        void showContextMenu(const QPoint& pos);
        void menuJoinSelected();
        void menuLeaveSelected();
        void menuMarkReadSelected();

    private:
        QMatrixClient::Connection* connection;
        QListView* view;
        RoomListModel* model;
        QMenu* contextMenu;
        QAction* joinAction;
        QAction* leaveAction;
        QAction* markAsReadAction;

        QuaternionRoom* getSelectedRoom() const;
};
