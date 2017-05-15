package Solver;

import org.hamcrest.MatcherAssert;
import org.hamcrest.Matchers;

public class SolveCommon
{
	Grid grid;
	Solve solve;
	int iterations;

	public SolveCommon()
	{
	}

	void runSteps(String expectedResult)
	{
		iterations = 0;
		while (solve.step())
		{
			iterations++;
//			System.out.printf("iter %d frame %d\n",iterations , solve.currentFrame);
		}

		System.out.printf("End iter %d frame %d max depth %d\n", iterations, solve.currentFrame, solve.maxIndexSize);
		System.out.println(grid.dump());
		MatcherAssert.assertThat(expectedResult, Matchers.comparesEqualTo(grid.dump().replace("\n", "")));
	}
}