// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef USER_H
#define USER_H

#include <QVariant>
#include <QColor>
#include <QDebug>

#define TB(a, n) (a & (1 << (n))) // test bit
#define SB(a, n) a |= (1 << (n))  // set bit
#define CB(a, n) a &= ~(1 << (n)) // clear bit
#define COPYBIT(s, si, d, di) if(TB(s, si)) SB(d, di); else CB(d, di);
#define COPYBITS(s, si, d, di, n) for(int i = 0; i < n; i++) {COPYBIT(s, si+i, d, di+i);}
#define EXTRACT(s, si, m) (s & (m << si)) >> si;

/*
    * b0 = in game (0 - normal, 1 - in game)
    * b1 = away status (0 - normal, 1 - away)
    * b2-b4 = rank (see Account class implementation for description of rank) - client is not allowed to change rank bits himself (only server may set them).
    * b5 = access status (tells us whether this client is a server moderator or not) - client is not allowed to change this bit himself (only server may set them).
    * b6 = bot mode (0 - normal user, 1 - automated bot). This bit is copied from user's account and can not be changed by the client himself. Bots differ from human players in that they are fully automated and that some anti-flood limitations do not apply to them.
*/

class UserState {
public:
    UserState(int state) {
        m_state = state;
    }
    UserState() {
        m_state = 0;
    }
    bool isIngame() const {
        return EXTRACT(m_state, 0, 0x1);
    }
    void setIngame(bool b) {
        COPYBIT(b, 0, m_state, 0);
    }
    bool isAway() const {
        return EXTRACT(m_state, 1, 0x1)
            }
    void setAway(bool b) {
        COPYBIT(b, 0, m_state, 1);
    }
    char getRank() const {
        return EXTRACT(m_state, 2, 0x7);
    }
    bool isModerator() {
        return EXTRACT(m_state, 5, 0x1);
    }
    bool isBot() const {
        return EXTRACT(m_state, 6, 0x1);
    }
    void setState(int state) {
        m_state = state;
    }
    int getState() const {
        return m_state;
    }
private:
    int m_state;
};

/*
    * b0 = undefined (reserved for future use)
    * b1 = ready (0=not ready, 1=ready)
    * b2..b5 = team no. (from 0 to 15. b2 is LSB, b5 is MSB)
    * b6..b9 = ally team no. (from 0 to 15. b6 is LSB, b9 is MSB)
    * b10 = mode (0 = spectator, 1 = normal player)
    * b11..b17 = handicap (7-bit number. Must be in range 0..100). Note: Only host can change handicap values of the players in the battle (with HANDICAP command). These 7 bits are always ignored in this command. They can only be changed using HANDICAP command.
    * b18..b21 = reserved for future use (with pre 0.71 versions these bits were used for team color index)
    * b22..b23 = sync status (0 = unknown, 1 = synced, 2 = unsynced)
    * b24..b27 = side (e.g.: arm, core, tll, ... Side index can be between 0 and 15, inclusive)
    * b28..b31 = undefined (reserved for future use)

*/

class BattleState {
public:
    BattleState(int state) {
        m_state = state;
    }
    BattleState() {
        m_state = 0;
    }
    bool isReady() const {
        return EXTRACT(m_state, 1, 0x1);
    }
    void setReady(bool b) {
        COPYBIT(b, 0, m_state, 1);
    }
    quint8 getTeamNo() const {
        return EXTRACT(m_state, 2, 0xF);
    }
    void setTeamNo(quint8 b) {
        COPYBITS(b, 0, m_state, 2, 4);
    }
    quint8 getAllyTeamNo() const {
        return EXTRACT(m_state, 6, 0xF);
    }
    void setAllyTeamNo(quint8 b) {
        COPYBITS(b, 0, m_state, 6, 4);
    }
    bool isPlayer() const {
        return EXTRACT(m_state, 10, 0x1);
    }
    void setPlayer(bool b) {
        COPYBIT(b, 0, m_state, 10);
    }
    quint8 getHandicap() const {
        return EXTRACT(m_state, 11, 0x7F);
    }
    void setHandicap(quint8 b) {
        COPYBITS(b, 0, m_state, 11, 7);
    }
    quint8 getColor() const {
        return EXTRACT(m_state, 18, 0xF);
    }
    void setColor(quint8 b) {
        COPYBITS(b, 0, m_state, 18, 4);
    }
    quint8 syncState() const {
        return EXTRACT(m_state, 22, 0x3);
    }
    void setSyncState(quint8 b) {
        COPYBITS(b, 0, m_state, 22, 2);
    }
    quint8 getSide() const {
        return EXTRACT(m_state, 24, 0xF);
    }
    void setSide(quint8 b) {
        COPYBITS(b, 0, m_state, 24, 4);
    }
    void setState(int state) {
        m_state = state;
    }
    int getState() const {
        return m_state;
    }
private:
    int m_state;
};

class User { /*: public QObject*/
    // Q_OBJECT
public:
    User();
    ~User();

    void setColor( int TASColor );
    int color();
    QString colorForScript();

    /* UserInformation */
    QString name;            //!< like "[NC]qknight"
    QString countryCode;     //!< like "DE"
    QString cpu;             //!< like "2100"
    int joinedBattleId;      //!< like 1234
    BattleState battleState; //!< like 0 to 2^32-1
    UserState userState;     //!< like 0 to 2^7-1 = 127
    QColor m_color;          //!< the in game color for the units
    bool founder;
};
Q_DECLARE_METATYPE( User );
#endif
