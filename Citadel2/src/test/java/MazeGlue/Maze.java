package MazeGlue;

import cucumber.api.java.en.Given;
import cucumber.api.java.en.Then;
import cucumber.api.java.en.When;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.comparesEqualTo;
import static org.hamcrest.Matchers.isEmptyString;

/**
 *
 */
public class Maze extends MazeCommon
{
	@Given("^a maze:$")
	public void a_maze(String arg1) throws Throwable
	{
		initMaze(arg1);
	}

	@Given("^a starting point @ and a target point X$")
	public void a_starting_point_and_a_target_point_X() throws Throwable
	{
		initMazePositions();
	}

	@When("^the unit plots a route$")
	public void the_unit_plots_a_route() throws Throwable
	{
		runSteps();
	}

	@Then("^the cheapest route is (.*?)$")
	public void the_cheapest_route_is_R(String arg1) throws Throwable
	{
		assertThat(solved, comparesEqualTo(arg1));
	}

	@Then("^there is no route$")
	public void there_is_no_route() throws Throwable
	{
		assertThat(solved, isEmptyString());
	}

	@Then("^the closest route is (.*?)$")
	public void the_closest_route_is(String arg1) throws Throwable
	{
		solved = solve.traceBackClosest(targetPosX, targetPosY);
		assertThat(solved, comparesEqualTo(arg1));
	}

	@Given("^a maze where numbers signify increasing cost to travel that tile:$")
	public void a_maze_where_numbers_signify_increasing_cost_to_travel_that_tile(String arg1) throws Throwable
	{
		// The default implementation is to use the input numbers as travel cost
		a_maze(arg1);
	}

	@When("^the route cost is ([+-]?\\d+)$")
	public void the_route_cost_is(int arg1) throws Throwable
	{
		assertThat(solve.lastSolveCost, comparesEqualTo(arg1));
	}

	@Given("^a maze where numbers signify a block at that time index:$")
	public void a_maze_where_numbers_signify_a_block_at_that_time_index(String arg1) throws Throwable
	{
		doBlockAtFrame(arg1);
	}

	@Then("^the frames seen are:$")
	public void the_frames_seen_are(String arg1) throws Throwable
	{
		// Write code here that turns the phrase above into concrete actions
		// MPi: TODO: Implement the frame checking?
//		throw new PendingException();
	}

	@Then("^the iterations are (\\d+)$")
	public void the_iterations_are(int arg1) throws Throwable
	{
		assertThat(iterations, comparesEqualTo(arg1));
	}

	@Then("^the frame count is (\\d+)$")
	public void the_frame_count_is(int arg1) throws Throwable
	{
		assertThat(solve.currentFrame, comparesEqualTo(arg1));
	}

	@Then("^the max depth is (\\d+)$")
	public void the_max_depth_is(int arg1) throws Throwable
	{
		assertThat(solve.maxIndexSize, comparesEqualTo(arg1));
	}

	@Given("^I set a new starting point (\\d+) , (\\d+)$")
	public void iSetANewStartingPoint(int x, int y) throws Throwable
	{
		setNewStartPosition(x,y);
	}

	@Given("^I set a new target point (\\d+) , (\\d+)$")
	public void iSetANewTargetPoint(int x, int y) throws Throwable
	{
		setNewTargetPosition(x,y);
	}
}
