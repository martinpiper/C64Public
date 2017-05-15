package MazeGlue;

import cucumber.api.java.en.Given;
import cucumber.api.java.en.Then;
import cucumber.api.java.en.When;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

/**
 * Created by Martin on 08/02/2016.
 */
public class Memory
{
	public int[] buffer;
	public int lastAllocatedAddress;

	protected boolean checkHeap(int arg1)
	{
		boolean ret = false;
		int i = 0;
		while (i < buffer.length)
		{
			if ((i + 2) == arg1)
			{
				ret = true;
			}
			assertThat(buffer[i], anyOf(equalTo(1), equalTo(2)));
			assertThat(buffer[i + 1], is(lessThanOrEqualTo(254)));
			assertThat(buffer[i + 1], is(greaterThanOrEqualTo(0)));
			i += buffer[i + 1] + 2;
		}

		assertThat(i, is(equalTo(buffer.length)));
		return ret;
	}

	@Given("^I have (\\d+) bytes of heap memory$")
	public void i_have_bytes_of_heap_memory(int arg1) throws Throwable
	{
		buffer = new int[arg1];
		int i = 0;
		while (i < buffer.length)
		{
			buffer[i] = 1;
			buffer[i+1] = 254;

			i += 256;
		}
	}

	@Then("^there is a free memory block at (\\d+) of (\\d+) bytes$")
	public void there_is_a_free_memory_block_at_of_bytes(int arg1, int arg2) throws Throwable
	{
		assertThat(checkHeap(arg1), is(true));

		assertThat(buffer[arg1 - 2], is(1));
		assertThat(buffer[arg1 - 1], is(arg2));
	}

	@When("^I allocate (\\d+) bytes$")
	public void i_allocate_bytes(int arg1) throws Throwable
	{
		lastAllocatedAddress = -1;
		checkHeap(-1);

		int i = 0;
		while (i < buffer.length)
		{
			if (1 == buffer[i] && arg1 <= buffer[i + 1])
			{
				int left = buffer[i + 1] - arg1 - 2;

				// Avoids blocks getting too small and fragmented
				if (left >= 8)
				{
					// Split the block
					buffer[i + 1] = arg1;
					buffer[i + 2 + arg1] = 1;
					buffer[i + 2 + arg1 + 1] = left;
				}

				buffer[i] = 2;

				lastAllocatedAddress = i + 2;
				assertThat(checkHeap(lastAllocatedAddress), is(true));
				return;
			}
			i += buffer[i + 1] + 2;
		}

		throw new Exception("Failed to allocate " + arg1);
	}

	@Then("^I get a block at (\\d+)$")
	public void i_get_a_block_at(int arg1) throws Throwable
	{
		assertThat(lastAllocatedAddress, is(equalTo(arg1)));
	}

	@Then("^there is a used memory block at (\\d+) of (\\d+) bytes$")
	public void there_is_a_used_memory_block_at_of_bytes(int arg1, int arg2) throws Throwable
	{
		assertThat(checkHeap(arg1), is(true));

		assertThat(buffer[arg1 - 2], is(2));
		assertThat(buffer[arg1 - 1], is(arg2));
	}

	@When("^I free the block at (\\d+)$")
	public void i_free_the_block_at(int arg1) throws Throwable
	{
		assertThat(checkHeap(arg1), is(true));

		assertThat(buffer[arg1 - 2], is(2));

		buffer[arg1 - 2] = 1;

		assertThat(checkHeap(arg1), is(true));
		assertThat(buffer[arg1 - 2], is(1));
	}

	@When("^I run a block sweep$")
	public void iRunABlockSweep() throws Throwable
	{
		checkHeap(-1);

		int i = 0;
		while (i < buffer.length)
		{
			if (1 == buffer[i] )
			{
				int nextBlock = i + buffer[i + 1] + 2;
				if (nextBlock < buffer.length)
				{
					// Try merging with the next free buffer if it doesn't get too large
					if (1 == buffer[nextBlock])
					{
						int total = buffer[i + 1] + buffer[nextBlock + 1];
						if (total <= 252) // Max block size - 2
						{
							total += 2;
							buffer[i + 1] = total;
						}
					}
				}
			}
			i += buffer[i + 1] + 2;
		}

		checkHeap(-1);
	}
}
