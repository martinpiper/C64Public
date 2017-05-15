package Solver;

import org.junit.After;
import org.junit.Before;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.equalTo;

public class SolveTest2 extends SolveCommon
{
	@Before
	public void setUp() throws Exception
	{
		String map =
				"**********" +
				"* *** ** *" +
				"*   **   *" +
				"****   ***" +
				"*    **  *" +
				"* * * *  *" +
				"* *   *  *" +
				"* ** **  *" +
				"*        *" +
				"**********";
		grid = new Grid(10, 10, map, false);
		solve = new Solve();
		solve.grid = grid;
		solve.indexEarlyOut = grid.getIndex(8, 1);
	}

	@After
	public void tearDown() throws Exception
	{

	}

	@org.junit.Test
	public void testStep1() throws Exception
	{
		// Start from here
		solve.setStartPosition(grid.getIndex(1, 8));

		runSteps(
				"##############################" +
				"###   #########   ######  e###" +
				"###         ######  b  c  d###" +
				"############  8  9  a#########" +
				"###  4  5  6  7######  a  b###" +
				"###  3###  7###  7###  9  a###" +
				"###  2###  6  5  6###  8  9###" +
				"###  1######  4######  7  8###" +
				"###  0  1  2  3  4  5  6  7###" +
				"##############################");
		assertThat(iterations, equalTo(34));
		assertThat(solve.currentFrame, equalTo(13));
		assertThat(solve.maxIndexSize, equalTo(5));
	}
}
