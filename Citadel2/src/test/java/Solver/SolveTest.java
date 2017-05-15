package Solver;

import org.junit.After;
import org.junit.Before;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

public class SolveTest extends SolveCommon
{
	@Before
	public void setUp() throws Exception
	{
		String map =
				"******" +
				"* * **" +
				"*    *" +
				"* ** *" +
				"* *  *" +
				"* * **" +
				"* *  *" +
				"* ** *" +
				"*    *" +
				"******";
		grid = new Grid(6, 10, map, false);
		solve = new Solve();
		solve.grid = grid;
		solve.indexEarlyOut = grid.getIndex(3,1);
	}

	@After
	public void tearDown() throws Exception
	{

	}

	@org.junit.Test
	public void testStep1() throws Exception
	{
		// Simulate some dead things here so the nodes are avoided with a factor of 4
		grid.getNode(grid.getIndex(1, 4)).aversion = 4;
		grid.getNode(grid.getIndex(1, 3)).aversion = 4;

		// Start from here
		solve.setStartPosition(grid.getIndex(1, 8));

		runSteps(
				"##################" +
				"###   ###  d######" +
				"###     d  c  b###" +
				"###  d######  a###" +
				"###  8###  8  9###" +
				"###  3###  7######" +
				"###  2###  6  5###" +
				"###  1######  4###" +
				"###  0  1  2  3###" +
				"##################");
		assertThat(iterations, equalTo(23));
		assertThat(solve.currentFrame, equalTo(12));
		assertThat(solve.traceBack(solve.indexEarlyOut), comparesEqualTo("RRRUULUURUULU"));
		assertThat(solve.maxIndexSize, equalTo(2));
	}

	@org.junit.Test
	public void testStep2() throws Exception
	{
		// Simulate something moving along this path
		grid.getNode(grid.getIndex(2, 2)).blockedAtFrame = 8;
		grid.getNode(grid.getIndex(1, 2)).blockedAtFrame = 6;
		grid.getNode(grid.getIndex(1, 3)).blockedAtFrame = 4;

		// Start from here
		solve.setStartPosition(grid.getIndex(1, 8));

		runSteps(
				"##################" +
				"###  9###  c######" +
				"###  8  a  b  b###" +
				"###  6######  a###" +
				"###  4###  8  9###" +
				"###  3###  7######" +
				"###  2###  6  5###" +
				"###  1######  4###" +
				"###  0  1  2  3###" +
				"##################");
		assertThat(iterations, equalTo(23));
		assertThat(solve.currentFrame, equalTo(11));
		assertThat(solve.traceBack(solve.indexEarlyOut), comparesEqualTo("UUUUUURRU"));
		assertThat(solve.maxIndexSize, equalTo(3));
	}

	@org.junit.Test
	public void testStep3() throws Exception {
		// Simulate something stopping after a certain number of frames
		grid.getNode(grid.getIndex(1, 3)).blockedBeyond = 3;

		// Start from here
		solve.setStartPosition(grid.getIndex(1, 8));

		runSteps(
				"##################" +
				"###   ###  d######" +
				"###     d  c  b###" +
				"###   ######  a###" +
				"###  4###  8  9###" +
				"###  3###  7######" +
				"###  2###  6  5###" +
				"###  1######  4###" +
				"###  0  1  2  3###" +
				"##################");
		assertThat(iterations, equalTo(16));
		assertThat(solve.currentFrame, equalTo(12));
		assertThat(solve.traceBack(solve.indexEarlyOut), comparesEqualTo("RRRUULUURUULU"));
		assertThat(solve.maxIndexSize, equalTo(2));
	}

	@org.junit.Test
	public void testStep4() throws Exception
	{
		// Solve the maze top down, not bottom up

		// Start from here
		solve.setStartPosition(grid.getIndex(1, 6));

		solve.indexEarlyOut = grid.getIndex(4, 6);

		runSteps(
				"##################" +
				"###  5###  7######" +
				"###  4  5  6  7###" +
				"###  3######   ###" +
				"###  2###      ###" +
				"###  1###   ######" +
				"###  0###     7###" +
				"###  1######  6###" +
				"###  2  3  4  5###" +
				"##################");
		assertThat(iterations, equalTo(13));
		assertThat(solve.currentFrame, equalTo(6));
		assertThat(solve.traceBack(solve.indexEarlyOut), comparesEqualTo("DDRRRUU"));
		assertThat(solve.maxIndexSize, equalTo(3));
	}

	@org.junit.Test
	public void testStep5() throws Exception
	{
		// Solve the maze top down right to left, not bottom up left to right

		// Start from here
		solve.setStartPosition(grid.getIndex(4, 6));

		solve.indexEarlyOut = grid.getIndex(1, 6);

		runSteps(
				"##################" +
				"###   ###   ######" +
				"###        7  6###" +
				"###   ######  5###" +
				"###   ###  3  4###" +
				"###   ###  2######" +
				"###  7###  1  0###" +
				"###  6######  1###" +
				"###  5  4  3  2###" +
				"##################");
		assertThat(iterations, equalTo(12));
		assertThat(solve.currentFrame, equalTo(6));
		assertThat(solve.traceBack(solve.indexEarlyOut), comparesEqualTo("DDLLLUU"));
		assertThat(solve.maxIndexSize, equalTo(2));
	}
}
