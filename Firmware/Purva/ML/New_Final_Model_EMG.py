import numpy as np
import tensorflow as tf
from tensorflow.keras import layers, models
import os
import re
import serial


def clean_and_extract_floats(file_path):
    clean_floats = []
    with open(file_path, 'r') as file:
        for line in file:
            cleaned_line = ''.join(char for char in line if char.isdigit() or char in ['.', '-'])
            cleaned_line = cleaned_line.split('.')[0]

            try:
                clean_floats.append(float(cleaned_line))
            except ValueError:
                print(f"Skipping invalid value: {cleaned_line}")

    return clean_floats

file1_path = '/home/purva/EMG_ArmBand/ML/assets/R_t_W_DONE_10sec.txt'
file2_path = '/home/purva/EMG_ArmBand/ML/assets/W_t_R_10sec_DONE.txt'


array_from_file1 = np.array(clean_and_extract_floats(file1_path))
array_from_file2 = np.array(clean_and_extract_floats(file2_path))

# Reshape arrays to have one column and rows as the length of the data
reshaped_array_from_file1 = np.array(array_from_file1).reshape((-1, 1))
reshaped_array_from_file2 = np.array(array_from_file2).reshape((-1, 1))

# # Print reshaped arrays
# print("Reshaped Array from file 1:")
# print(reshaped_array_from_file1)
#
# print("\nReshaped Array from file 2:")
# print(reshaped_array_from_file2)

# Load the data and labels for Class A
classA_data = reshaped_array_from_file1
classA_labels = np.zeros(len(classA_data), dtype=int)  # Label for Class A is 0

# Load the data and labels for Class B
classB_data = reshaped_array_from_file2
classB_labels = np.ones(len(classB_data), dtype=int)  # Label for Class B is 1


# Combine data and labels for both classes
data = np.vstack((classA_data, classB_data))
labels = np.concatenate((classA_labels, classB_labels))


# Shuffle the data and labels
indices = np.arange(len(data))
np.random.shuffle(indices)
data = data[indices]
labels = labels[indices]
labels = labels.reshape((-1, 1))

num_classes = 2  # Set number of classes

# Build your model
model = models.Sequential([
    layers.Dense(400, activation='relu', input_shape=(1,)),   # Assuming one feature
    layers.Dense(300, activation='relu'),
    layers.Dense(200, activation='relu'),
    layers.Dense(100, activation='relu'),
    layers.Dense(50, activation='relu'),
    layers.Dense(1, activation='sigmoid')  # Use 1 output node for binary classification
])

# Compile the model
model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])

# Train the model
model.fit(data, labels, epochs=20, batch_size=32)

# Evaluate the model on the test data
test_loss, test_accuracy = model.evaluate(data, labels)

print(f"Test Loss: {test_loss}")
print(f"Test Accuracy: {test_accuracy}")


# Save the trained model
model.save('trained_model.h5')


ser = serial.Serial('/dev/ttyACM0', baudrate=9600)  # Replace '/dev/ttyUSB0' with the appropriate port and set baudrate correctly

batch_size = 2000


# Load the trained model
loaded_model = tf.keras.models.load_model('/home/purva/EMG_ArmBand/ML/trained_model.h5')


# Continuous prediction loop
while True:
    # Read batch_size values from serial
    serial_values = [float(match.group()) for match in re.finditer(r'-?\d+(\.\d+)?', ser.readline().decode('utf-8', 'ignore').strip())]
    # Reshape the array to match the input shape of the model
    input_array = np.array(serial_values).reshape((-1, 1))

    # Make predictions using the trained model
    predictions = loaded_model.predict(input_array)

    # Convert probabilities to class predictions using a threshold (0.5 in this case)
    class_predictions = (predictions >= 0.5).astype(int)

    # Print the class predictions or use them as needed
    print(class_predictions)































