package Solver;

/**
 *
 */
public class Solve
{
	public boolean anyWorkDone = false;
	public int currentFrame = 0;
	public int indexEarlyOut = -1;
	public boolean earlyOutHit = false;
	public int nextSmallestFrame = -1;
	public int lastSolveCost = -1;
	FIFO<Integer> nextIndex = new FIFO<Integer>(64);
	public int incrementCounter = -1;
	public int maxIndexSize = -1;
	public int currentSolveID = 0;

	public Grid grid;

	public Solve()
	{
	}

	/**
	 * Sets the start position
	 */
	public void setStartPosition(int index)
	{
		currentSolveID++;
		grid.getNode(index).visitedAt = 0;
		grid.getNode(index).currentSolveID = currentSolveID;
		nextIndex.clear();
		nextIndex.add(index);
		anyWorkDone = false;
		currentFrame = 0;
		earlyOutHit = false;
		nextSmallestFrame = -1;
		lastSolveCost = -1;
		incrementCounter = 1;
		maxIndexSize = -1;
	}

	/**
	 * Call step() until it returns false indicating no more work can be done.
	 * @return true indicates more work needs to be done. false means there is no more work.
	 */
	public boolean step()
	{
		if (nextIndex.size() > maxIndexSize)
		{
			maxIndexSize = nextIndex.size();
		}
		if (nextIndex.isEmpty())
		{
			return false;
		}
		int index = nextIndex.remove();
		// Try to grow the current square
		Node node = grid.getNode(index);
		if (node.visitedAt == currentFrame)
		{
			// Consider it for growth
			growTo(node, index - 1, Node.Directions.Left);
			growTo(node, index + 1, Node.Directions.Right);
			growTo(node, index - grid.width, Node.Directions.Up);
			growTo(node, index + grid.width, Node.Directions.Down);
			index = -1;
		}

		if (earlyOutHit)
		{
			return false;
		}

		if (index != -1)
		{
			nextIndex.add(index);
			if (nextSmallestFrame == -1 || node.visitedAt < nextSmallestFrame)
			{
				nextSmallestFrame = node.visitedAt;
			}
		}

		incrementCounter--;
		if (incrementCounter == 0)
		{
			if (nextSmallestFrame != -1)
			{
				currentFrame = nextSmallestFrame;
			}
			else
			{
				currentFrame++;
			}
			nextSmallestFrame = -1;
			incrementCounter = nextIndex.size();
		}

		return true;
	}

	/**
	 *
	 * @param parentNode
	 * @param newIndex
	 * @return false if the Node is not valid to be grown to. true if the Node is valid.
	 */
	private boolean growTo(Node parentNode, int newIndex, Node.Directions directionTravelled)
	{
		Node node = grid.getNode(newIndex);

		if (null == node)
		{
			return false;
		}
		if (node.isBlocked)
		{
			return false;
		}
		if (node.currentSolveID == currentSolveID)
		{
			return false;
		}
		if (node.blockedBeyond != -1 && currentFrame > node.blockedBeyond)
		{
			return false;
		}

		node.reachedVia = directionTravelled;

		node.currentSolveID = currentSolveID;
		// Calculate when to do the next movement
		node.visitedAt = currentFrame + node.travelCost + node.aversion;
		if (node.blockedAtFrame == currentFrame)
		{
			node.visitedAt += node.travelCost;
		}

		if (newIndex == indexEarlyOut)
		{
			earlyOutHit = true;
		}

		anyWorkDone = true;

		nextIndex.add(newIndex);
		if (nextSmallestFrame == -1 || node.visitedAt < nextSmallestFrame)
		{
			nextSmallestFrame = node.visitedAt;
		}

		return true;
	}

	public String traceBack(int xPos, int yPos)
	{
		return traceBack(grid.getIndex(xPos, yPos));
	}
	public String traceBack(int index)
	{
		lastSolveCost = -1;
		String route = "";
		Node node = null;
		do
		{
			node = grid.getNode(index);
			if (null != node)
			{
				// If the current solveID doesn't match then return the route so far
				if (node.currentSolveID != currentSolveID)
				{
					return route;
				}
				if (lastSolveCost == -1)
				{
					lastSolveCost = node.visitedAt;
				}
				switch (node.reachedVia)
				{
					case Up:
						route = "U" + route;
						index+=grid.width;
						break;
					case Down:
						route = "D" + route;
						index-=grid.width;
						break;
					case Left:
						route = "L" + route;
						index++;
						break;
					case Right:
						route = "R" + route;
						index--;
						break;
					case None:
					default:
						node = null;
						break;
				}
			}
		} while (null != node);
		return route;
	}

	public String traceBackClosest(int xpos, int ypos)
	{
		String route = "";
		int smallestSolve = -1;
		int finalPosX = -1;
		int finalPosY = -1;
		Node node = null;
		for (int searchArea = 1; searchArea < 5; searchArea++)
		{
			// Search the increasingly sized boxes until there is a match
			for (int searchX = xpos-searchArea; searchX <= xpos+searchArea; searchX++)
			{
				node = grid.getNode(grid.getIndex(searchX, ypos - searchArea));
				if (null != node && node.currentSolveID == currentSolveID)
				{
					if (smallestSolve == -1 || (node.visitedAt < smallestSolve))
					{
						smallestSolve = node.visitedAt;
						finalPosX = searchX;
						finalPosY = ypos - searchArea;
					}
				}
				node = grid.getNode(grid.getIndex(searchX, ypos + searchArea));
				if (null != node && node.currentSolveID == currentSolveID)
				{
					if (smallestSolve == -1 || (node.visitedAt < smallestSolve))
					{
						smallestSolve = node.visitedAt;
						finalPosX = searchX;
						finalPosY = ypos + searchArea;
					}
				}
			}

			for (int searchY = ypos - searchArea; searchY <= ypos + searchArea; searchY++)
			{
				node = grid.getNode(grid.getIndex(xpos - searchArea, searchY));
				if (null != node && node.currentSolveID == currentSolveID)
				{
					if (smallestSolve == -1 || (node.visitedAt < smallestSolve))
					{
						smallestSolve = node.visitedAt;
						finalPosX = xpos - searchArea;
						finalPosY = searchY;
					}
				}
				node = grid.getNode(grid.getIndex(xpos + searchArea, searchY));
				if (null != node && node.currentSolveID == currentSolveID)
				{
					if (smallestSolve == -1 || (node.visitedAt < smallestSolve))
					{
						smallestSolve = node.visitedAt;
						finalPosX = xpos + searchArea;
						finalPosY = searchY;
					}
				}
			}

			if (smallestSolve != -1)
			{
				route = traceBack(grid.getIndex(finalPosX, finalPosY));
				break;
			}
		}

		return route;
	}
}
