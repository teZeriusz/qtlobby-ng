/*
 * Created on 2006.11.2
 *
 */

/**
 * @author Betalord
 *
 */

import java.io.IOException;
import java.nio.channels.*;
import java.util.ArrayList;

public class Clients {

	static private ArrayList<Client> clients = new ArrayList<Client>();
	
	static private ArrayList<Client> killList = new ArrayList<Client>(); // a list of clients waiting to be killed (disconnected)
	/* killList is used when we want to kill a client but not immediately (within a loop, for example).
	 * Client on the list will get killed after main server loop reaches its end. 
	 * Also see killClientDelayed() method. Any redundant entries will be
	 * removed (client will be killed only once), so no additional logic for
	 * consistency is required. */
	static private ArrayList<String>reasonList = new ArrayList<String>(); // used with killList list (gives reason for each scheduled kill) 
	
	
	/* will create new Client object and add it to the 'clients' list
	 * and will also register it's socket channel with 'readSelector'.
	 * Use 'sendBufferSize' to specify socket's send buffer size. */
	public static Client addNewClient(SocketChannel chan, Selector readSelector, int sendBufferSize) {
		
    	Client client = new Client(chan);
    	clients.add(client);
			
       	// register the channel with the selector 
       	// store a new Client as the Key's attachment
       	try {
       	    chan.configureBlocking(false);
       	    chan.socket().setSendBufferSize(sendBufferSize);
       	    //***chan.socket().setSoTimeout(TIMEOUT_LENGTH); -> this doesn't seem to have an effect with java.nio
       	    client.selKey = chan.register(readSelector, SelectionKey.OP_READ, client);
       	} catch (ClosedChannelException cce) {
       		killClient(client);
       		return null;
       	} catch (IOException ioe) {
       		killClient(client);
       		return null;
       	} catch (Exception e) {
       		killClient(client);
       		return null;
       	}
       	
       	return client;
	}
	
	/* returns number of clients (including those who haven't logged in yet) */
	public static int getClientsSize() {
		return clients.size();
	}
	
	public static Client getClient(String username) {
		for (int i = 0; i < clients.size(); i++)
			if (clients.get(i).account.user.equals(username)) return clients.get(i);
		return null;	
	}
	
	/* returns null if index is out of bounds */
	public static Client getClient(int index) {
		try {
			return clients.get(index);
		} catch (IndexOutOfBoundsException e) {
			return null;
		}
	}	

	/* returns true if user is logged in */
	public static boolean isUserLoggedIn(Account acc) {
		for (int i = 0; i < clients.size(); i++) {
			if (clients.get(i).account.user.equals(acc.user)) return true;
		}
		return false;
	}
	
	public static void sendToAllRegisteredUsers(String s) {
		for (int i = 0; i < clients.size(); i++) {
			if (clients.get(i).account.accessLevel() < Account.NORMAL_ACCESS) continue;
			clients.get(i).sendLine(s);
		}
	}	

	/* sends text to all registered users except for the client */
	public static void sendToAllRegisteredUsersExcept(Client client, String s) {
		for (int i = 0; i < clients.size(); i++) {
			if (clients.get(i).account.accessLevel() < Account.NORMAL_ACCESS) continue;
			if (clients.get(i) == client) continue; 
			clients.get(i).sendLine(s);
		}
	}
	
	public static void sendToAllAdministrators(String s) {
		for (int i = 0; i < clients.size(); i++) {
			if (clients.get(i).account.accessLevel() < Account.ADMIN_ACCESS) continue;
			clients.get(i).sendLine(s);
		}
	}
	
	/* notifies client of all statuses, including his own (but only if they are different from 0) */
	public static void sendInfoOnStatusesToClient(Client client) {
		for (int i = 0; i < clients.size(); i++) {
			if (clients.get(i).account.accessLevel() < Account.NORMAL_ACCESS) continue;
			if (clients.get(i).status != 0) // only send it if not 0. User assumes that every new user's status is 0, so we don't need to tell him that explicitly.
				client.sendLine("CLIENTSTATUS " + clients.get(i).account.user + " " + clients.get(i).status);
		}
	}

