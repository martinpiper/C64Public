package Solver;

/**
 *
 */
public class Node
{
	public enum Directions {None, Up, Down, Left, Right}
	public boolean isBlocked = false;	///< Blocked or not, a solid wall for example
	public int travelCost = 1;			///< The cost to travel over this Node
	public int blockedAtFrame = -1;		///< At a particular time this tile will be blocked
	public int blockedBeyond = -1;		///< After a particular time this tile will be blocked. Indicates something has stopped there.
	public int aversion = 0;			///< Avoid this Node metric. Higher numbers increase the cost to travel this Node
	public int visitedAt = -1;			///< Set with the frame count while growing the route, the value is increased every iteration
	public int currentSolveID = 0;		///< Every solve has a unique ID (up to max int). If the current solve encounters the same ID then it knows it has visited this Node before and skips it. This avoids the need to clear the solved Node array every time a solve is a started.
	public Directions reachedVia = Directions.None;	///< Updated during growing the solution

	public static int getC64Direction(Directions direction)
	{
		switch(direction)
		{
			case None:
				return 0;
			case Up:
				return 1;
			case Down:
				return 2;
			case Left:
				return 3;
			case Right:
				return 4;
			default:
				return -1;
		}
	}
}
