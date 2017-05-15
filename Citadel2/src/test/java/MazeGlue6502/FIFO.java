package MazeGlue6502;

import cucumber.api.Scenario;
import cucumber.api.java.Before;
import cucumber.api.java.en.Given;
import cucumber.api.java.en.Then;
import cucumber.api.java.en.When;

/**
 *
 */
public class FIFO
{
	TestGlue.Glue c64 = new TestGlue.Glue();

	@Before
	public void BeforeHook(Scenario scenario) { c64.BeforeHook(scenario); }

	@Given("^I have a new FIFO with maximum (\\d+) elements$")
	public void i_have_a_new_SlotLinkList_with_maximum_elements(String arg1) throws Throwable {
		c64.i_have_a_simple_6502_system();
		c64.i_create_file_with("t.a",
				"!sal\n" +
				"*=$400\n" +
				"FIFOEntries = " + arg1 + "\n" +
				"!source \"asm/FIFO.a\"\n"
		);
		c64.i_run_the_command_line("..\\acme.exe -o t.prg --labeldump t.lbl -f cbm t.a");
		c64.i_load_prg("t.prg");
		c64.i_load_labels("t.lbl");
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("FIFOInit","100");
	}

	@Then("^I expect to see the FIFO is empty$")
	public void i_expect_to_see_the_SlotLinkList_is_empty() throws Throwable {
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("FIFOIsEmpty", "100");
		c64.i_expect_register_contain("ST", "stZ");
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("FIFOSize", "100");
		c64.i_expect_register_equal("A" , "0");
	}

	@When("^I push the value (\\d+) to the FIFO$")
	public void i_push_the_value_to_the_SlotLinkList(String arg1) throws Throwable {
		c64.i_set_register_to("A", "low("+arg1+")");
		c64.i_set_register_to("X", "hi("+arg1+")");
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("FIFOAdd", "100");
	}

	@Then("^I expect to see the FIFO size = (\\d+)$")
	public void i_expect_to_see_the_SlotLinkList_size(String arg1) throws Throwable {
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("FIFOSize", "100");
		c64.i_expect_register_equal("A", arg1);
	}

	@Then("^I expect to see the FIFO is not empty$")
	public void i_expect_to_see_the_SlotLinkList_is_not_empty() throws Throwable {
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("FIFOIsEmpty", "100");
		c64.i_expect_register_exclude("ST", "stZ");
	}

	@When("^I pop the value it equals (\\d+)$")
	public void i_pop_the_value_it_equals(int arg1) throws Throwable {
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("FIFORemove", "100");
		c64.i_expect_register_equal("A", "low(" + arg1 + ")");
		c64.i_expect_register_equal("X", "hi(" + arg1+")");
	}
}
