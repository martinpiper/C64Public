package MazeGlue6502;

import cucumber.api.java.en.Given;
import cucumber.api.java.en.Then;
import cucumber.api.java.en.When;

/**
 * Created by Martin on 09/02/2016.
 */
public class Memory
{
	TestGlue.Glue c64 = new TestGlue.Glue();
	MazeGlue.Memory memory = new MazeGlue.Memory();

	@cucumber.api.java.Before
	public void BeforeHook(cucumber.api.Scenario scenario) { c64.BeforeHook(scenario); }

	private void validateMemory() throws Throwable
	{
		System.out.println("Start validateMemory");
		int memoryAddr = c64.valueToInt("DynamicMemoryData");
		c64.i_hex_dump_memory_between_$c_and_$c("DynamicMemoryData" , "DynamicMemoryData+512");
		for (int i = 0 ; i < memory.buffer.length ; i++)
		{
			c64.i_expect_to_see_equal(Integer.toString(memoryAddr + i), Integer.toString(memory.buffer[i]));
		}
		System.out.println("End validateMemory");
	}

	@Given("^I have (\\d+) bytes of heap memory$")
	public void i_have_bytes_of_heap_memory(int arg1) throws Throwable
	{
		memory.i_have_bytes_of_heap_memory(arg1);

		c64.i_have_a_simple_6502_system();
		c64.i_create_file_with("t.a",
				"!sal\n" +
				"!sl \"t.lbl\"\n" +
				"*=$400\n" +
				"DynamicMemorySize = " + arg1 + "\n" +
				"DynamicMemory_ZP_lo = $2\n" +
				"DynamicMemory_ZP_hi = $3\n" +
				"!source \"asm/DynamicMemory.a\"\n" +
				"DynamicMemoryData !fill DynamicMemorySize , 0\n"
		);
		c64.i_run_the_command_line("..\\acme.exe -o t.prg -f cbm t.a");
		c64.i_load_prg("t.prg");
		c64.i_load_labels("t.lbl");
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("DynamicMemory_Init", Integer.toString( 20 + ((arg1 * 40)/254)));

		validateMemory();
	}

	@Then("^there is a free memory block at (\\d+) of (\\d+) bytes$")
	public void there_is_a_free_memory_block_at_of_bytes(int arg1, int arg2) throws Throwable
	{
		memory.there_is_a_free_memory_block_at_of_bytes(arg1 , arg2);
		validateMemory();
	}

	@When("^I allocate (\\d+) bytes$")
	public void i_allocate_bytes(int arg1) throws Throwable
	{
		memory.i_allocate_bytes(arg1);
		c64.i_set_register_to("a" , Integer.toString(arg1));
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("DynamicMemory_AllocateBytes", "200");
		c64.i_expect_register_exclude("st" , "stC");

		validateMemory();
	}

	@Then("^I get a block at (\\d+)$")
	public void i_get_a_block_at(int arg1) throws Throwable
	{
		memory.i_get_a_block_at(arg1);

		int memoryAddr = c64.valueToInt("DynamicMemoryData");
		c64.i_expect_to_see_equal(Integer.toString(c64.valueToInt("DynamicMemory_ZP_lo")), "low(" + (memoryAddr + memory.lastAllocatedAddress) + ")");
		c64.i_expect_to_see_equal(Integer.toString(c64.valueToInt("DynamicMemory_ZP_hi")), "hi(" + (memoryAddr + memory.lastAllocatedAddress) + ")");
	}

	@Then("^there is a used memory block at (\\d+) of (\\d+) bytes$")
	public void there_is_a_used_memory_block_at_of_bytes(int arg1, int arg2) throws Throwable
	{
		memory.there_is_a_used_memory_block_at_of_bytes(arg1, arg2);
		validateMemory();
	}

	@When("^I free the block at (\\d+)$")
	public void i_free_the_block_at(int arg1) throws Throwable
	{
		memory.i_free_the_block_at(arg1);

		int memoryAddr = c64.valueToInt("DynamicMemoryData") + arg1;
		c64.i_set_register_to("a" , "low(" + memoryAddr + ")");
		c64.i_set_register_to("x" , "hi(" + memoryAddr + ")");

		c64.i_execute_the_procedure_at_for_no_more_than_instructions("DynamicMemory_FreeBytes", "12");

		validateMemory();
	}

	@When("^I run a block sweep$")
	public void iRunABlockSweep() throws Throwable
	{
		memory.iRunABlockSweep();
		c64.i_execute_the_procedure_at_for_no_more_than_instructions("DynamicMemory_BlockSweep", Integer.toString( 20 + ((memory.buffer.length * 20)/8)));

		validateMemory();
	}
}
