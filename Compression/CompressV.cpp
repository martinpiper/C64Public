#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <utility>
#include <algorithm>
#include <functional>
#include "CompressV.h"
#include "DecompressV.h"

using std::vector;
using std::make_pair;
using std::max;
using std::min;
using std::pair;
using std::string;
using std::vector;
using std::fill;
using std::less;

template <typename V> class CuckooHash;

#define UNINITIALIZED (-1)
#define IS_LMS(i) ((i) > 0 && stype[(i)] && !stype[(i) - 1])

// See: https://github.com/askeksa/Shrinkler
template <typename V>
class CuckooHashIterator
{
	const CuckooHash<V>* table;
	int index;
	CuckooHashIterator(const CuckooHash<V>* table, int index) : table(table), index(index)	{}

	void find()
	{
		while (table->element_array[index].first == CuckooHash<V>::UNUSED) index++;
	}

	friend class CuckooHash<V>;

public:
	pair<int, V>& operator*()
	{
		find();
		return table->element_array[index];
	}

	pair<int, V>* operator->()
	{
		find();
		return &table->element_array[index];
	}

	CuckooHashIterator<V> operator++(int)
	{
		find();
		return CuckooHashIterator<V>(table, index++);
	}

	bool operator!=(const CuckooHashIterator<V>& other)
	{
		return index != other.index;
	}
};

template <typename V>
class CuckooHash
{
public:
	typedef int key_type;
	typedef pair<key_type, V> value_type;
	typedef CuckooHashIterator<V> iterator;
private:
	friend class CuckooHashIterator<V>;

	typedef unsigned hash_type;

	static const key_type UNUSED = 0x80000000;
	static const hash_type HASH1_MUL = 0xF230D3A1;
	static const hash_type HASH2_MUL = 0x8084027F;
	static const int INITIAL_SIZE_LOG = 2;

	value_type* element_array;
	unsigned n_elements:26;
	unsigned hash_shift:6;

	int array_size() const
	{
		return 1 << (sizeof(hash_type) * 8 - hash_shift);
	}

	void init_array()
	{
		int size = array_size();
		element_array = new value_type[size];
		for (int i = 0 ; i < size ; i++)
		{
			element_array[i].first = UNUSED;
			element_array[i].second = V();
		}
	}

	value_type* get_array()
	{
		if (element_array == NULL)
		{
			init_array();
		}
		return element_array;
	}

	void init()
	{
		n_elements = 0;
		hash_shift = sizeof(hash_type) * 8 - INITIAL_SIZE_LOG;
		element_array = NULL;
	}

	void hashes(key_type key, hash_type& hash1, hash_type& hash2) const
	{
		hash_type f = (key << 1) + 1;
		hash1 = (f * HASH1_MUL) >> hash_shift;
		hash2 = (f * HASH2_MUL) >> hash_shift;
	}

	void rehash()
	{
		int old_size = array_size();
		value_type* old_array = get_array();
		n_elements = 0;
		hash_shift--;
		init_array();
		for (int i = 0 ; i < old_size ; i++)
		{
			if (old_array[i].first != UNUSED)
			{
				(*this)[old_array[i].first] = old_array[i].second;
			}
		}
		delete[] old_array;
	}

	void insert(hash_type hash, int key, V value, int n)
	{
		value_type* array = get_array();
		while (array[hash].first != UNUSED)
		{
			if (--n < 0)
			{
				rehash();
				(*this)[key] = value;
				return;
			}
			std::swap(key, array[hash].first);
			std::swap(value, array[hash].second);
			hash_type hash1;
			hash_type hash2;
			hashes(key, hash1, hash2);
			hash ^= hash1 ^ hash2;
		}
		array[hash].first = key;
		array[hash].second = value;
		n_elements++;
	}

public:
	CuckooHash()
	{
		init();
	}

	CuckooHash(const CuckooHash& source)
	{
		// We only use copy for array initialization, so just create an empty map
		init();
	}

	~CuckooHash()
	{
		delete[] element_array;
	}

	void clear()
	{
		delete[] element_array;
		init();
	}

	iterator begin() const
	{
		return CuckooHashIterator<V>(this, 0);
	}

	iterator end() const
	{
		if (element_array == NULL)
		{
			// Empty
			return CuckooHashIterator<V>(this, 0);
		}

		int index = array_size();
		assert(element_array != NULL);
		value_type* array = element_array;
		while (index > 0 && array[index - 1].first == UNUSED) index--;
		return CuckooHashIterator<V>(this, index);
	}

	int size() const
	{
		return n_elements;
	}

	bool empty() const
	{
		return size() == 0;
	}

	int count(int key) const
	{
		if (empty()) return 0;

		hash_type hash1;
		hash_type hash2;
		hashes(key, hash1, hash2);

		assert(element_array != NULL);
		value_type* array = element_array;
		if (array[hash1].first == key || array[hash2].first == key) return 1;
		return 0;
	}

	void erase(int key)
	{
		hash_type hash1;
		hash_type hash2;
		hashes(key, hash1, hash2);

		value_type* array = get_array();
		hash_type hash;
		if (array[hash1].first == key)
		{
			hash = hash1;
		}
		else if (array[hash2].first == key)
		{
			hash = hash2;
		}
		else
		{
			return;
		}
		array[hash].first = UNUSED;
		array[hash].second = V();
		n_elements--;
	}

	V& operator[](int key)
	{
		hash_type hash1;
		hash_type hash2;
		hashes(key, hash1, hash2);

		value_type* array = get_array();
		if (array[hash1].first == key) return array[hash1].second;
		if (array[hash2].first == key) return array[hash2].second;
		if (array[hash1].first == UNUSED)
		{
			array[hash1].first = key;
			array[hash1].second = V();
			n_elements++;
			return array[hash1].second;
		}
		if (array[hash2].first == UNUSED)
		{
			array[hash2].first = key;
			array[hash2].second = V();
			n_elements++;
			return array[hash2].second;
		}
		insert(hash1, key, V(), n_elements);
		return (*this)[key];
	}
};


template <class T>
class Heap
{
	vector<T> elements;
	less<T> compare;

	void swap(int i1, int i2)
	{
		T t1 = elements[i1];
		T t2 = elements[i2];
		elements[i1] = t2;
		elements[i2] = t1;
		t2->_heap_index = i1;
		t1->_heap_index = i2;
	}

