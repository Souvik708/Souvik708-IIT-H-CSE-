import subprocess
import os

c_program_name = "DAM1-CS23MTECH02001.cpp"  # Replace with your C program filename

current_directory = os.getcwd()
c_program_path = os.path.join(current_directory, c_program_name)

for _ in range(5):
	subprocess.run(["g++", c_program_path, "-o", "temp_program", "-pthread"])  # Compile the C program
	subprocess.run(["./temp_program"])  # Run the compiled program
