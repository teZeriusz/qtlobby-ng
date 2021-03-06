// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "BattleCloseFirstWidget.h"

BattleCloseFirstWidget::BattleCloseFirstWidget( QDialog *parent ) : QDialog( parent ) {
    setupUi( this );
    id = 0;
    connect( battleCloseFirstButtonBox, SIGNAL( accepted() ), this, SLOT( acceptThis() ) );
    connect( battleCloseFirstButtonBox, SIGNAL( rejected() ), this, SLOT( close() ) );
}

BattleCloseFirstWidget::~BattleCloseFirstWidget() { }

void BattleCloseFirstWidget::acceptThis() {
    Settings::Instance()->setValue( "Battle/autoCloseFirst", battleCloseFirstCheckBox->isChecked() );
    hide();
    if( id == -1 )
        emit wantHostBattle( true );
    else
        emit wantJoinBattle( id, password, true );
}

void BattleCloseFirstWidget::setBattleId( int id ) {
    this->id = id;
    battleCloseFirstCheckBox->setChecked( Settings::Instance()->value( "Battle/autoCloseFirst", false ).toBool() );
}

void BattleCloseFirstWidget::setPassword( QString password ) {
    this->password = password;
}