	void up(int i)
	{
		while (i > 0)
		{
			int pi = (i-1)/2;
			if (!compare(elements[pi], elements[i])) return;
			swap(i, pi);
			i = pi;
		}
	}

	void down(int i)
	{
		while (i*2+1 < (int) elements.size())
		{
			int ci1 = i*2+1;
			int ci2 = i*2+2;
			int ci = ci2 < (int) elements.size() && compare(elements[ci1], elements[ci2]) ? ci2 : ci1;
			if (!compare(elements[i], elements[ci])) return;
			swap(i, ci);
			i = ci;
		}		
	}

	T remove_index(int i)
	{
		T removed = elements[i];
		T last = elements[elements.size()-1];
		elements[i] = last;
		elements.pop_back();
		last->_heap_index = i;
		down(i);
		return removed;
	}

public:
	Heap() {}

	void insert(T t)
	{
		elements.push_back(t);
		t->_heap_index = elements.size()-1;
		up(elements.size()-1);
	}

	void remove(T t)
	{
		if (contains(t))
		{
			remove_index(t->_heap_index);
		}
	}

	T remove_largest()
	{
		return remove_index(0);
	}

	bool contains(T t)
	{
		return t->_heap_index < (int)elements.size() && elements[t->_heap_index] == t;
	}

	int size()
	{
		return elements.size();
	}

	void clear()
	{
		elements.clear();
	}

};

static void induce(const int *data, int *suffix_array, int length, int alphabet_size, const vector<bool>& stype, const int *buckets, int *bucket_index)
{
	// Induce L suffixes
	for (int b = 0 ; b < alphabet_size ; b++)
	{
		bucket_index[b] = buckets[b];
	}
	for (int s = 0 ; s < length ; s++)
	{
		int index = suffix_array[s];
		if (index > 0 && !stype[index - 1])
		{
			suffix_array[bucket_index[data[index - 1]]++] = index - 1;
		}
	}
	// Induce S suffixes
	for (int b = 0 ; b < alphabet_size ; b++)
	{
		bucket_index[b] = buckets[b + 1];
	}
	for (int s = length - 1 ; s >= 0 ; s--)
	{
		int index = suffix_array[s];
		assert(index != UNINITIALIZED);
		if (index > 0 && stype[index - 1])
		{
			suffix_array[--bucket_index[data[index - 1]]] = index - 1;
		}
	}
}

static bool substrings_equal(const int *data, int i1, int i2, const vector<bool>& stype)
{
	while (data[i1++] == data[i2++])
	{
		if (IS_LMS(i1) && IS_LMS(i2))
		{
			return true;
		}
	}
	return false;
}

// Compute the suffix array of a string over an integer alphabet.
// The last character in the string (the sentinel) must be uniquely smallest in the string.
static void computeSuffixArray(const int *data, int *suffix_array, int length, int alphabet_size)
{
	// Handle empty string
	assert(length >= 1);
	if (length == 1)
	{
		suffix_array[0] = 0;
		return;
	}

	vector<bool> stype(length);
	vector<int> buckets(alphabet_size + 1, 0);
	vector<int> bucket_index(alphabet_size);

	// Compute suffix types and count symbols
	stype[length - 1] = true;
	buckets[data[length - 1]] = 1;
	bool is_s = true;
	int lms_count = 0;
	for (int i = length - 2; i >= 0; i--)
	{
		buckets[data[i]]++;
		if (data[i] > data[i + 1])
		{
			if (is_s) lms_count++;
			is_s = false;
		}
		else if (data[i] < data[i + 1])
		{
			is_s = true;
		}
		stype[i] = is_s;
	}

	// Accumulate bucket sizes
	int l = 0;
	for (int b = 0; b <= alphabet_size; b++)
	{
		int l_next = l + buckets[b];
		buckets[b] = l;
		l = l_next;
	}
	assert(l == length);

	// Put LMS suffixes at the ends of buckets
	fill(&suffix_array[0], &suffix_array[length], UNINITIALIZED);
	for (int b = 0 ; b < alphabet_size ; b++)
	{
		bucket_index[b] = buckets[b + 1];
	}
	for (int i = length - 1; i >= 1; i--)
	{
		if (IS_LMS(i))
		{
			suffix_array[--bucket_index[data[i]]] = i;
		}
	}

	// Induce to sort LMS strings
	induce(data, suffix_array, length, alphabet_size, stype, &buckets[0], &bucket_index[0]);

	// Compact LMS indices at the beginning of the suffix array
	int j = 0;
	for (int s = 0; s < length; s++)
	{
		int index = suffix_array[s];
		if (IS_LMS(index))
		{
			suffix_array[j++] = index;
		}
	}
	assert(j == lms_count);

	// Name LMS strings, using the second half of the suffix array
	int *sub_data = &suffix_array[length / 2];
	int sub_capacity = length - length / 2;
	fill(sub_data, &sub_data[sub_capacity], UNINITIALIZED);
	int name = 0;
	int prev_index = UNINITIALIZED;
	for (int s = 0; s < lms_count; s++)
	{
		int index = suffix_array[s];
		assert(index != UNINITIALIZED);
		if (prev_index != UNINITIALIZED && !substrings_equal(data, prev_index, index, stype))
		{
			name += 1;
		}
		assert(sub_data[index / 2] == UNINITIALIZED);
		sub_data[index / 2] = name;
		prev_index = index;
	}
	int new_alphabet_size = name + 1;

	if (new_alphabet_size != lms_count)
	{
		// Order LMS strings using suffix array of named LMS symbols

		// Compact named LMS symbols
		j = 0;
		for (int i = 0; i < sub_capacity; i++)
		{
			int name = sub_data[i];
			if (name != UNINITIALIZED)
			{
				sub_data[j++] = name;
			}
		}
		assert(j == lms_count);

		// Sort named LMS symbols recursively
		computeSuffixArray(sub_data, suffix_array, lms_count, new_alphabet_size);

		// Map named LMS symbol indices to LMS string indices in input string
		j = 0;
		for (int i = 1; i < length ; i++)
		{
			if (IS_LMS(i))
			{
				sub_data[j++] = i;
			}
		}
		assert(j == lms_count);
		for (int s = 0 ; s < lms_count ; s++)
		{
			assert(suffix_array[s] < lms_count);
			suffix_array[s] = sub_data[suffix_array[s]];
		}
	}

	// Put LMS suffixes in sorted order at the ends of buckets
	j = length;
	int s = lms_count - 1;
	for (int b = alphabet_size - 1; b >= 0; b--)
	{
		while (s >= 0 && data[suffix_array[s]] == b)
		{
			suffix_array[--j] = suffix_array[s--];
		}
		assert(j >= buckets[b]);
		while (j > buckets[b])
		{
			suffix_array[--j] = UNINITIALIZED;
		}
	}

	// Induce from sorted LMS strings to sort all suffixes
	induce(data, suffix_array, length, alphabet_size, stype, &buckets[0], &bucket_index[0]);
}

