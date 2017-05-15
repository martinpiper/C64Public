package MazeGlue6502;

import MazeGlue.MazeCommon;
import Solver.Node;
import cucumber.api.PendingException;
import cucumber.api.Scenario;
import cucumber.api.java.Before;
import cucumber.api.java.en.Given;
import cucumber.api.java.en.Then;
import cucumber.api.java.en.When;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

/**
 *
 */
public class Maze extends MazeCommon
{
	TestGlue.Glue c64 = new TestGlue.Glue();
	int c64iterations;

	@Before
	public void BeforeHook(Scenario scenario) { c64.BeforeHook(scenario); }

	// Validate the C64 code with the data from the Java code execution for this current test
	protected void validateGridData() throws Throwable
	{
		System.out.println("validateGridData c64iterations " + c64iterations);

		System.out.println("Checking index");

		int mazeAddr = c64.valueToInt("MazeNodes");
		int index = 0;
		for(Character theChar : inputMaze.toCharArray())
		{
//			System.out.println(" " + index);
			Node node = grid.getNode(index);

			c64.i_expect_to_see_equal(Integer.toString(mazeAddr + c64.valueToInt("Node_isBlocked")), "low(" + (node.isBlocked?"1":"0") + ")");
			c64.i_expect_to_see_equal(Integer.toString(mazeAddr + c64.valueToInt("Node_travelCost")), "low(" + Integer.toString(node.travelCost) + ")");
			c64.i_expect_to_see_equal(Integer.toString(mazeAddr + c64.valueToInt("Node_blockedAtFrame")), "low(" + Integer.toString(node.blockedAtFrame) + ")");
			c64.i_expect_to_see_equal(Integer.toString(mazeAddr + c64.valueToInt("Node_blockedBeyond")), "low(" + Integer.toString(node.blockedBeyond) + ")");
			c64.i_expect_to_see_equal(Integer.toString(mazeAddr + c64.valueToInt("Node_aversion")), "low(" + Integer.toString(node.aversion) + ")");
			c64.i_expect_to_see_equal(Integer.toString(mazeAddr + c64.valueToInt("Node_visitedAt")), "low(" + Integer.toString(node.visitedAt) + ")");
			c64.i_expect_to_see_equal(Integer.toString(mazeAddr + c64.valueToInt("Node_currentSolveID")), "low(" + Integer.toString(node.currentSolveID) + ")");
			c64.i_expect_to_see_equal(Integer.toString(mazeAddr + c64.valueToInt("Node_reachedVia")), "low(" + Integer.toString(Node.getC64Direction(node.reachedVia)) + ")");

			mazeAddr += c64.valueToInt("Node__size");
			index++;
		}

		System.out.println("Finished checking");
	}

	@Given("^a maze:$")
	public void a_maze(String arg1) throws Throwable
	{
		initMaze(arg1);

		c64.i_have_a_simple_6502_system();
		c64.i_create_file_with("t.a",
				"!sal\n" +
				"!sl \"t.lbl\"\n" +
				"*=$400\n" +
				"Maze !tx \"" + inputMaze + "\"\n" +
				"MazeEnd = *\n" +
				"TraceBackResultBuffer !fill 256,0\n" +
				"MazeWidth = " + width + "\n" +
				"MazeHeight = " + height + "\n" +
				"!source \"asm/MazeSolve.a\"\n"
		);
		c64.i_run_the_command_line("..\\acme.exe -o t.prg -f cbm t.a");
		c64.i_load_prg("t.prg");
		c64.i_load_labels("t.lbl");
		c64.i_set_register_to("x" , "0");
		if (blockAtFrame)
		{
			c64.i_set_register_to("x" , "1");
		}
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("MazeSolve_init", Integer.toString(63 * width * height));	// Approximate instructions per map block
		// Validate the work that MazeSolve_init has done for the Node array from the input map data and blockAtFrame
		validateGridData();
	}

	@Given("^a starting point @ and a target point X$")
	public void a_starting_point_and_a_target_point_X() throws Throwable
	{
		initMazePositions();

		// Setup start positions
		int addr = c64.valueToInt("MazeNodes + (Node__size * ((MazeWidth * "+startPosY+") + "+startPosX+"))");
		c64.i_set_register_to("a" , "low("+addr+")");
		c64.i_set_register_to("x" , "hi ("+addr+")");
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("MazeSolve_setStartPosition","100");
		c64.i_expect_to_see_equal(Integer.toString(addr + c64.valueToInt("Node_visitedAt")), "0");

		addr = c64.valueToInt("MazeNodes + (Node__size * ((MazeWidth * "+targetPosY+") + "+targetPosX+"))");
		c64.i_set_register_to("a" , "low("+addr+")");
		c64.i_set_register_to("x" , "hi ("+addr+")");
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("MazeSolve_setIndexEarlyOut","10");

		validateInternalState();
		validateGridData();
	}

