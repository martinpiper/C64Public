#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "CVersion.h"

#define NUM_INPUTS 9
#define NUM_HIDDEN 2
#define NUM_OUTPUTS 1
#define NUM_TRAINING_SETS 2

double sigmoid(double x)
{
	return 1.0 / (1.0 + exp(-x));
}

double d_sigmoid(double x)
{
	return x * (1.0 - x);
}

// Helper to initialise weights randomly between -1.0 and 1.0
double rand_weight()
{
	return ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
}

// Network components
double hidden_layer[NUM_HIDDEN];
double output_layer[NUM_OUTPUTS];

double hidden_biases[NUM_HIDDEN];
double output_biases[NUM_OUTPUTS];

double hidden_weights[NUM_INPUTS][NUM_HIDDEN];
double output_weights[NUM_HIDDEN][NUM_OUTPUTS];

// Training datasets
double training_inputs[NUM_TRAINING_SETS][NUM_INPUTS] =
{
	// Nought
	//  *
	// * *
	//  *
	{
		0.0, 1.0, 0.0,
		1.0, 0.0, 1.0,
		0.0, 1.0, 0.0
	},
	// Cross
	// * *
	//  * 
	// * *
	{
		1.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 1.0
	}
};
// Expected outputs for each input dataset
double training_outputs[NUM_TRAINING_SETS][NUM_OUTPUTS] =
{
	{ 0.0 },	// Nought
	{ 1.0 }		// Cross
};

void TrainOnce(double learningRate)
{
	for (int t = 0; t < NUM_TRAINING_SETS; t++)
	{
		// 1. FORWARD PROPAGATION
		// Compute hidden layer activation
		for (int j = 0; j < NUM_HIDDEN; j++)
		{
			double activation = hidden_biases[j];
			for (int i = 0; i < NUM_INPUTS; i++)
			{
				activation += training_inputs[t][i] * hidden_weights[i][j];
			}
			hidden_layer[j] = sigmoid(activation);
		}

		// Compute output layer activation
		for (int j = 0; j < NUM_OUTPUTS; j++)
		{
			double activation = output_biases[j];
			for (int i = 0; i < NUM_HIDDEN; i++)
			{
				activation += hidden_layer[i] * output_weights[i][j];
			}
			output_layer[j] = sigmoid(activation);
		}

		// 2. BACKPROPAGATION
		// Compute output layer error gradient (Delta)
		double delta_output[NUM_OUTPUTS];
		for (int j = 0; j < NUM_OUTPUTS; j++)
		{
			double error = training_outputs[t][j] - output_layer[j];
			delta_output[j] = error * d_sigmoid(output_layer[j]);
		}

		// Compute hidden layer error gradient (Delta)
		double delta_hidden[NUM_HIDDEN];
		for (int j = 0; j < NUM_HIDDEN; j++)
		{
			double error = 0.0;
			for (int k = 0; k < NUM_OUTPUTS; k++)
			{
				error += delta_output[k] * output_weights[j][k];
			}
			delta_hidden[j] = error * d_sigmoid(hidden_layer[j]);
		}

		// 3. WEIGHT & BIAS UPDATES (Gradient Descent)
		// Update output weights and biases
		for (int j = 0; j < NUM_OUTPUTS; j++)
		{
			output_biases[j] += delta_output[j] * learningRate;
			for (int i = 0; i < NUM_HIDDEN; i++)
			{
				output_weights[i][j] += hidden_layer[i] * delta_output[j] * learningRate;
			}
		}

		// Update hidden weights and biases
		for (int j = 0; j < NUM_HIDDEN; j++)
		{
			hidden_biases[j] += delta_hidden[j] * learningRate;
			for (int i = 0; i < NUM_INPUTS; i++)
			{
				hidden_weights[i][j] += training_inputs[t][i] * delta_hidden[j] * learningRate;
			}
		}
	}
}

void PrintResults()
{
	printf("Trained Results for Neural Network:\n");
	for (int t = 0; t < NUM_TRAINING_SETS; t++)
	{
		// Forward pass execution for demonstration
		for (int j = 0; j < NUM_HIDDEN; j++)
		{
			double activation = hidden_biases[j];
			for (int i = 0; i < NUM_INPUTS; i++)
			{
				activation += training_inputs[t][i] * hidden_weights[i][j];
			}
			hidden_layer[j] = sigmoid(activation);
		}
		for (int j = 0; j < NUM_OUTPUTS; j++)
		{
			double activation = output_biases[j];
			for (int i = 0; i < NUM_HIDDEN; i++)
			{
				activation += hidden_layer[i] * output_weights[i][j];
			}
			output_layer[j] = sigmoid(activation);
		}
		printf("Input: ");
		for (int i = 0; i < NUM_INPUTS; i++)
		{
			printf("%0.1f ",training_inputs[t][i]);
		}
		printf(" -> Output: ");
		for (int i = 0; i < NUM_OUTPUTS; i++)
		{
			printf("%f ", output_layer[i]);
		}
		printf("(Expected: ");
		for (int i = 0; i < NUM_OUTPUTS; i++)
		{
			printf("%0.1f ", training_outputs[t][i]);
		}
		printf(")\n");
	}
}


int main()
{
	srand(42);	// Life, the universe, and everything

	// Initialise weights and biases randomly
	for (int i = 0; i < NUM_INPUTS; i++)
	{
		for (int j = 0; j < NUM_HIDDEN; j++)
		{
			hidden_weights[i][j] = rand_weight();
		}
	}
	for (int i = 0; i < NUM_HIDDEN; i++)
	{
		hidden_biases[i] = rand_weight();
		for (int j = 0; j < NUM_OUTPUTS; j++)
		{
			output_weights[i][j] = rand_weight();
		}
	}
	for (int i = 0; i < NUM_OUTPUTS; i++)
	{
		output_biases[i] = rand_weight();
	}

	int epochs = 500;

	// Training loop
	for (int epoch = 0; epoch < epochs; epoch++)
	{
		TrainOnce(0.5);	// Learning rate, not too fast or too slow...
		if (!(epoch % 10))
		{
			printf("Epoch %d\n", epoch);
			PrintResults();
		}
	}

	printf("Final\n");
	PrintResults();

	return 0;
}
