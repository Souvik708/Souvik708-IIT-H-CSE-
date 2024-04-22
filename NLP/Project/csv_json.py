import csv
import json

# Define the file paths
input_csv_file = "Manually_Extracted_Dataset - Sheet1.csv"
output_json_file = "output_file.json"

# Read data from CSV and convert to JSON
data = []
with open(input_csv_file, "r", newline="", encoding="utf-8") as csv_file:
    csv_reader = csv.DictReader(csv_file)
    for row in csv_reader:
        data.append(row)

# Write data to JSON file
with open(output_json_file, "w", encoding="utf-8") as json_file:
    json.dump(data, json_file, indent=4)

print("Conversion completed. JSON file created:", output_json_file)
