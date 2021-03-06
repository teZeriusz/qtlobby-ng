// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "InputLine.h"
#include "CompletionListWidget.h"
#include <QDebug>
#include <QListWidget>

InputLine::InputLine( QWidget * parent ) : QLineEdit( parent ), historyIndex( 0 ) {
    history << "" << "";
    users = 0;
    currentIndex = -1;
    previousWasTab = false;
}
InputLine::~InputLine() {
}

/**
 * keyPressEvent and arrowkeys history behavior
 * @param event the keyEvent from QObject
 */
void InputLine::keyPressEvent( QKeyEvent * event ) {
    if ( event->key() == Qt::Key_Up ) {
        if ( historyIndex < history.count() - 2 ) {
            setText( history[++historyIndex] );
        }
        QLineEdit::keyPressEvent( event );
        return;
    } else if ( event->key() == Qt::Key_Down ) {
        if ( historyIndex > 0 && historyIndex < history.count() ) {
            setText( history[--historyIndex] );
        }
        QLineEdit::keyPressEvent( event );
        return;
    } else if ( event->key() == Qt::Key_Return
             || event->key() == Qt::Key_Enter ) {
        history[0] = text();
        returnPressed();
        historyIndex = 0;
    } else
        historyIndex = 0;
    QLineEdit::keyPressEvent( event );
    history[0] = text();
}

/**
  * handling tab completion
  */
bool InputLine::event(QEvent* event) {
    if (users && event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Tab ||
            (keyEvent->key() == Qt::Key_Space && keyEvent->modifiers() == Qt::ControlModifier)) {
            if (previousWasTab)
                return QLineEdit::event(event);
            previousWasTab = true;
            QString str = text();
            int start = cursorPosition();
            QFontMetrics fm(font());
            int width = fm.width(str.left(start));
            int end = start;
            while (start > 0 && !str[start].isSpace()) {
                start--;
            }
            if (start == end) return true;
            start = start == 0 ? start : start + 1;
            str = str.mid(start, end - start);
            QStringList usernames = users->getUsernamesList();
            QStringList variants;
            for (int i = 0; i < usernames.size(); i++) {
                if (usernames[i].contains(str, Qt::CaseInsensitive)) variants << usernames[i];
            }
            if (variants.size() > 1) {
                CompletionListWidget* list = new CompletionListWidget();
                connect(list, SIGNAL(completionSelected(int,int,QString)), SLOT(onCompletionSelected(int,int,QString)));
                connect(list, SIGNAL(simulateKeyEvent(QKeyEvent*)), SLOT(emulateKeyEvent(QKeyEvent*)));
                list->setCompletionParams(start, end-start);
                list->setInitianBuffer(str);
                list->setInitianSubstitutions(usernames);
                list->move(mapToGlobal(QPoint(width, height())));
                list->setWindowFlags(Qt::Popup);
                list->sortItems(Qt::AscendingOrder);
                list->show();
            } else if (variants.size() == 1) {
                setText(text().replace(start, end - start, variants[0]));
            }
            return true;
        }
        previousWasTab = false;
        if ( keyEvent->key() == Qt::Key_Escape ) {
            setText("");
            return true;
        }
    }
    return QLineEdit::event(event);
}

void InputLine::onCompletionSelected(int start, int length, QString str) {
    setText(text().replace(start, length, str));
}

void InputLine::setUsers(Users* users) {
    this->users = users;
}

/**
 * sends the input text after short trimming
 * and saves the history
 */
void InputLine::returnPressed() {
    //allow at most one leading space and now trailing spaces
    QString input = text().trimmed();
    if ( input.size() == 0 )
        return;
    input = input.prepend( text().at( 0 ).isSpace() ? " " : "" );
    clear();
    // this disables multiple equal history messages in a series
    if ( history[1] != history[0] )
        history.prepend( "" );
    // limit the history to 20 history entries
    if ( history.count() > 22 )
        history.removeLast();
    input.replace("\\\\","___###SLASH__PLACEHOLDER###___");
    input.replace("\\b",QChar::fromAscii(2));
    input.replace("\\k",QChar::fromAscii(3));
    input.replace("\\o",QChar::fromAscii(15));
    input.replace("\\f",QChar::fromAscii(17));
    input.replace("\\s",QChar::fromAscii(29));
    input.replace("\\u",QChar::fromAscii(31));
    input.replace("\\n","[br]");
    input.replace("___###SLASH__PLACEHOLDER###___", "\\");
    input.replace("\n","[br]");
    emit sendInput( input );
    emit returnPressed();
}

void InputLine::onChannelActivated(QModelIndex modelIndex) {
    //we assume that number of chanels/privates ofr a user will be < 1000000 and generate unique number describing a channel or private
    int index = modelIndex.parent().row() * 1000000 + modelIndex.row();
    buffers[currentIndex] = text();
    clear();
    if (buffers.contains(index))
        setText(buffers[index]);
    if (currentIndex != index)
        this->setFocus();
    currentIndex = index;
}

void InputLine::emulateKeyEvent(QKeyEvent* event) {
    QLineEdit::keyPressEvent(event);
}