class Coder
{
	bool cacheable;
	bool has_cache;
	int number_context_offset;
	int n_number_contexts;
	vector<vector<unsigned short> > cache;

protected:
	Coder() : cacheable(false), has_cache(false){}

	// Mark coder as cacheable
	void setCacheable(bool cacheable)
	{
		this->cacheable = cacheable;
	}

public:
	// Set parameters for number size cache
	void setNumberContexts(int number_context_offset, int n_number_contexts, int max_number)
	{
		if (!cacheable) return;

		this->number_context_offset = number_context_offset;
		this->n_number_contexts = n_number_contexts;
		cache.clear();
		for (int context_index = 0 ; context_index < n_number_contexts ; context_index++)
		{
			int base_context = number_context_offset + (context_index << 8);
			cache.push_back(vector<unsigned short>());
			vector<unsigned short>& c = cache.back();
			c.resize(4);
			c[2] = code(base_context + 2, 0) + code(base_context + 1, 0);
			c[3] = code(base_context + 2, 0) + code(base_context + 1, 1);
			int prev_base = 2;
			for (int data_bits = 2 ; data_bits < 30 ; data_bits++)
			{
				int base = c.size();
				int base_sizedif = - code(base_context + data_bits * 2 - 2, 0)
					+ code(base_context + data_bits * 2 - 2, 1)
					+ code(base_context + data_bits * 2, 0);
				for (int msb = 0 ; msb <= 1 ; msb++)
				{
					int sizedif = base_sizedif + code(base_context + data_bits * 2 - 1, msb);
					for (int tail = 0 ; tail < 1 << (data_bits - 1) ; tail++)
					{
						int size = c[prev_base + tail] + sizedif;
						c.push_back(size);
						if (c.size() > (size_t) max_number) goto next_context;
					}
				}
				prev_base = base;
			}
next_context:;
#if 0
			for (int i = 2 ; i < c.size() ; i++)
			{
				assert(c[i] == encodeNumber(base_context, i));
			}
#endif
		}

		has_cache = true;
	}

	// Number of fractional bits in the bit sizes returned by coding functions.
	static const int BIT_PRECISION = 6;

	// Code the given bit value in the given context.
	// Returns the coded size of the bit (in fractional bits).
	virtual int code(int context, int bit) = 0;

	// Encode a number >= 2 using a variable-length encoding.
	// Returns the coded size of the number (in fractional bits).
	int encodeNumber(int base_context, int number)
	{
		assert(number >= 2);

		if (has_cache)
		{
			int context_index = (base_context - number_context_offset) >> 8;
			vector<unsigned short>& cache_for_context = cache[context_index];
			if (number < (int)cache_for_context.size())
			{
				return cache_for_context[number];
			}
		}

		int size = 0;
		int context;
		int i;
		for (i = 0 ; (4 << i) <= number ; i++)
		{
			context = base_context + (i * 2 + 2);
			size += code(context, 1);
		}
		context = base_context + (i * 2 + 2);
		size += code(context, 0);

		for (; i >= 0 ; i--)
		{
			int bit = ((number >> i) & 1);
			context = base_context + (i * 2 + 1);
			size += code(context, bit);
		}

		return size;
	}

	virtual ~Coder()	{}
};


class LZState
{
	unsigned after_first:1;
	unsigned prev_was_ref:1;
	unsigned last_offset:28;

	friend class LZEncoder;
};

class LZEncoder
{
	static const int NUM_SINGLE_CONTEXTS = 1;
	static const int NUM_CONTEXT_GROUPS = 4;
	static const int CONTEXT_GROUP_SIZE = 256;

	static const int CONTEXT_KIND = 0;
	static const int CONTEXT_REPEATED = -1;

	static const int CONTEXT_GROUP_LIT = 0;
	static const int CONTEXT_GROUP_OFFSET = 2;
	static const int CONTEXT_GROUP_LENGTH = 3;

	Coder *coder;

	int code(int context, int bit) const
	{
		return coder->code(NUM_SINGLE_CONTEXTS + context, bit);
	}

	int encodeNumber(int context_group, int number) const
	{
		return coder->encodeNumber(NUM_SINGLE_CONTEXTS + (context_group << 8), number);
	}

	friend class LZDecoder;

public:
	static const int KIND_LIT = 0;
	static const int KIND_REF = 1;

	static const int NUM_CONTEXTS = (NUM_SINGLE_CONTEXTS + NUM_CONTEXT_GROUPS * CONTEXT_GROUP_SIZE);
	static const int NUMBER_CONTEXT_OFFSET = (NUM_SINGLE_CONTEXTS + CONTEXT_GROUP_OFFSET * CONTEXT_GROUP_SIZE);
	static const int NUM_NUMBER_CONTEXTS = 2;

	LZEncoder(Coder *coder) : coder(coder)
	{

	}

	void setInitialState(LZState *state) const
	{
		state->after_first = 0;
		state->prev_was_ref = 0;
		state->last_offset = 0;
	}

	void constructState(LZState *state, int pos, bool prev_was_ref, int last_offset) const
	{
		state->after_first = pos > 0;
		state->prev_was_ref = prev_was_ref;
		state->last_offset = last_offset;
	}

	int encodeLiteral(unsigned char value, const LZState *state_before, LZState *state_after) const
	{
		int size = 0;
		if (state_before->after_first)
		{
			size += code(CONTEXT_KIND, KIND_LIT);
		}
		int context = 1;
		for (int i = 7 ; i >= 0 ; i--)
		{
			int bit = ((value >> i) & 1);
			size += code(context, bit);
			context = (context << 1) | bit;
		}

		state_after->after_first = 1;
		state_after->prev_was_ref = 0;
		state_after->last_offset = state_before->last_offset;

		return size;
	}