	public void validateInternalState() throws Throwable
	{
		System.out.println("validateInternalState");
		System.out.println("anyWorkDone="+solve.anyWorkDone);
		System.out.println("currentFrame="+solve.currentFrame);
		System.out.println("currentSolveID="+solve.currentSolveID);
		System.out.println("earlyOutHit="+solve.earlyOutHit);
		System.out.println("nextSmallestFrame="+solve.nextSmallestFrame);
		System.out.println("incrementCounter="+solve.incrementCounter);
		System.out.println("maxIndexSize="+solve.maxIndexSize);

		c64.i_hex_dump_memory_between_$c_and_$c("MazeSolve_anyWorkDone","MazeSolve_maxIndexSize");

		c64.i_expect_to_see_equal(Integer.toString(c64.valueToInt("MazeSolve_anyWorkDone")), "low(" + (solve.anyWorkDone?"1":"0") + ")");
		c64.i_expect_to_see_equal(Integer.toString(c64.valueToInt("MazeSolve_currentFrame")), "low(" + solve.currentFrame + ")");
		c64.i_expect_to_see_equal(Integer.toString(c64.valueToInt("MazeSolve_currentSolveID")), "low(" + solve.currentSolveID + ")");
		c64.i_expect_to_see_equal(Integer.toString(c64.valueToInt("MazeSolve_earlyOutHit")), "low(" + (solve.earlyOutHit?"1":"0") + ")");
		c64.i_expect_to_see_equal(Integer.toString(c64.valueToInt("MazeSolve_nextSmallestFrame")), "low(" + solve.nextSmallestFrame + ")");
		c64.i_expect_to_see_equal(Integer.toString(c64.valueToInt("MazeSolve_incrementCounter")), "low(" + solve.incrementCounter + ")");
		c64.i_expect_to_see_equal(Integer.toString(c64.valueToInt("MazeSolve_maxIndexSize")), "low(" + solve.maxIndexSize + ")");
	}

	public void c64RunStep() throws Throwable
	{
		System.out.println("c64RunStep c64iterations " + c64iterations);

		// This is run after the java code runs a step, so execute the C64 function
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("MazeSolve_step","300");
		// Since this is called by runSteps() solve.step() then carry must be set
		c64.i_expect_register_contain("st" , "stC");

		c64iterations++;

		assertThat(iterations , is(equalTo(c64iterations)));
		validateGridData();
		validateInternalState();
	}

	@When("^the unit plots a route$")
	public void the_unit_plots_a_route() throws Throwable
	{
		validateInternalState();

		c64iterations = 0;
		runSteps();

		// The final MazeSolve_step is executed now because runSteps() solve.step() returned false
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("MazeSolve_step","200");
		c64.i_expect_register_exclude("st", "stC");
		validateGridData();
		validateInternalState();

		c64.i_set_register_to("a" , "low(TraceBackResultBuffer)");
		c64.i_set_register_to("x" , "hi (TraceBackResultBuffer)");
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("MazeSolve_setTraceBackResultBuffer","10");

		int addr = c64.valueToInt("MazeNodes + (Node__size * ((MazeWidth * "+targetPosY+") + "+targetPosX+"))");
		c64.i_set_register_to("a" , "low("+addr+")");
		c64.i_set_register_to("x" , "hi ("+addr+")");
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("MazeSolve_startTraceBack","20");

		int j = 0;
		for (int i = solved.length()-1 ; i>= 0 ; i--)
		{
			c64.i_execute_the_procedure_at_for_no_more_than_instructions("MazeSolve_traceBackStep","36");
			c64.i_expect_register_contain("st" , "stC");
			c64.i_expect_to_see_equal(Integer.toString(c64.valueToInt("workTraceBackBuffer") + j) , Integer.toString(solved.charAt(i)));
			j++;
		}
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("MazeSolve_traceBackStep",Integer.toString(30 + (solved.length() * 15) ));
		c64.i_expect_register_exclude("st" , "stC");
		c64.i_expect_register_contain("y" , Integer.toString(solved.length()+1));

		c64.i_hex_dump_memory_between_$c_and_$c("workTraceBackBuffer" , "workTraceBackBuffer+128");
		c64.i_hex_dump_memory_between_$c_and_$c("TraceBackResultBuffer" , "TraceBackResultBuffer+128");

		validateGridData();
		validateInternalState();
		c64.i_expect_to_see_equal(Integer.toString(c64.valueToInt("MazeSolve_lastSolveCost")), "low(" + solve.lastSolveCost + ")");	// Only updated during Solve.traceBack()

		validateSolved(solved);
	}

