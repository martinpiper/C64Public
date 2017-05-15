package Solver;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

/**
 *
 */
public class Grid
{
	int age = 0;
	int width = 0;
	int height = 0;

	// Just need linear access for this "grid"
	private Node[] grid;

	public Grid(int width, int height , String walls, boolean blockAtFrame)
	{
		this.width = width;
		this.height = height;
		grid = new Node[width * height];
		assertThat(grid.length, is(equalTo(walls.length())));
		int i;
		for (i = 0; i < grid.length; i++)
		{
			grid[i] = new Node();
			char theChar = walls.charAt(i);
			if (theChar == '@' || theChar == 'X')
			{
				// Do nothing, it's a start or end position
			}
			else if (Character.isDigit(theChar))
			{
				if(blockAtFrame)
				{
					grid[i].blockedAtFrame = theChar - '0';
				}
				else
				{
					grid[i].travelCost = theChar - '0';
				}
			}
			else if (theChar == '*' || theChar == '#')
			{
				grid[i].isBlocked = true;
			}
		}
	}

	private Node getNode(int xPos, int yPos)
	{
		return getNode(xPos + (yPos * width));
	}

	public Node getNode(int index)
	{
		if (index < 0 || index >= grid.length)
		{
			return null;
		}
		return grid[index];
	}

	public String dump()
	{
		String map = "";
		int x, y;
		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				Node node = getNode(x, y);
				if (node.isBlocked)
				{
					map += "###";
				}
				else if (node.visitedAt >= 0)
				{
					String val = Integer.toString(node.visitedAt, 16);
					while (val.length() < 3)
					{
						val = " " + val;
					}
					map += val;
				}
				else
				{
					map += "   ";
				}
			}
			map += "\n";
		}
		return map;
	}

	public int length()
	{
		return grid.length;
	}

	public int getIndex(int xPos, int yPos)
	{
		if (xPos < 0 || xPos >= width || yPos < 0 || yPos >= height)
		{
			return -1;
		}
		return xPos + (yPos*width);
	}
}