	int encodeReference(int offset, int length, const LZState *state_before, LZState *state_after) const
	{
		assert(offset >= 1);
		assert(length >= 2);
		assert(state_before->after_first);

		int size = code(CONTEXT_KIND, KIND_REF);
		int rep_offset = offset == state_before->last_offset;
		if (!state_before->prev_was_ref)
		{
			size += code(CONTEXT_REPEATED, rep_offset);
		}
		else
		{
			assert(!rep_offset);
		}
		if (!rep_offset)
		{
			size += encodeNumber(CONTEXT_GROUP_OFFSET, offset + 2);
		}
		size += encodeNumber(CONTEXT_GROUP_LENGTH, length);

		state_after->after_first = 1;
		state_after->prev_was_ref = 1;
		state_after->last_offset = offset;

		return size;
	}

	int finish(const LZState *state_before) const
	{
		int size = code(CONTEXT_KIND, KIND_REF);
		if (!state_before->prev_was_ref)
		{
			size += code(CONTEXT_REPEATED, 0);
		}
		int context_group = CONTEXT_GROUP_OFFSET;
		int number = 2;
		size += encodeNumber(context_group, number);

		return size;
	}
};


// For each offset:
//   Best total size with last ref having that offset

class RefEdge
{
	int pos;
	int offset;
	int length;
	int total_size;
	int refcount;
	RefEdge *source;

	RefEdge(int pos, int offset, int length, int total_size, RefEdge *source)
		: pos(pos), offset(offset), length(length), total_size(total_size), source(source)
	{
		assert(source != this);
		refcount = 1;
		if (source != NULL)
		{
			source->refcount++;
		}
		_heap_index = 0;
	}

	int target()
	{
		return pos + length;
	}

	friend class RefEdgeFactory;
	friend class LZParser;
	friend struct LZResultEdge;
	friend class LZParseResult;
	friend struct std::less<RefEdge*>;

public:
	int _heap_index;
};

namespace std
{
	template <> struct less<RefEdge*>
	{
		bool operator()(RefEdge* const & e1, RefEdge* const & e2) const
		{
			return e1->total_size < e2->total_size;
		}
	};
}

// Factory for RefEdge objects which recycles destroyed objects for efficiency
class RefEdgeFactory
{
	int edge_capacity;
	int edge_count;
	int cleaned_edges;

	RefEdge* buffer;
public:
	int max_edge_count;
	int max_cleaned_edges;

	RefEdgeFactory(int edge_capacity) : edge_capacity(edge_capacity),
		edge_count(0), cleaned_edges(0), max_edge_count(0), max_cleaned_edges(0)
	{
		buffer = NULL;
	}

	~RefEdgeFactory()
	{
		while (buffer != NULL)
		{
			RefEdge *edge = buffer;
			buffer = buffer->source;
			delete edge;
		}
	}

	void reset()
	{
		assert(edge_count == 0);
		cleaned_edges = 0;
	}

	RefEdge* create(int pos, int offset, int length, int total_size, RefEdge *source)
	{
		max_edge_count = max(max_edge_count, ++edge_count);
		if (buffer == NULL)
		{
			return new RefEdge(pos, offset, length, total_size, source);
		}
		else
		{
			RefEdge* edge = buffer;
			buffer = edge->source;
			return new (edge) RefEdge(pos, offset, length, total_size, source);
		}
	}

	void destroy(RefEdge* edge, bool clean)
	{
		edge->source = buffer;
		buffer = edge;
		edge_count--;
		if (clean)
		{
			max_cleaned_edges = max(max_cleaned_edges, ++cleaned_edges);
		}
	}

	bool full()
	{
		return edge_count >= edge_capacity;
	}

};


class MatchFinder
{
	// Inputs
	unsigned char *data;
	int length;
	int min_length;
	int match_patience;
	int max_same_length;

	// Suffix array
	vector<int> suffix_array;
	vector<int> rev_suffix_array;
	vector<int> longest_common_prefix;

	// Matcher parameters
	int current_pos;
	int min_pos;

	// Matcher state
	int left_index;
	int left_length;
	int right_index;
	int right_length;
	int current_length;

	// Best matches seen with current length
	std::priority_queue<int, vector<int>, std::greater<int> > match_buffer;

	void make_suffix_array()
	{
		// Use reverse suffix array to store string as integers with sentinel
		rev_suffix_array.resize(length + 1);
		for (int i = 0; i < length ; i++)
		{
			rev_suffix_array[i] = data[i] + 1;
		}
		rev_suffix_array[length] = 0;

		// Compute suffix array
		suffix_array.resize(length + 1);
		computeSuffixArray(&rev_suffix_array[0], &suffix_array[0], length + 1, 257);

		// Compute reverse suffix array
		for (int i = 0 ; i <= length ; i++)
		{
			rev_suffix_array[suffix_array[i]] = i;
		}

		// Compute LCP array
		longest_common_prefix.resize(length + 1);
		longest_common_prefix[0] = 0;
		longest_common_prefix[length] = 0;
		int h = 0;
		for (int i = 0 ; i < length ; i++)
		{
			int r = rev_suffix_array[i];
			if (r < length)
			{
				int j = suffix_array[r + 1];
				int m = length - std::max(i, j);
				while (h < m && data[i + h] == data[j + h])
				{
					h = h + 1;
				}
				longest_common_prefix[r] = h;
				if (h > 0) h = h - 1;
			}
		}
	}

	void extend_left()
	{
		int iter = 0;
		while (left_length >= min_length)
		{
			left_length = std::min(left_length, longest_common_prefix[--left_index]);
			int pos = suffix_array[left_index];
			if (pos < current_pos && pos >= min_pos) break;
			if (++iter > match_patience)
			{
				left_length = 0;
				break;
			}
		}
	}

	void extend_right()
	{
		int iter = 0;
		while (true)
		{
			right_length = std::min(right_length, longest_common_prefix[right_index]);
			if (right_length < min_length) break;
			int pos = suffix_array[++right_index];
			if (pos < current_pos && pos >= min_pos) break;
			if (++iter > match_patience)
			{
				right_length = 0;
				break;
			}
		}
	}

	int next_length()
	{
		return std::max(left_length, right_length);
	}

public:
	MatchFinder(unsigned char *data, int length, int min_length, int match_patience, int max_same_length) :
		data(data), length(length), min_length(min_length), match_patience(match_patience), max_same_length(max_same_length)
	{
		make_suffix_array();
		reset();
	}

	void reset()
	{
	}

	// Start finding matches between strings starting at pos and earlier strings.
	void beginMatching(int pos)
	{
		current_pos = pos;
		min_pos = 0;

		left_index = rev_suffix_array[pos];
		left_length = length - pos;
		extend_left();
		right_index = rev_suffix_array[pos];
		right_length = length - pos;
		extend_right();
	}

