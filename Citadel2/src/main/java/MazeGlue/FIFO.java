package MazeGlue;

import cucumber.api.java.en.Given;
import cucumber.api.java.en.Then;
import cucumber.api.java.en.When;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.equalTo;
import static org.hamcrest.Matchers.is;

/**
 *
 */
public class FIFO
{
	Solver.FIFO<Integer> list;

	@Given("^I have a new FIFO with maximum (\\d+) elements$")
	public void i_have_a_new_SlotLinkList_with_maximum_elements(int arg1) throws Throwable {
		list = new Solver.FIFO<Integer>(arg1);
	}

	@Then("^I expect to see the FIFO is empty$")
	public void i_expect_to_see_the_SlotLinkList_is_empty() throws Throwable {
		assertThat(list.isEmpty() , is(equalTo(true)));
	}

	@When("^I push the value (\\d+) to the FIFO$")
	public void i_push_the_value_to_the_SlotLinkList(int arg1) throws Throwable {
		list.add(arg1);
	}

	@Then("^I expect to see the FIFO size = (\\d+)$")
	public void i_expect_to_see_the_SlotLinkList_size(int arg1) throws Throwable {
		assertThat(list.size(), is(equalTo(arg1)));
	}

	@Then("^I expect to see the FIFO is not empty$")
	public void i_expect_to_see_the_SlotLinkList_is_not_empty() throws Throwable {
		assertThat(list.isEmpty() , is(equalTo(false)));
	}

	@When("^I pop the value it equals (\\d+)$")
	public void i_pop_the_value_it_equals(int arg1) throws Throwable {
		assertThat(list.remove() , is(equalTo(arg1)));
	}
}