	protected void validateSolved(String theRoute) throws Throwable
	{
		int addr = c64.valueToInt("TraceBackResultBuffer");
		for (char theChar : theRoute.toCharArray())
		{
			c64.i_expect_to_see_equal(Integer.toString(addr) , Integer.toString((int)theChar));
			addr++;
		}
	}

	@Then("^the cheapest route is (.*?)$")
	public void the_cheapest_route_is_R(String arg1) throws Throwable
	{
		assertThat(solved, comparesEqualTo(arg1));

		validateSolved(arg1);
	}

	@Then("^there is no route$")
	public void there_is_no_route() throws Throwable
	{
		assertThat(solved, isEmptyString());

		int addr = c64.valueToInt("TraceBackResultBuffer");
		c64.i_expect_to_see_equal(Integer.toString(addr) , "0");
	}

	@Then("^the closest route is (.*?)$")
	public void the_closest_route_is(String arg1) throws Throwable
	{
		solved = solve.traceBackClosest(targetPosX, targetPosY);
		assertThat(solved, comparesEqualTo(arg1));

		throw new PendingException();
	}

	@Given("^a maze where numbers signify increasing cost to travel that tile:$")
	public void a_maze_where_numbers_signify_increasing_cost_to_travel_that_tile(String arg1) throws Throwable
	{
		a_maze(arg1);
	}

	@When("^the route cost is ([+-]?\\d+)$")
	public void the_route_cost_is(int arg1) throws Throwable
	{
		assertThat(solve.lastSolveCost, comparesEqualTo(arg1));

		c64.i_expect_to_see_equal(Integer.toString(c64.valueToInt("MazeSolve_lastSolveCost")), "low(" + arg1 + ")");
	}

	@Given("^a maze where numbers signify a block at that time index:$")
	public void a_maze_where_numbers_signify_a_block_at_that_time_index(String arg1) throws Throwable
	{
		doBlockAtFrame(arg1);
	}

	@Then("^the frames seen are:$")
	public void the_frames_seen_are(String arg1) throws Throwable
	{
//		throw new PendingException();
	}

	@Then("^the iterations are (\\d+)$")
	public void the_iterations_are(int arg1) throws Throwable
	{
		assertThat(iterations, comparesEqualTo(arg1));

		assertThat(c64iterations, comparesEqualTo(arg1));
	}

	@Then("^the frame count is (\\d+)$")
	public void the_frame_count_is(int arg1) throws Throwable
	{
		assertThat(solve.currentFrame, comparesEqualTo(arg1));

		c64.i_expect_to_see_equal(Integer.toString(c64.valueToInt("MazeSolve_currentFrame")), "low(" + arg1 + ")");
	}

	@Then("^the max depth is (\\d+)$")
	public void the_max_depth_is(int arg1) throws Throwable
	{
		assertThat(solve.maxIndexSize, comparesEqualTo(arg1));

		c64.i_expect_to_see_equal(Integer.toString(c64.valueToInt("MazeSolve_maxIndexSize")), "low(" + arg1 + ")");
	}

	@Given("^I set a new starting point (\\d+) , (\\d+)$")
	public void iSetANewStartingPoint(int x, int y) throws Throwable
	{
		setNewStartPosition(x,y);

		// Setup start positions
		int addr = c64.valueToInt("MazeNodes + (Node__size * ((MazeWidth * "+startPosY+") + "+startPosX+"))");
		c64.i_set_register_to("a" , "low("+addr+")");
		c64.i_set_register_to("x" , "hi ("+addr+")");
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("MazeSolve_setStartPosition","100");
		c64.i_expect_to_see_equal(Integer.toString(addr + c64.valueToInt("Node_visitedAt")), "0");

		validateInternalState();
		validateGridData();
	}

	@Given("^I set a new target point (\\d+) , (\\d+)$")
	public void iSetANewTargetPoint(int x, int y) throws Throwable
	{
		setNewTargetPosition(x,y);

		int addr = c64.valueToInt("MazeNodes + (Node__size * ((MazeWidth * "+targetPosY+") + "+targetPosX+"))");
		c64.i_set_register_to("a" , "low("+addr+")");
		c64.i_set_register_to("x" , "hi ("+addr+")");
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("MazeSolve_setIndexEarlyOut","10");

		validateInternalState();
		validateGridData();
	}
}