	// Report next match. Returns whether a match was found.
	bool nextMatch(int *match_pos_out, int *match_length_out)
	{
		if (match_buffer.empty())
		{
			// Fill match buffer
			current_length = next_length();
			if (current_length < min_length) return false;
			int new_min_pos = min_pos;
			do
			{
				int match_pos;
				if (left_length > right_length)
				{
					match_pos = suffix_array[left_index];
					extend_left();
				}
				else
				{
					match_pos = suffix_array[right_index];
					extend_right();
				}
				new_min_pos = std::max(new_min_pos, match_pos);
				if (match_buffer.size() < (size_t) max_same_length)
				{
					match_buffer.push(match_pos);
				}
				else
				{
					if (match_pos > match_buffer.top())
					{
						match_buffer.pop();
						match_buffer.push(match_pos);
					}
					min_pos = match_buffer.top();
				}
			}
			while (next_length() == current_length);
			assert(!match_buffer.empty());
			min_pos = new_min_pos;
		}

		*match_length_out = current_length;
		*match_pos_out = match_buffer.top();
		match_buffer.pop();
		assert(*match_pos_out < current_pos);
		return true;
	}
};

struct LZResultEdge
{
	int pos;
	int offset;
	int length;

	LZResultEdge(RefEdge *edge) : pos(edge->pos), offset(edge->offset), length(edge->length)	{}

	friend class LZParseResult;
};

typedef unsigned long long result_size_t;

class LZParseResult
{
	vector<LZResultEdge> edges;
	const unsigned char *data;
	int data_length;
	int zero_padding;
public:
	result_size_t encode(const LZEncoder& result_encoder) const
	{
		result_size_t size = 0;
		int pos = 0;
		LZState state;
		result_encoder.setInitialState(&state);
		for (int i = edges.size() - 1 ; i >= 0 ; i--)
		{
			const LZResultEdge *edge = &edges[i];
			while (pos < edge->pos)
			{
				size += result_encoder.encodeLiteral(data[pos++], &state, &state);
			}
			size += result_encoder.encodeReference(edge->offset, edge->length, &state, &state);
			pos += edge->length;
		}
		while (pos < data_length)
		{
			size += result_encoder.encodeLiteral(data[pos++], &state, &state);
		}
		if (zero_padding > 0)
		{
			size += result_encoder.encodeLiteral(0, &state, &state);
			if (zero_padding == 2)
			{
				size += result_encoder.encodeLiteral(0, &state, &state);
			}
			else if (zero_padding > 1)
			{
				size += result_encoder.encodeReference(1, zero_padding - 1, &state, &state);
			}
		}
		size += result_encoder.finish(&state);
		return size;
	}

	friend class LZParser;
};

class LZParser
{
	const unsigned char *data;
	int data_length;
	int zero_padding;
	MatchFinder& finder;
	int length_margin;
	int skip_length;
	const LZEncoder* encoderp;
	RefEdgeFactory* edge_factory;

	vector<int> literal_size;
	vector<CuckooHash<RefEdge*> > edges_to_pos;
	RefEdge* best;
	CuckooHash<RefEdge*> best_for_offset;
	Heap<RefEdge*> root_edges;

	bool is_root(RefEdge *edge)
	{
		return root_edges.contains(edge);
	}

	void remove_root(RefEdge *edge)
	{
		root_edges.remove(edge);
	}

	void releaseEdge(RefEdge *edge, bool clean = false)
	{
		while (edge != NULL)
		{
			RefEdge *source = edge->source;
			if (--edge->refcount == 0)
			{
				assert(!is_root(edge));
				edge_factory->destroy(edge, clean);
			}
			else
			{
				return;
			}
			edge = source;
		}
	}

	// Return progress
	bool clean_worst_edge(int pos, RefEdge *exclude)
	{
		if (root_edges.size() == 0) return false;
		RefEdge *worst_edge = root_edges.remove_largest();
		if (worst_edge == best || worst_edge == exclude) return true;
		CuckooHash<RefEdge*>& container = worst_edge->target() > pos
			? edges_to_pos[worst_edge->target()]
		: best_for_offset;
		if (container.size() > 1 && container.count(worst_edge->offset) > 0)
		{
			container.erase(worst_edge->offset);
			releaseEdge(worst_edge, true);
		}
		return true;
	}

	void put_by_offset(CuckooHash<RefEdge*>& by_offset, RefEdge* edge)
	{
		assert(!is_root(edge));
		if (by_offset.count(edge->offset) == 0)
		{
			by_offset[edge->offset] = edge;
			root_edges.insert(edge);
		}
		else if (edge->total_size < by_offset[edge->offset]->total_size)
		{
			RefEdge* old_edge = by_offset[edge->offset];
			remove_root(old_edge);
			releaseEdge(old_edge);
			by_offset[edge->offset] = edge;
			root_edges.insert(edge);
		}
		else
		{
			releaseEdge(edge);
		}
	}

	void newEdge(RefEdge *source, int pos, int offset, int length)
	{
		if (source && offset == source->offset && pos == source->target()) return;
		int prev_target = source ? source->target() : 0;
		int new_target = pos + length;
		LZState state_before;
		LZState state_after;
		encoderp->constructState(&state_before, pos, pos == prev_target, source ? source->offset : 0);
		int size_before = (source ? source->total_size : literal_size[data_length]) - (literal_size[data_length] - literal_size[pos]);
		int edge_size = encoderp->encodeReference(offset, length, &state_before, &state_after);
		int size_after = literal_size[data_length] - literal_size[new_target];
		while (edge_factory->full())
		{
			if (!clean_worst_edge(pos, source)) break;
		}
		RefEdge *new_edge = edge_factory->create(pos, offset, length, size_before + edge_size + size_after, source);
		put_by_offset(edges_to_pos[new_target], new_edge);
	}

public:
	LZParser(const unsigned char *data, int data_length, int zero_padding, MatchFinder& finder, int length_margin, int skip_length, RefEdgeFactory* edge_factory)
		: data(data), data_length(data_length), zero_padding(zero_padding), finder(finder), length_margin(length_margin), skip_length(skip_length), edge_factory(edge_factory)
	{
		// Initialize edges_to_pos array
		edges_to_pos.resize(data_length + 1);
		best = NULL;
	}

