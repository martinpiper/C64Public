package MazeGlue;

import Solver.Grid;
import Solver.Solve;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

/**
 * Common maze related testing routines
 */
public class MazeCommon
{
	protected String inputMaze;
	protected int startPosX, startPosY;
	protected int targetPosX, targetPosY;
	protected int width, height;


	protected Grid grid;
	protected Solve solve;
	protected int iterations;
	protected String solved;
	protected boolean blockAtFrame;

	public void a_maze(String arg1) throws Throwable {throw new Exception("Should not ever be called");};

	public void c64RunStep() throws Throwable
	{}

	public void initMaze(String arg1) throws Throwable
	{
		inputMaze = arg1.replaceAll("\\r?\\n","");
		String[] lines = arg1.split("\\r?\\n");
		width = lines[0].length();
		height = lines.length;

		grid = new Grid(width, height, inputMaze, blockAtFrame);
	}

	public void initMazePositions() throws Throwable
	{
		int pos = inputMaze.indexOf("@");
		assertThat(pos, is(not(-1)));
		startPosX = pos % width;
		startPosY = pos / width;
		pos = inputMaze.indexOf("X");
		assertThat(pos, is(not(-1)));
		targetPosX = pos % width;
		targetPosY = pos / width;

		solve = new Solve();
		solve.grid = grid;
		solve.indexEarlyOut = grid.getIndex(targetPosX, targetPosY);
		solve.setStartPosition(grid.getIndex(startPosX, startPosY));
	}

	public void setNewTargetPosition(int x , int y)
	{
		targetPosX = x;
		targetPosY = y;
		solve.indexEarlyOut = grid.getIndex(targetPosX, targetPosY);
	}

	public void setNewStartPosition(int x , int y)
	{
		startPosX = x;
		startPosY = y;
		solve.setStartPosition(grid.getIndex(startPosX, startPosY));
	}

	protected void runSteps() throws Throwable
	{
		iterations = 0;
		while(solve.step())
		{
			iterations++;
//			System.out.printf("iter %d frame %d\n",iterations , solve.currentFrame);

			c64RunStep();
		}

		System.out.printf("End iter %d frame %d max depth\n", iterations, solve.currentFrame, solve.maxIndexSize);
		System.out.println(grid.dump());

		solved = solve.traceBack(grid.getIndex(targetPosX, targetPosY));
	}

	protected void doBlockAtFrame(String arg1) throws Throwable
	{
		blockAtFrame = true;
		a_maze(arg1);
	}
}
