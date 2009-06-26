/*
 * Created on 2006.8.4
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 * 
 */

/**
 * @author Betalord
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */

import java.util.*;

public class MapGrading {
	public static ArrayList<GlobalMapGrade> globalMapGrades = new ArrayList<GlobalMapGrade>();
	
	private static GlobalMapGrade findGlobalMapGrade(String mapHash) {
		for (int i = 0; i < globalMapGrades.size(); i++)
			if (globalMapGrades.get(i).mapHash.equals(mapHash))
				return globalMapGrades.get(i);
		return null;
	}
	
	/* will reconstruct global map grade list from accounts info */
	public static void reconstructGlobalMapGrades() {
		globalMapGrades.clear();
		for (int i = 0; i < Accounts.getAccountsSize(); i++) {
			if (Accounts.getAccount(i).getRank() < 3) continue ; // accept only grades from players with rank higher than "beginner"
			MapGradeList grades = Accounts.getAccount(i).mapGrades;
			if (grades.size() == 0) continue;
			for (int j = 0; j < grades.size(); j++) {
				addGlobalGrade(grades.elementAt(j).hash, grades.elementAt(j).grade);
			}
		}
	}

	public static float getAvarageMapGrade(String mapHash) {
		GlobalMapGrade mg = findGlobalMapGrade(mapHash);
		if (mg == null) return 0;
		else return mg.avGrade;
	}
	
	public static int getNumberOfMapVotes(String mapHash) {
		GlobalMapGrade mg = findGlobalMapGrade(mapHash);
		if (mg == null) return 0;
		else return mg.noVotes;
	}
	
	/* where "grade" should be between 1 and 10 */
	private static void addGlobalGrade(String mapHash, int grade) {
		if (grade == 0) return ;
		GlobalMapGrade mg = findGlobalMapGrade(mapHash);
		if (mg == null) {
			mg = new GlobalMapGrade(mapHash);
			globalMapGrades.add(mg);
		}
		mg.avGrade = (mg.avGrade * mg.noVotes + grade) / (++mg.noVotes);
	}
	
	/* will update client's grade list and also global grade list */
	public static void updateLocalAndGlobalGrade(Client client, String mapHash, int grade) {
		if (grade == 0) return ; // 0 means user hasn't graded the map yet, so we ignore it

		if (client.account.getRank() < 3) return ; // accept only grades from players with rank higher than "beginner"
		
		MapGrade mg = client.account.mapGrades.findMapGrade(mapHash);
		if (mg == null) { // this is client's first time grading this map
			mg = new MapGrade(mapHash, grade, 0);
			client.account.mapGrades.add(mg);
			// update global map grades:
			GlobalMapGrade gmg = findGlobalMapGrade(mapHash);
			if (gmg == null) {
				gmg = new GlobalMapGrade(mapHash);
				globalMapGrades.add(gmg);
			}
			gmg.avGrade = (gmg.avGrade * gmg.noVotes + grade) / (++gmg.noVotes);
		} else { // client is only adjusting grade for this map
			if (mg.grade == grade) return ; // no change
			int diff = grade - mg.grade;
			mg.grade = grade;
			// update global map grades:
			GlobalMapGrade gmg = findGlobalMapGrade(mapHash);
			if (gmg == null) {
				gmg = new GlobalMapGrade(mapHash);
				globalMapGrades.add(gmg);
			}
			gmg.avGrade = (gmg.avGrade * gmg.noVotes + diff) / gmg.noVotes;
		}		
	}
	
	/* 'addMins' - how many minutes to add to the "map grade" */
	public static void updateLocalMapGradeMins(Client client, String mapHash, int addMins) {
		MapGrade mg = client.account.mapGrades.findMapGrade(mapHash);
		if (mg == null) {
			mg = new MapGrade(mapHash, 0, addMins);
			client.account.mapGrades.add(mg);
		} else {
			mg.mins += addMins;
		}
	}
	
}