	LZParseResult parse(const LZEncoder& encoder)
	{
		encoderp = &encoder;

		// Reset state
		best_for_offset.clear();
		root_edges.clear();
		edge_factory->reset();

		// Accumulate literal sizes
		literal_size.resize(data_length + 1, 0);
		int size = 0;
		LZState literal_state;
		encoder.setInitialState(&literal_state);
		for (int i = 0 ; i < data_length ; i++)
		{
			literal_size[i] = size;
			size += encoder.encodeLiteral(data[i], &literal_state, &literal_state);
		}
		literal_size[data_length] = size;

		// Parse
		RefEdge* initial_best = edge_factory->create(0, 0, 0, literal_size[data_length], NULL);
		best = initial_best;
		for (int pos = 1 ; pos <= data_length ; pos++)
		{
			// Assimilate edges ending here
			for (CuckooHash<RefEdge*>::iterator it = edges_to_pos[pos].begin() ; it != edges_to_pos[pos].end() ; it++)
			{
				RefEdge *edge = it->second;
				if (edge->total_size < best->total_size)
				{
					best = edge;
				}
				remove_root(edge);
				put_by_offset(best_for_offset, edge);
			}
			edges_to_pos[pos].clear();

			// Add new edges according to matches
			finder.beginMatching(pos);
			int match_pos;
			int match_length;
			int max_match_length = 0;
			while (finder.nextMatch(&match_pos, &match_length))
			{
				int offset = pos - match_pos;
				if (match_length > data_length - pos)
				{
					match_length = data_length - pos;
				}
				int min_length = match_length - length_margin;
				if (min_length < 2) min_length = 2;
				for (int length = min_length ; length <= match_length ; length++)
				{
					newEdge(best, pos, offset, length);
					if (best->offset != offset && best_for_offset.count(offset))
					{
						assert(best_for_offset[offset]->target() <= pos);
						newEdge(best_for_offset[offset], pos, offset, length);
					}
				}
				max_match_length = max(max_match_length, match_length);
			}

			// If we have a very long match, skip ahead
			if (max_match_length >= skip_length && !edges_to_pos[pos + max_match_length].empty())
			{
				root_edges.clear();
				for (CuckooHash<RefEdge*>::iterator it = best_for_offset.begin() ; it != best_for_offset.end() ; it++)
				{
					releaseEdge(it->second);
				}
				best_for_offset.clear();
				int target_pos = pos + max_match_length;
				while (pos < target_pos - 1)
				{
					CuckooHash<RefEdge*>& edges = edges_to_pos[++pos];
					for (CuckooHash<RefEdge*>::iterator it = edges.begin() ; it != edges.end() ; it++)
					{
						releaseEdge(it->second);
					}
					edges.clear();
				}
				best = initial_best;
			}
		}

		// Clean unused paths
		root_edges.clear();
		for (CuckooHash<RefEdge*>::iterator it = best_for_offset.begin() ; it != best_for_offset.end() ; it++)
		{
			RefEdge *edge = it->second;
			if (edge != best)
			{
				releaseEdge(edge);
			}
		}

		// Find best path
		LZParseResult result;
		result.data = data;
		result.data_length = data_length;
		result.zero_padding = zero_padding;
		RefEdge *edge = best;
		while (edge->length > 0)
		{
			result.edges.push_back(LZResultEdge(edge));
			edge = edge->source;
		}
		releaseEdge(edge);
		releaseEdge(best);

		return result;
	}

};



struct PackParams
{
	int iterations;
	int length_margin;
	int skip_length;
	int match_patience;
	int max_same_length;
};

struct ContextCounts
{
	int counts[2];
};

class CountingCoder : public Coder
{
	vector<ContextCounts> context_counts;

	friend class SizeMeasuringCoder;
public:
	CountingCoder(int n_contexts)
	{
		struct ContextCounts init_counts =
		{
			{ 0, 0 }
		};
		context_counts.resize(n_contexts, init_counts);
	}

	CountingCoder(CountingCoder *old_counts, CountingCoder *new_counts)
	{
		for (int i = 0 ; i < (int) old_counts->context_counts.size() ; i++)
		{
			struct ContextCounts old_count = old_counts->context_counts[i];
			struct ContextCounts new_count = new_counts->context_counts[i];
			struct ContextCounts mixed_count =
			{
				{
					(old_count.counts[0] * 3 + new_count.counts[0]) / 4,
						(old_count.counts[1] * 3 + new_count.counts[1]) / 4
				}
			};
			context_counts.push_back(mixed_count);
		}
	}

	virtual int code(int context_index, int bit)
	{
		context_counts[context_index].counts[bit]++;
		return 0;
	}

	void printRange(FILE *out, int first, int num)
	{
		fprintf(out, "[");
		for (int i = 0 ; i < num ; i++)
		{
			if (i > 0)
			{
				fprintf(out, " ");
			}
			fprintf(out, "%d/%d", context_counts[first + i].counts[0], context_counts[first + i].counts[1]);
		}
		fprintf(out, "]");
	}

};

struct ContextSizes
{
	unsigned short sizes[2];
};

class SizeMeasuringCoder : public Coder
{
	static const int MIN_SIZE = 2;
	static const int MAX_SIZE = 12 << BIT_PRECISION;

	vector<ContextSizes> context_sizes;

	int sizeForCount(int count, int total)
	{
		int size = (int) floor(0.5 + log(total / (double) count) / log(2.0) * (1 << BIT_PRECISION));
		if (size < MIN_SIZE) size = MIN_SIZE;
		if (size > MAX_SIZE) size = MAX_SIZE;
		return size;
	}

public:
	SizeMeasuringCoder(int n_contexts)
	{
		struct ContextSizes default_sizes =
		{
			{ 1 << BIT_PRECISION, 1 << BIT_PRECISION }
		};
		context_sizes.resize(n_contexts, default_sizes);
		setCacheable(true);
	}

	SizeMeasuringCoder(CountingCoder *counting_coder)
	{
		context_sizes.resize(counting_coder->context_counts.size());
		for (int i = 0 ; i < (int) counting_coder->context_counts.size() ; i++)
		{
			struct ContextSizes s;
			struct ContextCounts c = counting_coder->context_counts[i];
			int count0 = 1 + c.counts[0];
			int count1 = 1 + c.counts[1];
			int sum = count0 + count1;
			s.sizes[0] = sizeForCount(count0, sum);
			s.sizes[1] = sizeForCount(count1, sum);
			context_sizes[i] = s;
		}
		setCacheable(true);
	}