	/* notifies all logged-in clients (including this client) of the client's new status */
	public static void notifyClientsOfNewClientStatus(Client client) {
		sendToAllRegisteredUsers("CLIENTSTATUS " + client.account.user + " " + client.status);
	}	
	
	/* sends a list of all users connected to the server to client (this list includes
	 * the client itself, assuming he is already logged in and in the list) */
	public static void sendListOfAllUsersToClient(Client client) {
		for (int i = 0; i < clients.size(); i++) {
			if (clients.get(i).account.accessLevel() < Account.NORMAL_ACCESS) continue;
			client.sendLine("ADDUSER " + clients.get(i).account.user + " " + clients.get(i).country + " " + clients.get(i).cpu);
		}
	}

	/* notifies all registered clients of a new client who just logged in. The new client
	 * is not notified (he is already notified by some other method) */
	public static void notifyClientsOfNewClientOnServer(Client client) {
		for (int i = 0; i < clients.size(); i++) {
			if (clients.get(i).account.accessLevel() < Account.NORMAL_ACCESS) continue;
			if (clients.get(i) == client) continue;
			clients.get(i).sendLine("ADDUSER " + client.account.user + " " + client.country + " " + client.cpu);
		}
	}

	/* The client who just joined the battle is also notified (he should also be notified
	 * with JOINBATTLE command. See protocol description) */
	public static void notifyClientsOfNewClientInBattle(Battle battle, Client client) {
		for (int i = 0; i < clients.size(); i++)  {
			if (clients.get(i).account.accessLevel() < Account.NORMAL_ACCESS) continue;
			clients.get(i).sendLine("JOINEDBATTLE " + battle.ID + " " + client.account.user);
		}
	}
	
	/* see the overloaded killClient() method for more info */
	public static boolean killClient(Client client) {
		return killClient(client, "");
	}
	
	/* this method disconnects and removes client from the clients list. 
	 * Also cleans up after him (channels, battles) and notifies other
	 * users of his departure. "reason" is used with LEFT command to
	 * notify other users on same channel of this client's departure
	 * reason (it may be left blank ("") to give no reason). */
	public static boolean killClient(Client client, String reason) {
		int index = clients.indexOf(client);
		if (index == -1) return false;
		if (!client.alive) return false;
		client.disconnect();
		clients.remove(index);
		client.alive = false;
		if (reason.trim().equals("")) reason = "Quit";
		
		// let's remove client from all channels he is participating in:
		client.leaveAllChannels(reason);
		
		if (client.battleID != -1) {
			Battle bat = Battles.getBattleByID(client.battleID);
			if (bat == null) {
				System.out.println("Serious error occured: Invalid battle ID. Server will now exit!");
				TASServer.closeServerAndExit();
			}
			Battles.leaveBattle(client, bat); // automatically checks if client is founder and closes the battle
		}
		
		if (client.account.accessLevel() != Account.NIL_ACCESS) {
			sendToAllRegisteredUsers("REMOVEUSER " + client.account.user);
			if (TASServer.DEBUG > 0) System.out.println("Registered user killed: " + client.account.user);
		} else {
			if (TASServer.DEBUG > 0) System.out.println("Unregistered user killed");
		}

		if (TASServer.LAN_MODE) {
			Accounts.removeAccount(client.account);
		}
		
		return true;
	}
	
	/* this method will cause the client to be killed, but not immediately - it will do it
	 * once main server loop reaches its end. We need this on some occassions when we iterate
	 * through the clients list - we don't want client to be removed from the clients list 
	 * since that would "broke" our loop (since we go from index 0 to highest index, which 
	 * would be invalid as highest index would decrease by 1). */
	public static void killClientDelayed(Client client, String reason) {
		killList.add(client);
		reasonList.add(reason);
	}
	
	/* this will kill all clients in the current kill list and empty it.
	 * Must only be called from the main server loop (at the end of it)!
	 * Any redundant entries are ignored (cleared). */
	public static void processKillList() {
	    for (; killList.size() > 0;) {
	    	killClient(killList.get(0), reasonList.get(0));
	    	killList.remove(0);
	    	reasonList.remove(0);
	    }
	}
}
