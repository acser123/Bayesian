import pandas as pd
import sys

def train(training_file):
    """
    Trains the Naive Bayes model.
    Calculates prior probabilities for the class variable and conditional probabilities for each feature.
    """
    try:
        training_data = pd.read_csv(training_file)
    except FileNotFoundError:
        print(f"Error: Training file not found at {training_file}")
        sys.exit(1)

    # Determine the class variable (the last column)
    class_variable = training_data.columns[-1]

    # Calculate prior probabilities (P(C))
    priors = training_data[class_variable].value_counts(normalize=True).to_dict()

    # Calculate conditional probabilities (P(X|C))
    conditionals = {}
    features = training_data.columns[:-1]
    for feature in features:
        conditionals[feature] = {}
        # Group by class variable and then get value counts for each feature
        grouped = training_data.groupby(class_variable)[feature].value_counts(normalize=True)
        for class_val in priors:
            if class_val in grouped:
                # Convert the series to a dictionary for easier lookup
                conditionals[feature][class_val] = grouped[class_val].to_dict()

    return priors, conditionals, class_variable

def predict(priors, conditionals, class_variable, decisioning_file):
    """
    Makes predictions on the decisioning data.
    """
    try:
        decisioning_data = pd.read_csv(decisioning_file)
    except FileNotFoundError:
        print(f"Error: Decisioning file not found at {decisioning_file}")
        sys.exit(1)

    predictions = []
    for _, row in decisioning_data.iterrows():
        # Find the feature values for the current row
        features = row.to_dict()

        # Calculate the posterior probability for each class
        posteriors = {}
        for class_val, prior_prob in priors.items():
            posterior = prior_prob
            for feature, value in features.items():
                if feature != class_variable and value != '?':
                    # Look up the conditional probability
                    # Use a default of 0 if a value was not seen during training
                    prob = conditionals.get(feature, {}).get(class_val, {}).get(value, 0)
                    posterior *= prob
            posteriors[class_val] = posterior

        # Predict the class with the highest posterior probability
        if not posteriors:
             predictions.append("?") # Should not happen with valid data
        else:
            prediction = max(posteriors, key=posteriors.get)
            predictions.append(prediction)

    # Add the predictions to the decisioning DataFrame
    decisioning_data[class_variable] = predictions
    return decisioning_data

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python nbc.py <training_file> <decisioning_file>")
        sys.exit(1)

    training_file = sys.argv[1]
    decisioning_file = sys.argv[2]

    # Train the model
    priors, conditionals, class_variable = train(training_file)

    # Make predictions
    decisioned_df = predict(priors, conditionals, class_variable, decisioning_file)

    # Print the result to stdout, matching the C program's output format
    print(decisioned_df.to_csv(index=False, lineterminator='\n'), end="")