	virtual int code(int context_index, int bit)
	{
		return context_sizes[context_index].sizes[bit];
	}
};

#ifndef ADJUST_SHIFT
#define ADJUST_SHIFT 4
#endif

class RangeCoder : public Coder
{
	vector<unsigned short> contexts;
	vector<unsigned char>& out;
	int dest_bit;
	unsigned intervalsize;
	unsigned intervalmin;

	static int sizetable[128];
	static bool sizetable_init;

	static bool init_sizetable()
	{
		for (int i = 0 ; i < 128 ; i++)
		{
			sizetable[i] = (int) floor(0.5 + (8.0 - log((double) (128 + i)) / log(2.0)) * (1 << BIT_PRECISION));
		}

		return true;
	}

	void addBit()
	{
		int pos = dest_bit;
		int bytepos;
		int bitmask;
		do
		{
			pos--;
			if (pos < 0) return;
			bytepos = pos >> 3;
			bitmask = 0x80 >> (pos & 7);
			while (bytepos >= (int) out.size())
			{
				out.push_back(0);
			}
			out[bytepos] ^= bitmask;
		}
		while ((out[bytepos] & bitmask) == 0);
	}

public:
	RangeCoder(int n_contexts, vector<unsigned char>& out) : out(out)
	{
		contexts.resize(n_contexts, 0x8000);
		dest_bit = -1;
		intervalsize = 0x8000;
		intervalmin = 0;
		out.clear();
	}

	virtual int code(int context_index, int bit)
	{
		assert(context_index < (int) contexts.size());
		assert(bit == 0 || bit == 1);
		int size_before = (dest_bit << BIT_PRECISION) + sizetable[(intervalsize - 0x8000) >> 8];
		unsigned prob = contexts[context_index];
		unsigned threshold = (intervalsize * prob) >> 16;
		unsigned new_prob;
		if (!bit)
		{
			// Zero
			intervalmin += threshold;
			if (intervalmin & 0x10000)
			{
				addBit();
			}
			intervalsize = intervalsize - threshold;
			new_prob = prob - (prob >> ADJUST_SHIFT);
		}
		else
		{
			// One
			intervalsize = threshold;
			new_prob = prob + (0xffff >> ADJUST_SHIFT) - (prob >> ADJUST_SHIFT);
		}
		assert(new_prob > 0);
		assert(new_prob < 0x10000);
		contexts[context_index] = new_prob;
		while (intervalsize < 0x8000)
		{
			dest_bit++;
			intervalsize <<= 1;
			intervalmin <<= 1;
			if (intervalmin & 0x10000)
			{
				addBit();
			}
		}
		intervalmin &= 0xffff;

		int size_after = (dest_bit << BIT_PRECISION) + sizetable[(intervalsize - 0x8000) >> 8];
		return size_after - size_before;
	}

	void reset()
	{
		fill(contexts.begin(), contexts.end(), 0x8000);
	}

	void finish()
	{
		int intervalmax = intervalmin + intervalsize;
		int final_min = 0;
		int final_size = 0x10000;
		while (final_min < (int) intervalmin || final_min + final_size >= intervalmax)
		{
			if (final_min + final_size < intervalmax)
			{
				addBit();
				final_min += final_size;
			}
			dest_bit++;
			final_size >>= 1;
		}

		while ((dest_bit - 1) >> 3 >= (int) out.size())
		{
			out.push_back(0);
		}
	}

	int sizeInBits()
	{
		return dest_bit + 1;
	}

};


int RangeCoder::sizetable[128];
bool RangeCoder::sizetable_init = init_sizetable();

static void packData(unsigned char *data, int data_length, int zero_padding, PackParams *params, Coder *result_coder, RefEdgeFactory *edge_factory)
{
	MatchFinder finder(data, data_length, 2, params->match_patience, params->max_same_length);
	LZParser parser(data, data_length, zero_padding, finder, params->length_margin, params->skip_length, edge_factory);
	result_size_t real_size = 0;
	result_size_t best_size = (result_size_t)1 << (32 + 3 + Coder::BIT_PRECISION);
	int best_result = 0;
	vector<LZParseResult> results(2);
	CountingCoder *counting_coder = new CountingCoder(LZEncoder::NUM_CONTEXTS);
	printf("%8d", data_length);
	for (int i = 0 ; i < params->iterations ; i++)
	{
		printf("  ");

		// Parse data into LZ symbols
		LZParseResult& result = results[1 - best_result];
		Coder *measurer = new SizeMeasuringCoder(counting_coder);
		measurer->setNumberContexts(LZEncoder::NUMBER_CONTEXT_OFFSET, LZEncoder::NUM_NUMBER_CONTEXTS, data_length);
		finder.reset();
		result = parser.parse(LZEncoder(measurer));
		delete measurer;

		// Encode result using adaptive range coding
		vector<unsigned char> dummy_result;
		RangeCoder *range_coder = new RangeCoder(LZEncoder::NUM_CONTEXTS, dummy_result);
		real_size = result.encode(LZEncoder(range_coder));
		range_coder->finish();
		delete range_coder;

		// Choose if best
		if (real_size < best_size)
		{
			best_result = 1 - best_result;
			best_size = real_size;
		}

		// Print size
		printf("%.4f", real_size / (double) (8 << Coder::BIT_PRECISION));

		// Count symbol frequencies
		CountingCoder *new_counting_coder = new CountingCoder(LZEncoder::NUM_CONTEXTS);
		result.encode(LZEncoder(counting_coder));

		// New size measurer based on frequencies
		CountingCoder *old_counting_coder = counting_coder;
		counting_coder = new CountingCoder(old_counting_coder, new_counting_coder);
		delete old_counting_coder;
		delete new_counting_coder;
	}
	delete counting_coder;

	results[best_result].encode(LZEncoder(result_coder));
}




CompressionV::CompressionV()
{

}

CompressionV::~CompressionV()
{
}

