package Solver;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;

/**
 * Creates a link list for objects that relies on the IndexList behaviour
 */
public class FIFO<T>
{
	private T[] slots;
	private int numElements;
	private int startIndex , endIndex;

	public FIFO(int numElements)
	{
		this.numElements = numElements;
		clear();
	}

	public void clear()
	{
		startIndex = 0;
		endIndex = 0;
		slots = (T[]) new Object[numElements];
	}

	public int size()
	{
		if (endIndex >= startIndex)
		{
			return endIndex - startIndex;
		}
		return (slots.length - startIndex) + endIndex;
	}

	public boolean isEmpty()
	{
		return startIndex == endIndex;
	}

	public void add(T object)
	{
		slots[endIndex] = object;
		endIndex++;
		if (endIndex >= slots.length)
		{
			endIndex = 0;
		}
	}

	public T remove()
	{
		assertThat(isEmpty() , is(false));

		T toReturn = slots[startIndex];
		slots[startIndex] = null;
		startIndex++;
		if (startIndex >= slots.length)
		{
			startIndex = 0;
		}
		return toReturn;
	}
}
