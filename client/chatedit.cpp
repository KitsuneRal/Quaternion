/**************************************************************************
 *                                                                        *
 * Copyright (C) 2017 Kitsune Ral <kitsune-ral@users.sf.net>
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

#include "chatedit.h"

#include <QtGui/QKeyEvent>

#include "chatroomwidget.h"

ChatEdit::ChatEdit(ChatRoomWidget* c) : KChatEdit(c), chatRoomWidget(c) { }

void ChatEdit::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Tab) {
        triggerCompletion();
        return;
    }

    cancelCompletion();
    KChatEdit::keyPressEvent(event);
}

void ChatEdit::appendTextAtCursor(const QString& text, bool select)
{
    completionCursor.insertText(text);
    completionCursor.movePosition(QTextCursor::Left,
        select ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor, text.size());
}

void ChatEdit::startNewCompletion()
{
    completionCursor = textCursor();
    completionCursor.clearSelection();
    while ( completionCursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor) )
    {
        auto firstChar = completionCursor.selectedText()[0];
        if (!firstChar.isLetterOrNumber() && firstChar != '@')
        {
            completionCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
            break;
        }
    }
    completionMatches =
        chatRoomWidget->findCompletionMatches(completionCursor.selectedText());
    if ( !completionMatches.isEmpty() )
    {
        matchesListPosition = 0;
        if ( completionCursor.atStart() )
        {
            appendTextAtCursor(": ", false);
            return;
        }
        for (auto stringBefore: {":", ": "})
        {
            completionCursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
            if ( completionCursor.selectedText().startsWith(stringBefore) )
            {
                completionCursor.insertText(", ");
                appendTextAtCursor(": ", false);
                return;
            }
        }
        completionCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 2);
        appendTextAtCursor(" ", false);
    }
}

void ChatEdit::triggerCompletion()
{
    if (completionMatches.isEmpty())
        startNewCompletion();

    if (!completionMatches.isEmpty())
    {
        appendTextAtCursor(completionMatches.at(matchesListPosition), true);
        ensureCursorVisible(); // The real one, not completionCursor
        QTextCharFormat completionHL = completionCursor.charFormat();
        completionHL.setUnderlineStyle(QTextCharFormat::DashUnderline);
        setExtraSelections({ { completionCursor, completionHL } });
        emit proposedCompletion(completionMatches, matchesListPosition);
        matchesListPosition = (matchesListPosition + 1) % completionMatches.length();
    }
}

void ChatEdit::cancelCompletion()
{
    completionMatches.clear();
    setExtraSelections({});
    emit cancelledCompletion();
}