int CompressionV::Compress( const u8*src, u32 src_len,u8 *dst, u32 *dst_len,int level)
{
	PackParams params;
	params.iterations = min(9 , (9 * level) / 10);
	params.length_margin = (1000 * level) / 10;
	params.skip_length = (10 * level) / 10;//100000;
	params.match_patience = (1000 * level) / 10;
	params.max_same_length = (1000 * level) / 10;

	vector<unsigned char> pack_buffer;
	RangeCoder range_coder(LZEncoder::NUM_CONTEXTS, pack_buffer);
	RefEdgeFactory edge_factory(1000000);

	// Crunch the data
	range_coder.reset();
	packData((unsigned char*)src, src_len, 0, &params, &range_coder, &edge_factory);
	range_coder.finish();
	printf("\nEdges %d and removed %d\n",  edge_factory.max_edge_count, edge_factory.max_cleaned_edges);

	memcpy(dst , pack_buffer.data() , pack_buffer.size());
	*dst_len = pack_buffer.size();

	return 0;
}




class Decoder
{
public:
	// Decode a bit in the given context.
	// Returns the decoded bit value.
	virtual int decode(int context) = 0;

	// Decode a number >= 2 using a variable-length encoding.
	// Returns the decoded number.
	int decodeNumber(int base_context)
	{
		int context;
		int i;
		for (i = 0 ;; i++)
		{
			context = base_context + (i * 2 + 2);
			if (decode(context) == 0) break;
		}

		int number = 1;
		for (; i >= 0 ; i--)
		{
			context = base_context + (i * 2 + 1);
			int bit = decode(context);
			number = (number << 1) | bit;
		}

		return number;
	}

	virtual ~Decoder()	{}
};



class CompressedDataReadListener
{
public:
	virtual void read(int index) = 0;

	virtual ~CompressedDataReadListener()	{}
};

class RangeDecoder : public Decoder
{
	vector<unsigned short> contexts;
	vector<unsigned char>& data;
	CompressedDataReadListener* listener;
	int bit_index;
	unsigned intervalsize;
	unsigned intervalvalue;
	unsigned uncertainty;

	int getBit()
	{
		int byte_index = bit_index >> 3;
		int bit_in_byte = (~bit_index) & 7;
		if (bit_in_byte == 7)
		{
			if (listener) listener->read(byte_index);
		}
		if (bit_index++ >= (int) data.size() * 8)
		{
			uncertainty <<= 1;
			return 0;
		}
		int bit = (data[byte_index] >> bit_in_byte) & 1;
		return bit;
	}

public:
	RangeDecoder(int n_contexts, vector<unsigned char>& data) : data(data)
	{
		contexts.resize(n_contexts, 0x8000);
		bit_index = 0;
		intervalsize = 1;
		intervalvalue = 0;
		uncertainty = 1;
		listener = NULL;
	}

	virtual int decode(int context_index)
	{
		assert(context_index < (int) contexts.size());
		unsigned prob = contexts[context_index];
		while (intervalsize < 0x8000)
		{
			intervalsize <<= 1;
			intervalvalue = (intervalvalue << 1) | getBit();
		}

		int bit;
		unsigned new_prob;
		unsigned threshold = (intervalsize * prob) >> 16;
		if (intervalvalue >= threshold)
		{
			// Zero
			bit = 0;
			intervalvalue -= threshold;
			intervalsize -= threshold;
			new_prob = prob - (prob >> ADJUST_SHIFT);
		}
		else
		{
			// One
			assert(intervalvalue + uncertainty <= threshold);
			bit = 1;
			intervalsize = threshold;
			new_prob = prob + (0xffff >> ADJUST_SHIFT) - (prob >> ADJUST_SHIFT);
		}
		assert(new_prob > 0);
		assert(new_prob < 0x10000);
		contexts[context_index] = new_prob;

		return bit;
	}

	void reset()
	{
		fill(contexts.begin(), contexts.end(), 0x8000);
	}

	void setListener(CompressedDataReadListener* listener)
	{
		this->listener = listener;
	}
};


class LZReceiver
{
public:
	virtual bool receiveLiteral(unsigned char value) = 0;
	virtual bool receiveReference(int offset, int length) = 0;
	virtual ~LZReceiver()	{}
};

class LZDecoder
{
	Decoder *decoder;

	int decode(int context) const
	{
		return decoder->decode(LZEncoder::NUM_SINGLE_CONTEXTS + context);
	}

	int decodeNumber(int context_group) const
	{
		return decoder->decodeNumber(LZEncoder::NUM_SINGLE_CONTEXTS + (context_group << 8));
	}

public:
	LZDecoder(Decoder *decoder) : decoder(decoder)
	{

	}

	bool decode(LZReceiver& receiver)
	{
		bool ref = false;
		bool prev_was_ref = false;
		int pos = 0;
		int offset = 0;
		do
		{
			if (ref)
			{
				bool repeated = false;
				if (!prev_was_ref)
				{
					repeated = decode(LZEncoder::CONTEXT_REPEATED)?true:false;
				}
				if (!repeated)
				{
					offset = decodeNumber(LZEncoder::CONTEXT_GROUP_OFFSET) - 2;
					if (offset == 0) break;
				}
				int length = decodeNumber(LZEncoder::CONTEXT_GROUP_LENGTH);
				if (!receiver.receiveReference(offset, length)) return false;
				pos += length;
				prev_was_ref = true;
			}
			else
			{
				int context = 1;
				for (int i = 7 ; i >= 0 ; i--)
				{
					int bit = decode(context);
					context = (context << 1) | bit;
				}
				unsigned char lit = context;
				if (!receiver.receiveLiteral(lit)) return false;
				pos += 1;
				prev_was_ref = false;
			}
			ref = decode(LZEncoder::CONTEXT_KIND)?true:false;
		}
		while (true);
		return true;
	}

};

class LZReceiverImpl : public LZReceiver
{
public:
	LZReceiverImpl(unsigned char *ibuffer) : pos(0) , buffer(ibuffer)	{}
	int pos;
	unsigned char *buffer;
	virtual bool receiveLiteral(unsigned char value)
	{
		buffer[pos++] = value;
		return true;
	}
	virtual bool receiveReference(int offset, int length)
	{
		for (int i = 0 ; i < length ; i++)
		{
			buffer[pos+i] = buffer[pos - offset + i];
		}
		pos += length;
		return true;
	}
	virtual ~LZReceiverImpl()	{}
};

int DecompressV( const u8 * source, u32 sourceLen ,u8 * dest, u32 * destLen )
{
	vector<unsigned char> pack_buffer(source , source + sourceLen);

	RangeDecoder decoder(LZEncoder::NUM_CONTEXTS, pack_buffer);
	LZDecoder lzd(&decoder);

	LZReceiverImpl receiver(dest);
	lzd.decode(receiver);

	*destLen = receiver.pos;

	return GBA_E_OK;
}
