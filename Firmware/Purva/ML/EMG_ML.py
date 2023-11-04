import numpy as np
import tensorflow as tf
from tensorflow.keras import layers, models
import os
import re
import serial

# Load your data from text files
def load_data(file_paths):
    data = []
    for file_path in file_paths:
        with open(file_path, 'r') as file:
            # Read each line
            lines = file.readlines()

            # Extract numbers using regular expressions
            numbers = []
            for line in lines:
                numbers.extend(map(float, re.findall(r'[-+]?\d*\.\d+|\d+', line)))

            data.extend(numbers)
    return np.array(data)

# Open a serial connection to your Arduino
ser = serial.Serial('/dev/ttyACM0', baudrate=9600)  # Replace '/dev/ttyUSB0' with the appropriate port

# Function to continuously read and predict data using a trained model
def predict_continuous_data(model, array_size=500):
    input_array = []

    try:
        while True:
            line = ser.readline().decode('utf-8', 'ignore').strip()  # Read a line from the serial port

            # Replace all dots except the first one in each sequence
            cleaned_line = '.'.join(part if i == 0 else '' for i, part in enumerate(line.split('.')))

            # Remove dots at the beginning or end of the line
            cleaned_line = cleaned_line.strip('.')

            if cleaned_line:
                value = float(cleaned_line)
                input_array.append(value)

                if len(input_array) == array_size:
                    # Reshape the input data for prediction
                    input_data = np.array(input_array).reshape((-1, 1))

                    # Make a single prediction for the entire array
                    prediction = model.predict(input_data)

                    # Convert probabilities to class predictions using a threshold (0.5 in this case)
                    class_predictions = (prediction >= 0.5).astype(int)

                    # Print the class predictions or use them as needed
                    print(class_predictions[0])

                    # Clear the input array for the next batch
                    input_array = []

    except KeyboardInterrupt:
        # Stop when the user presses Ctrl+C
        pass
    finally:
        # Close the serial connection
        ser.close()

# Get the absolute paths
base_path = '/home/purva/EMG_ArmBand/ML/assets/'
train_data_class1_path = os.path.join(base_path, 'R_t_W_DONE_10sec.txt')
train_data_class2_path = os.path.join(base_path, 'W_t_R_10sec_DONE.txt')

# Load your training data
train_data_class1 = load_data([train_data_class1_path])
train_data_class2 = load_data([train_data_class2_path])

# Create labels for your data (0 for class 1, 1 for class 2)
labels_class1 = np.zeros(len(train_data_class1))
labels_class2 = np.ones(len(train_data_class2))

# Combine data and labels for training
train_labels = np.concatenate((labels_class1, labels_class2))
train_data = np.concatenate((train_data_class1, train_data_class2))

# # Shuffle the data
# perm = np.random.permutation(len(train_data))
# train_data = train_data[perm]
# train_labels = train_labels[perm]

# Convert labels to one-hot encoding for softmax activation
num_classes = 2  # Set number of classes
train_labels = tf.keras.utils.to_categorical(train_labels, num_classes=num_classes)

# Build your model
model = models.Sequential([
    layers.Dense(10, activation='relu', input_shape=(1,)),  # Assuming one feature
    layers.Dense(300, activation='relu'),
    layers.Dense(200, activation='relu'),
    layers.Dense(100, activation='relu'),
    layers.Dense(50, activation='relu'),
    layers.Dense(num_classes, activation='softmax')
])

model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])

# Train the model
model.fit(train_data, train_labels, epochs=200, batch_size=100)

# Uncomment the following line based on whether you want to print continuous data or predict using the trained model
# print_continuous_data()
predict_continuous_data(model, array_size=500)


